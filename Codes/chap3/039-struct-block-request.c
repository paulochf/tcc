// vetor de pedidos de blocos
struct block_request {
    tr_block_index_t block;
    tr_peer * peer;
    time_t sentAt;
};