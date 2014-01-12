// Função que cria a lista de blocos para serem requisitados a partir da lista de partes.
void tr_peerMgrGetNextRequests(tr_torrent * tor, tr_peer * peer, int numwant,
    tr_block_index_t * setme, int * numgot, bool get_intervals)
{
    int i, got = 0;
    struct weighted_piece * pieces;
    tr_swarm * s = tor->swarm;
    const tr_bitfield * const have = &peer->have;
    (...)

    // Se não existir uma lista de partes, cria uma já ordenada
    if (s->pieces == NULL) pieceListRebuild(s);

    // Caso já exista a lista de partes, verifica o estado da ordenação por importância.
    if (s->pieceSortState != PIECES_SORTED_BY_WEIGHT)
        pieceListSort(s, PIECES_SORTED_BY_WEIGHT);

    (...)
    // Confirma o estado do torrent, verificando se está no modo de fim de jogo.
    updateEndgame(s); pieces = s->pieces;

    // Para cada parte que falta ser baixada, ...
    for (i = 0; i < s->pieceCount && got < numwant; ++i) {
        struct weighted_piece * p = pieces + i;

        // ... se o peer tiver a parte desejada, ....
        if (tr_bitfieldHas(have, p->index)) {
            tr_block_index_t b, first, last;
            tr_ptrArray peerArr = TR_PTR_ARRAY_INIT;

            // ... calcula o intervalo de bytes da parte.
            tr_torGetPieceBlockRange(tor, p->index, &first, &last);

            // Dentro do intervalo de bytes da parte:
            for (b = first; b <= last && (...)) {
                int peerCount; tr_peer ** peers;

                // Ignore blocos já possuídos.
                if (tr_cpBlockIsComplete(&tor->completion, b)) continue;

                // Encontra peers novos para se requisitar o bloco,...
                // ... juntando às antigas requisições para ele.
                tr_ptrArrayClear(&peerArr);
                getBlockRequestPeers(s, b, &peerArr);
                peers = (tr_peer **) tr_ptrArrayPeek(&peerArr, &peerCount);
                if (peerCount != 0) {
                    // Não faça uma 2ª requisição antes do fim do jogo.
                    if (!s->endgame) continue;

                    // Não requisite o bloco para mais de 1 peer simultaneamente.
                    if (peerCount > 1) continue;

                    // Não refaça a requisição para o mesmo peer.
                    if (peer == peers[0]) continue;

                    // No modo de fim de jogo, permita requisitar para um 2º peer...
                    // ... somente se este estiver atendendo pedidos rapidamente.
                    if (peer->pendingReqsToPeer + numwant - got < s->endgame) continue;
                }
                (...)

                // Atualiza a lista de blocos pedidos.
                requestListAdd(s, b, peer);
                ++p->requestCount;
            }
            (...)
        }
    }
    (...)
}