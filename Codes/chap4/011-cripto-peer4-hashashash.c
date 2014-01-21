    /* 3 A->B: HASH('req1', S),
               HASH('req2', SKEY) xor HASH('req3', S),
               ENCRYPT(VC, crypto_provide, len(PadC), PadC, len(IA)),
               ENCRYPT(IA)
    */
    (...)
    /* now send these: HASH ('req1', S), HASH ('req2', SKEY) xor HASH ('req3', S),
     * ENCRYPT (VC, crypto_provide, len (PadC), PadC, len (IA)), ENCRYPT (IA) */
    outbuf = evbuffer_new();

    /* HASH ('req1', $S$) */
    {
        uint8_t req1[SHA_DIGEST_LENGTH];
        tr_sha1(req1, "req1", 4, secret, KEY_LEN, NULL);
        evbuffer_add(outbuf, req1, SHA_DIGEST_LENGTH);
    }

    /* HASH ('req2', SKEY) xor HASH ('req3', $S$) */
    // SKEY = chave compartilhada para evitar ataques de homem no meio.
    // No BitTorrent, SKEY = info hash do torrent
    {
        int i;
        uint8_t req2[SHA_DIGEST_LENGTH], buf[SHA_DIGEST_LENGTH],
                req3[SHA_DIGEST_LENGTH];

        tr_sha1(req2, "req2", 4, tr_cryptoGetTorrentHash(handshake->crypto),
            SHA_DIGEST_LENGTH, NULL);
        tr_sha1(req3, "req3", 4, secret, KEY_LEN, NULL);

        for (i = 0; i < SHA_DIGEST_LENGTH; ++i)
            buf[i] = req2[i] ^ req3[i];

        evbuffer_add(outbuf, buf, SHA_DIGEST_LENGTH);
    }

    /* ENCRYPT (VC, crypto_provide, len (PadC), PadC */
    // - VC: constante de verificação ("verification constant"), que é uma string de
    //       8 bytes de valor 0x00 usada para verificar se a outra parte conhece $S$ e
    //       SKEY, evitando ataques de por repetição do SKEY.
    //
    // - crypto_provide: bitfields de 32 bits. Dois valores são usados atualmente, o
    //                   resto está reservado para uso futuro. O peer A deve ligar os
    //                   bits de todos os métodos suportados por ele; o peer B deve
    //                   ligar o bit do método escolhido entre os oferecidos.
    //                   0x01 \rightarrow sem encriptação; 0x02 \rightarrow RC4

    PadC is reserved for future extensions to the handshake...
    standard practice at this time is for it to be zero-length
    ENCRYPT() is RC4, that uses one of the following keys to send data:
    "HASH('keyA', S, SKEY)" if you're A
    "HASH('keyB', S, SKEY)" if you're B
    The first 1024 bytes of the RC4 output are discarded.
    consecutive calls to ENCRYPT() by one side continue the encryption
    stream (no reinitialization, no keychange). They are only used to distinguish
    semantically seperate content.
    {
        uint8_t vc[VC_LENGTH] = { 0, 0, 0, 0, 0, 0, 0, 0 };

        tr_peerIoWriteBuf(handshake->io, outbuf, false);
        tr_cryptoEncryptInit(handshake->crypto);
        tr_peerIoSetEncryption(handshake->io, PEER_ENCRYPTION_RC4);

        evbuffer_add(outbuf, vc, VC_LENGTH);
        evbuffer_add_uint32(outbuf, getCryptoProvide(handshake));
        evbuffer_add_uint16(outbuf, 0);
    }

    (...)