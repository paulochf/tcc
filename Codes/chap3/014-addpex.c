void tr_peerMgrAddPex(tr_torrent * tor, uint8_t from, const tr_pex * pex,
    int8_t seedProbability) {
    if (tr_isPex(pex)) { /* safeguard against corrupt data */
        tr_swarm * s = tor->swarm;
        managerLock(s->manager);

        if (!tr_sessionIsAddressBlocked(s->manager->session, &pex->addr))
            if (tr_address_is_valid_for_peers(&pex->addr, pex->port))
                ensureAtomExists(s, &pex->addr, pex->port, pex->flags, seedProbability, from);

        managerUnlock(s->manager);
    }
}