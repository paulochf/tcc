static void updateEndgame(tr_swarm * s) {
    (...)

    if (!testForEndgame(s)) { /* not in endgame */
        s->endgame = 0;
    }
    else if (!s->endgame) { /* only recalculate when endgame first begins */
        int i, numDownloading = 0;
        const int n = tr_ptrArraySize(&s->peers);

        // Soma os peers BitTorrent ativos...
        for (i = 0; i < n; ++i) {
            const tr_peer * p = tr_ptrArrayNth(&s->peers, i);
            if (p->pendingReqsToPeer > 0) ++numDownloading;
        }

        // ... com os possíveis seeders web.
        numDownloading += countActiveWebseeds(s);

        // Média instantânea de requisições pendentes por peers fornecedores.
        s->endgame = s->requestCount / MAX(numDownloading, 1);
    }
}