static void rechokePulse(int foo UNUSED, short bar UNUSED, void * vmgr) {
    tr_torrent * tor = NULL;
    tr_peerMgr * mgr = vmgr;
    const uint64_t now = tr_time_msec();

    managerLock(mgr);
    while ((tor = tr_torrentNext(mgr->session, tor))) {
        if (tor->isRunning) {
            tr_swarm * s = tor->swarm;

            if (s->stats.peerCount > 0) {
                rechokeUploads(s, now);
                rechokeDownloads(s);
            }
        }
    }
    tr_timerAddMsec(mgr->rechokeTimer, RECHOKE_PERIOD_MSEC);
    managerUnlock(mgr);
}