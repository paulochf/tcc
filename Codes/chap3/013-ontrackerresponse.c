static void onTrackerResponse(tr_torrent * tor, const tr_tracker_event * event,
    void * unused UNUSED) {
    switch (event->messageType) {
        case TR_TRACKER_PEERS: {
            size_t i;
            (...)
            for (i = 0; i < event->pexCount; ++i)
                tr_peerMgrAddPex(tor, TR_PEER_FROM_TRACKER, &event->pex[i], seedProbability);
            break;
        }
        (...)
    }
}