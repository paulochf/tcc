/* 4 $A \leftarrow B$: ENCRYPT(VC, crypto_select, len(padD), padD), ENCRYPT2(Payload Stream) */
static int readVC(tr_handshake * handshake, struct evbuffer * inbuf) {
    uint8_t tmp[VC_LENGTH];
    const int key_len = VC_LENGTH;
    const uint8_t key[VC_LENGTH] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    /* note: this works w/o having to `unwind' the buffer if
     * we read too much, but it is pretty brute-force.
     * it would be nice to make this cleaner. */
    for (;;) {
        (...)
        memcpy(tmp, evbuffer_pullup(inbuf, key_len), key_len);
        tr_cryptoDecryptInit(handshake->crypto);
        tr_cryptoDecrypt(handshake->crypto, key_len, tmp, tmp); // (DE)CRYPT(VC, ...
        if (!memcmp(tmp, key, key_len)) break;
        (...)
    }
    (...)
}

static int readCryptoSelect(tr_handshake * handshake, struct evbuffer * inbuf) {
    uint16_t pad_d_len; uint32_t crypto_select;
    static const size_t needlen = sizeof(uint32_t) + sizeof(uint16_t);
    (...)

    tr_peerIoReadUint32(handshake->io, inbuf, &crypto_select); // ... crypto_select, ...
    handshake->crypto_select = crypto_select;
    (...)
    tr_peerIoReadUint16(handshake->io, inbuf, &pad_d_len);     // ... len(padD), ...
    (...)
}
static int readPadD(tr_handshake * handshake, struct evbuffer * inbuf) {
    const size_t needlen = handshake->pad_d_len;

    (...)
    tr_peerIoDrain(handshake->io, inbuf, needlen);  // ... padD), ...
    tr_peerIoSetEncryption(handshake->io, handshake->crypto_select);
    (...)
}

static int readHandshake(tr_handshake * handshake, struct evbuffer * inbuf) {
    uint8_t pstrlen;
    (...)
    /* peek, don't read. We may be handing inbuf to AWAITING_YA */
    pstrlen = evbuffer_pullup(inbuf, 1)[0];

    if (pstrlen == 19) { /* unencrypted */
        (...)            // handshake não encriptado é tratado aqui
    }
    else { /* encrypted or corrupt */
        tr_peerIoSetEncryption(handshake->io, PEER_ENCRYPTION_RC4);
        (...)
        // ENCRYPT2(Payload Stream)
        tr_cryptoDecrypt(handshake->crypto, 1, &pstrlen, &pstrlen);
        (...)
    }
    (...)