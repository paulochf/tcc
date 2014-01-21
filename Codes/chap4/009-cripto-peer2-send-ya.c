/* 1 $A \rightarrow B$: Diffie Hellman $Y_A$, PadA */
static void sendYa(tr_handshake * handshake) {
    int len; const uint8_t * public_key;
    char outbuf[KEY_LEN + PadA_MAXLEN]; // 96 + 512 = 608 bytes
    char *walk = outbuf;

    /* add our public key ($Y_A$) */
    public_key = tr_cryptoGetMyPublicKey(handshake->crypto, &len);
    (...)
    memcpy(walk, public_key, len); walk += len;

    /* add some bullshit padding */
    len = tr_cryptoRandInt(PadA_MAXLEN);
    tr_cryptoRandBuf(walk, len); walk += len;

    /* send it */
    setReadState(handshake, AWAITING_YB);
    tr_peerIoWriteBytes(handshake->io, outbuf, walk - outbuf, false);
}