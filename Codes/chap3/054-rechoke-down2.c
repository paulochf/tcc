    if (peerCount > 0) {
        bool * piece_is_interesting;
        const tr_torrent * const tor = s->tor;
        const int n = tor->info.pieceCount;

        // Constrói um bitfield com as partes que ainda precisam ser baixadas.
        piece_is_interesting = tr_new(bool, n);
        for (i = 0; i < n; i++)
            piece_is_interesting[i] = !tor->info.pieces[i].dnd &&
                                        !tr_cpPieceIsComplete(&tor->completion, i);

        // Decide quais peers se interesará, baseado na razão de cancelamentos / pedidos
        for (i = 0; i < peerCount; ++i) {
            tr_peer * peer = tr_ptrArrayNth(&s->peers, i);

            if (!isPeerInteresting(s->tor, piece_is_interesting, peer)) {
                tr_peerMsgsSetInterested(PEER_MSGS(peer), false);
            }
            else {
                tr_rechoke_state rechoke_state;
                const int blocks = tr_historyGet(&peer->blocksSentToClient, now,
                    CANCEL_HISTORY_SEC);
                const int cancels = tr_historyGet(&peer->cancelsSentToPeer, now,
                    CANCEL_HISTORY_SEC);

                // Peers bons cancelam 10% ou menos dos pedidos.
                if (!blocks && !cancels) rechoke_state = RECHOKE_STATE_UNTESTED;
                else if (!cancels) rechoke_state = RECHOKE_STATE_GOOD;
                else if (!blocks) rechoke_state = RECHOKE_STATE_BAD;
                else if ((cancels * 10) < blocks) rechoke_state = RECHOKE_STATE_GOOD;
                else rechoke_state = RECHOKE_STATE_BAD;

                if (rechoke == NULL) rechoke = tr_new(struct tr_rechoke_info, peerCount);

                // valor aleatório entre 0 e INT_MAX
                rechoke[rechoke_count].salt = tr_cryptoWeakRandInt(INT_MAX);
                rechoke[rechoke_count].peer = peer;
                rechoke[rechoke_count].rechoke_state = rechoke_state;
                rechoke_count++;
            }
        }
        (...)
    }

    // Ordena os peers de acordo com o interesse.
    qsort(rechoke, rechoke_count, sizeof(struct tr_rechoke_info), compare_rechoke_info);
    s->interestedCount = MIN(maxPeers, rechoke_count);
    for (i = 0; i < rechoke_count; ++i) // Envia mensagens interested.
        tr_peerMsgsSetInterested(PEER_MSGS(rechoke[i].peer), i < s->interestedCount);
    (...)
}