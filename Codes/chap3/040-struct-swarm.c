// Estrutura de informações sobre o swarm de um torrent.
typedef struct tr_swarm {
    (...)
    tr_ptrArray peers; // vetor de objetos peer
    tr_torrent * tor;  // informações sobre o torrent

    struct block_request * requests; // vetor de pedidos de blocos
    int requestCount;                // tamanho do vetor de pedidos de blocos

    struct weighted_piece * pieces;       // vetor de partes desejadas
    int pieceCount;                       // tamanho do vetor de partes

    // estado do vetor de partes: {não ordenado, por índice, por importância}
    enum piece_sort_state pieceSortState;

    uint16_t * pieceReplication; // vetor sobre quantos peers possuem cada parte
    size_t pieceReplicationSize; // tamanho do vetor de índices de partes comuns

    // indicador do modo de fim de jogo
    // - antes do fim de jogo: endgame = 0
    // - durante o fim de jogo: endgame = média de qtd de requisições pendentes por peer
    // Somente peers "rápidos" serão utilizados no fim de jogo.
    int endgame;
} tr_swarm;