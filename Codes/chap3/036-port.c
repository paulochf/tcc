// uint16_t port: número da porta a ser utilizada
static void protocolSendPort(tr_peerMsgs *msgs, uint16_t port) {
    struct evbuffer * out = msgs->outMessages; // buffer de saída

    dbgmsg(msgs, "sending Port %u", port);
    evbuffer_add_uint32(out, 3);      // <tamanho=0003>
    evbuffer_add_uint8(out, BT_PORT); // <ID da mensagem=9>
    evbuffer_add_uint16(out, port);   // <dados=porta>
}