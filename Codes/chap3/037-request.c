// const struct peer_request * req: requisição a ser cancelada
static void protocolSendRequest(tr_peerMsgs * msgs, const struct peer_request * req) {
    struct evbuffer * out = msgs->outMessages; // buffer de saída

    // sizeof(uint8_t) + 3 * sizeof(uint32_t) = 1B + 3 * 4B = 13B
    evbuffer_add_uint32(out, sizeof(uint8_t) + 3 * sizeof(uint32_t)); // <tamanho=0013>
    evbuffer_add_uint8(out, BT_REQUEST);   // <ID da mensagem> = 6

    // <dados=<índice><início><tamanho>>
    evbuffer_add_uint32(out, req->index);  // <índice>
    evbuffer_add_uint32(out, req->offset); // <início>
    evbuffer_add_uint32(out, req->length); // <tamanho>

    dbgmsg(msgs, "requesting %u:%u->%u...", req->index, req->offset, req->length);
    (...)
}