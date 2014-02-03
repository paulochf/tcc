/* determines who we send "interested" messages to */
static void rechokeDownloads(tr_swarm * s) {
    int i, maxPeers, rechoke_count = 0;
    struct tr_rechoke_info * rechoke = NULL;
    const int MIN_INTERESTING_PEERS = 5;
    const int peerCount = tr_ptrArraySize(&s->peers);
    const time_t now = tr_time();

    if (tr_torrentIsSeed(s->tor)) return; // Não é necessário se já somos um seeder.
    if (!tr_torrentIsPieceTransferAllowed(s->tor, TR_PEER_TO_CLIENT)) return;

    /* decide HOW MANY peers to be interested in */
    {
        int blocks = 0, cancels = 0; time_t timeSinceCancel;

        // Conta quantos blocos pedidos e cancelados cada peer tem. Isso é devido ao
        // fato de que o Transmission gastou toda a banda de rede disponível, e agora
        // precisa saber quantos peers utlizará. Peers não responsivos não serão
        // levados em conta.
        for (i = 0; i < peerCount; ++i) {
            const tr_peer * peer = tr_ptrArrayNth(&s->peers, i);
            const int b = tr_historyGet(&peer->blocksSentToClient, now, CANCEL_HISTORY_SEC);
            const int c = tr_historyGet(&peer->cancelsSentToPeer, now, CANCEL_HISTORY_SEC);

            if (b == 0) continue; // ignora peers não responsivos
            blocks += b; cancels += c;
        }

        if (cancels > 0) {
            /* cancelRate: of the block requests we've recently made, the percentage
             * we cancelled. higher values indicate more congestion. */
            const double cancelRate = cancels / (double) (cancels + blocks);
            const double mult = 1 - MIN(cancelRate, 0.5);
            maxPeers = s->interestedCount * mult;
            tordbg(s, "cancel rate is %.3f -- reducing the number of peers we're interested "
                "in by %.0f percent", cancelRate, mult * 100);
            s->lastCancel = now;
        }

        timeSinceCancel = now - s->lastCancel;
        if (timeSinceCancel) {
            const int maxIncrease = 15;
            const time_t maxHistory = 2 * CANCEL_HISTORY_SEC;
            const double mult = MIN(timeSinceCancel, maxHistory) / (double) maxHistory;
            const int inc = maxIncrease * mult;
            maxPeers = s->maxPeers + inc;
            tordbg(s, "time since last cancel is %li -- increasing the number of peers"
                "we're interested in by %d", timeSinceCancel, inc);
        }
    }

    /* don't let the previous section's number tweaking go too far... */
    if (maxPeers < MIN_INTERESTING_PEERS) maxPeers = MIN_INTERESTING_PEERS;
    if (maxPeers > s->tor->maxConnectedPeers) maxPeers = s->tor->maxConnectedPeers;
    s->maxPeers = maxPeers;
    (...)