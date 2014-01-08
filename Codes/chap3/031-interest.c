// bool b:  true $\Longrightarrow$ interested
//         false $\Longrightarrow$ not interested
static void sendInterest(tr_peerMsgs * msgs, bool b) {
    struct evbuffer * out = msgs->outMessages; // buffer de saída
    (...)
    dbgmsg(msgs, "Sending %s", b ? "Interested" : "Not Interested");
    evbuffer_add_uint32(out, sizeof(uint8_t)); // <tamanho> = 1 byte

    // <ID da mensagem>: BT_INTERESTED (2) ou BT_NOT_INTERESTED (3)
    evbuffer_add_uint8(out, b ? BT_INTERESTED : BT_NOT_INTERESTED);
    (...)
}