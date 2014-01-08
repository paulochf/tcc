// uint32_t index: índice $i$ da parte adquirida
static void protocolSendHave(tr_peerMsgs * msgs, uint32_t index) {
    struct evbuffer * out = msgs->outMessages; // buffer de saída

    evbuffer_add_uint32(out, sizeof(uint8_t) + sizeof(uint32_t)); // <tamanho> = 1B + 4B
    evbuffer_add_uint8(out, BT_HAVE);                             // <ID da mensagem> = 4
    evbuffer_add_uint32(out, index);                              // <indice> = indice $i$

    dbgmsg(msgs, "sending Have %u", index);
    (...)
}