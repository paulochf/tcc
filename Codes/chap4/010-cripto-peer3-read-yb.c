/* 2 $A \leftarrow B$: Diffie Hellman $Y_B$, PadB */
static int readYb(tr_handshake * handshake, struct evbuffer * inbuf) {
    int isEncrypted;
    const uint8_t * secret;
    uint8_t yb[KEY_LEN];
    struct evbuffer * outbuf;
    size_t needlen = HANDSHAKE_NAME_LEN;

    (...)
    isEncrypted = memcmp(evbuffer_pullup(inbuf, HANDSHAKE_NAME_LEN),
                    HANDSHAKE_NAME, HANDSHAKE_NAME_LEN);
    (...)
    tr_peerIoSetEncryption(handshake->io, isEncrypted ? PEER_ENCRYPTION_RC4
                                                      : PEER_ENCRYPTION_NONE);

    if (!isEncrypted) {
        setState(handshake, AWAITING_HANDSHAKE); // Handshake não encriptado acaba aqui.
        return READ_NOW;
    }

    (...)
    /* compute the secret $S$*/
    evbuffer_remove(inbuf, yb, KEY_LEN);
    secret = tr_cryptoComputeSecret(handshake->crypto, yb);
    memcpy(handshake->mySecret, secret, KEY_LEN);