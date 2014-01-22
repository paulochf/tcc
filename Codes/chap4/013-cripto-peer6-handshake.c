/* 5 $A \rightarrow B$: ENCRYPT2(Payload Stream) */
(...)
    if (!handshake->haveSentBitTorrentHandshake) {
        uint8_t msg[HANDSHAKE_SIZE];
        if (!buildHandshakeMessage(handshake, msg))
            return tr_handshakeDone(handshake, false);
        tr_peerIoWriteBytes(handshake->io, msg, sizeof(msg), false);
        handshake->haveSentBitTorrentHandshake = 1;
    }
    (...)
}