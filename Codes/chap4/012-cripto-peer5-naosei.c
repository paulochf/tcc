    (...)
    /* ENCRYPT len (IA)), ENCRYPT (IA) */
    {
        uint8_t msg[HANDSHAKE_SIZE];
        if (!buildHandshakeMessage(handshake, msg)) return tr_handshakeDone(
                handshake, false);

        evbuffer_add_uint16(outbuf, sizeof(msg));
        evbuffer_add(outbuf, msg, sizeof(msg));

        handshake->haveSentBitTorrentHandshake = 1;
    }

    /* send it */
    tr_cryptoDecryptInit(handshake->crypto);
    setReadState(handshake, AWAITING_VC);
    tr_peerIoWriteBuf(handshake->io, outbuf, false);

    /* cleanup */
    evbuffer_free(outbuf);
    return READ_LATER;
}