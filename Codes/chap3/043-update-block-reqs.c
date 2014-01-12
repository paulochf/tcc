// Verifica a possibilidade de comunicação com um dado peer e envia requisições de blocos
static void updateBlockRequests(tr_peerMsgs * msgs) {
    (...)
    blocks = tr_new(tr_block_index_t, numwant); // Cria um vetor de blocos.

    // Monta a lista de quais blocos pedir.
    tr_peerMgrGetNextRequests(msgs->torrent, &msgs->peer, numwant, blocks, &n, false);

    for (i = 0; i < n; ++i) {
        struct peer_request req;
        blockToReq(msgs->torrent, blocks[i], &req); // Cria uma requisição para um bloco.
        protocolSendRequest(msgs, &req);            // Envia uma mensagem de request.
    }
}