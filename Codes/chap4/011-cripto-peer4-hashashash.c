    /* 3 $A \rightarrow B$ now send these:
     * HASH ('req1', S), HASH ('req2', SKEY) xor HASH ('req3', S),
     *     ENCRYPT (VC, crypto_provide, len (PadC), PadC, len (IA)), ENCRYPT (IA) */
    outbuf = evbuffer_new();

    /* HASH ('req1', $S$) */
    {
        uint8_t req1[SHA_DIGEST_LENGTH];
        tr_sha1(req1, "req1", 4, secret, KEY_LEN, NULL);
        evbuffer_add(outbuf, req1, SHA_DIGEST_LENGTH);
    }

    /* HASH ('req2', SKEY) xor HASH ('req3', $S$) */
    // SKEY = chave compartilhada para evitar ataques de homem no meio.
    // No BitTorrent, SKEY = info hash do torrent.
    {
        int i;
        uint8_t req2[SHA_DIGEST_LENGTH], buf[SHA_DIGEST_LENGTH],
                req3[SHA_DIGEST_LENGTH];

        tr_sha1(req2, "req2", 4, tr_cryptoGetTorrentHash(handshake->crypto),
            SHA_DIGEST_LENGTH, NULL);                       // HASH ('req2', SKEY)
        tr_sha1(req3, "req3", 4, secret, KEY_LEN, NULL);    // HASH ('req3', SKEY)

        for (i = 0; i < SHA_DIGEST_LENGTH; ++i)
            buf[i] = req2[i] ^ req3[i];

        evbuffer_add(outbuf, buf, SHA_DIGEST_LENGTH);
    }




    /* ENCRYPT (VC, crypto_provide, len (PadC), PadC */
    {
        uint8_t vc[VC_LENGTH] = { 0, 0, 0, 0, 0, 0, 0, 0 };

        tr_peerIoWriteBuf(handshake->io, outbuf, false);
        tr_cryptoEncryptInit(handshake->crypto);                    // ENCRYPT(...
        tr_peerIoSetEncryption(handshake->io, PEER_ENCRYPTION_RC4);

        evbuffer_add(outbuf, vc, VC_LENGTH);                        // ...VC, ...
        evbuffer_add_uint32(outbuf, getCryptoProvide(handshake));   // ...crypto_provide, ...
        evbuffer_add_uint16(outbuf, 0);                             // ..., 0 bytes, ...
    }

    /* ...len (IA)), ENCRYPT (IA) */
    {
        uint8_t msg[HANDSHAKE_SIZE];
        if (!buildHandshakeMessage(handshake, msg))
            return tr_handshakeDone(handshake, false);

        evbuffer_add_uint16(outbuf, sizeof(msg));                   // ...len (IA)),
        evbuffer_add(outbuf, msg, sizeof(msg));                     // ... ENCRYPT (IA)
        (...)
    }

    tr_cryptoDecryptInit(handshake->crypto);
    (...)