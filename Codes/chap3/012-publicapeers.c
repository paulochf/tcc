static void publishPeersPex(tr_tier * tier, int seeds, int leechers, const tr_pex * pex,
    int n) {
    if (tier->tor->tiers->callback) {
        tr_tracker_event e = TRACKER_EVENT_INIT;
        e.pex = pex; e.pexCount = n; e.messageType = TR_TRACKER_PEERS;
        (...)
        tier->tor->tiers->callback(tier->tor, &e, NULL);
    }
}