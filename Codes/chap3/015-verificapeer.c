static void ensureAtomExists(tr_swarm * s, const tr_address * addr, const tr_port port,
    const uint8_t flags, const int8_t seedProbability, const uint8_t from) {
    struct peer_atom * a = getExistingAtom(s, addr);

    (...)

    if (a == NULL) { // Se ainda não está na lista...
        const int jitter = tr_cryptoWeakRandInt(60 * 10);
        a = tr_new0(struct peer_atom, 1);
        a->addr = *addr;
        a->port = port;
        (...)

        // ...adiciona peer na lista de peers, de forma ordenada.
        tr_ptrArrayInsertSorted(&s->pool, a, compareAtomsByAddress);
    }
    (...)
}