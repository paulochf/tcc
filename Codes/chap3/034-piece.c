static size_t fillOutputBuffer(tr_peerMsgs * msgs, time_t now) {
    int piece;
    size_t bytesWritten = 0;
    struct peer_request req;

    (...)

    /**
     ***  Data Blocks
     **/
    if ((tr_peerIoGetWriteBufferSpace(msgs->io, now) >= msgs->torrent->blockSize) &&
        popNextRequest(msgs, &req))
    {   // Quando tiver espaço disponível no buffer de saída e outra requisição de...
        // ... bloco a ser atendida na fila.
        (...)

        if (requestIsValid(msgs, &req)
            && tr_cpPieceIsComplete(&msgs->torrent->completion, req.index))
        {   // Se a requisição tiver enviado parâmetros corretos e se o Transmission...
            // ... já tiver sido baixado-a.
            int err; struct evbuffer * out;
            struct evbuffer_iovec iovec[1];
            const uint32_t msglen = 4 + 1 + 4 + 4 + req.length;

            out = evbuffer_new();
            evbuffer_expand(out, msglen);

            // <tamanho=0009+X>
            // sizeof(uint8_t) + 2 * sizeof(uint32_t) = 1B + 2 * 4B = 9B
            evbuffer_add_uint32(out, sizeof(uint8_t) + 2 * sizeof(uint32_t) + req.length);
            evbuffer_add_uint8(out, BT_PIECE);    // <ID da mensagem=7>

            // <dados=<índice><início><bloco>
            evbuffer_add_uint32(out, req.index);   // <índice>
            evbuffer_add_uint32(out, req.offset);  // <início>

            // <início>
            evbuffer_reserve_space(out, req.length, iovec, 1);
            err = tr_cacheReadBlock(getSession(msgs)->cache, msgs->torrent, req.index,
                req.offset, req.length, iovec[0].iov_base); // conteúdo de dados do bloco
            iovec[0].iov_len = req.length;
            evbuffer_commit_space(out, iovec, 1);

            (...)
        }
        (...)
    }
    (...)

    return bytesWritten;
}