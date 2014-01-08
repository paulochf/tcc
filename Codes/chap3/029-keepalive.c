static size_t fillOutputBuffer(tr_peerMsgs * msgs, time_t now) {
    size_t bytesWritten = 0;
    (...)

    // KEEPALIVE_INTERVAL_SECS = 100
    if ((msgs != NULL) && (msgs->clientSentAnythingAt != 0)
            && ((now - msgs->clientSentAnythingAt) > KEEPALIVE_INTERVAL_SECS)) {
        dbgmsg(msgs, "sending a keepalive message");
        evbuffer_add_uint32(msgs->outMessages, 0); // Envia uma mensagem vazia.
        (...)
    }

    return bytesWritten;
}