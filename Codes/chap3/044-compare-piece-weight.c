// Função que compara duas partes e calcula a importância de uma sobre a outra.
static int comparePieceByWeight(const void * va, const void * vb) {
    const struct weighted_piece * a = va;
    const struct weighted_piece * b = vb;
    int ia, ib, missing, pending;
    const tr_torrent * tor = weightTorrent;
    const uint16_t * rep = weightReplication;

    // 1o fator: peso (strict policy)
    // Uma parte que possui muitos blocos faltando e alguns pedidos tem prioridade menor...
    // ... do que uma parte que está próxima de terminar.
    missing = tr_cpMissingBlocksInPiece(&tor->completion, a->index);
    pending = a->requestCount;
    ia = missing > pending ?
            missing - pending : (tor->blockCountInPiece + pending);
    missing = tr_cpMissingBlocksInPiece(&tor->completion, b->index);
    pending = b->requestCount;
    ib = missing > pending ?
            missing - pending : (tor->blockCountInPiece + pending);
    if (ia < ib) return -1;
    if (ia > ib) return 1;

    // 2º fator: prioridade
    // O usuário pode ter assinalado para o Transmission que um dos arquivos do torrent...
    // ... deve ser baixado com prioridade maior do que outros.
    ia = tor->info.pieces[a->index].priority;
    ib = tor->info.pieces[b->index].priority;
    if (ia > ib) return -1;
    if (ia < ib) return 1;

    // 3º fator: raridade (rarest first)
    // Se um elemento da lista de partes tiver menos réplicas do que outro, então é...
    // ... mais raro, logo, tem maior prioridade.
    ia = rep[a->index];
    ib = rep[b->index];
    if (ia < ib) return -1;
    if (ia > ib) return 1;

    // 4º fator: aleatoriedade (random first piece)
    // Isso afeta o início do download, quanto não se possui nenhuma parte e os peers...
    // ... vizinhos ainda não são bem conhecidos. Nesse caso, a aleatoriedade está no...
    // ... salt (valor aleatório entre 0 e 4096).
    if (a->salt < b->salt) return -1;
    if (a->salt > b->salt) return 1;

    return 0; // partes podem ser consideradas de prioridade igual
}