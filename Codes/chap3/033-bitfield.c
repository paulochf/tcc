static void sendBitfield(tr_peerMsgs * msgs) {
    void * bytes; size_t byte_count = 0; // bitfield e seu comprimento
    struct evbuffer * out = msgs->outMessages; // buffer de saída

    (...)
    // Cria o bitfield conforme as partes que possui no momento.
    bytes = tr_cpCreatePieceBitfield(&msgs->torrent->completion, &byte_count);

    // <tamanho> = 1 + tamanho do bitfield
    evbuffer_add_uint32(out, sizeof(uint8_t) + byte_count);

    evbuffer_add_uint8(out, BT_BITFIELD); // <ID da mensagem> = 5
    evbuffer_add(out, bytes, byte_count); // <dados=mapa de bits>
    dbgmsg(msgs, "sending bitfield... outMessage size is now %zu", evbuffer_get_length(out));
    (...)
}