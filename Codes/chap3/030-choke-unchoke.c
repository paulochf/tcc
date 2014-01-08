// int choke: 1 $\Longrightarrow$   choke
//            0 $\Longrightarrow$ unchoke
static void protocolSendChoke(tr_peerMsgs * msgs, int choke) {
    struct evbuffer * out = msgs->outMessages; // buffer de saída

    evbuffer_add_uint32(out, sizeof(uint8_t)); // <tamanho> = 1 byte

    // <ID da mensagem>: BT_CHOKE (0) ou BT_UNCHOKE (1)
    evbuffer_add_uint8(out, choke ? BT_CHOKE : BT_UNCHOKE);

    dbgmsg(msgs, "sending %s...", choke ? "Choke" : "Unchoke");
    (...)
}