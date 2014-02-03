    (...)
    /**
     * (...)
     * Peers which have a better upload rate (as compared to the downloaders) but
     * aren't interested get unchoked. If they become interested, the downloader with
     * the worst upload rate gets choked. If a client has a complete file, it uses its
     * upload rate rather than its download rate to decide which peers to unchoke.
     * (...)
     */

    // O vetor choke está ordenado do peer mais interessante para o menos interessante.
    // Sendo $N$ = session->uploadSlotsPerTorrent, que é a qtd de peers que receberá
    // uploads simultaneamente, $N$ peers serão unchoked. Se já tiver chegado ao limite
    // máximo, ele mantém o estado anterior.
    unchokedInterested = 0;
    for (i = 0; i < size && unchokedInterested < session->uploadSlotsPerTorrent; ++i) {
        choke[i].isChoked = isMaxedOut ? choke[i].wasChoked : false;
        if (choke[i].isInterested) ++unchokedInterested;
    }

    // Se ainda existirem peers na lista, e nenhum no modo otimista, monta-se o sorteio para
    // escolher um.
    if (!s->optimistic && !isMaxedOut && (i < size)) {
        int n; struct ChokeData * c;
        tr_ptrArray randPool = TR_PTR_ARRAY_INIT;

        for (; i < size; ++i) {
            if (choke[i].isInterested) {
                const tr_peerMsgs * msgs = choke[i].msgs;
                int x = 1, y;
                if (isNew(msgs)) x *= 3; // peers novos têm mais chance no sorteio
                for (y = 0; y < x; ++y)
                    tr_ptrArrayAppend(&randPool, &choke[i]);
            }
        }

        // Sorteia um peer aleatoriamente para o unchoke otimista.
        if ((n = tr_ptrArraySize(&randPool))) {
            c = tr_ptrArrayNth(&randPool, tr_cryptoWeakRandInt(n));
            c->isChoked = false;
            s->optimistic = c->msgs;
            s->optimisticUnchokeTimeScaler = OPTIMISTIC_UNCHOKE_MULTIPLIER;
        }
        (...)
    }

    // Envia mensagens choke.
    for (i = 0; i < size; ++i)
        tr_peerMsgsSetChoke(choke[i].msgs, choke[i].isChoked);
    (...)
}