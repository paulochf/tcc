// Função de callback para recepção de mensagens de peers.
static void peerCallbackFunc(tr_peer * peer, const tr_peer_event * e, void * vs) {
    tr_swarm * s = vs;

    swarmLock(s);
    switch (e->eventType) {
        (...)
        case TR_PEER_CLIENT_GOT_HAVE:                        // peer avisou que tem uma parte
            if (replicationExists(s)) {
                tr_incrReplicationOfPiece(s, e->pieceIndex); // replicação para uma parte
                (...)
            }
            break;

        case TR_PEER_CLIENT_GOT_HAVE_ALL: // peer avisou que tem todas as partes
            if (replicationExists(s)) {
                tr_incrReplication(s);    // replicação para todas as partes
                (...)
            }
            break;

        (...)

        case TR_PEER_CLIENT_GOT_BITFIELD:                       // peer enviou bitfield
            (...)
            if (replicationExists(s)) {
                tr_incrReplicationFromBitfield(s, e->bitfield); // replicação usando bitfield
                (...)
            }
            break;
        (...)
    }
    swarmUnlock(s);
}