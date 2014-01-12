// elemento do vetor de partes desejadas
struct weighted_piece {
    tr_piece_index_t index; // índice $i$ da parte
    int16_t salt;           // valor aleatório entre 0 e 4096
    int16_t requestCount;   // qtd de requisições feitas
};