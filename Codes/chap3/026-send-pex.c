// Envio de mensagem PEX
static void sendPex(tr_peerMsgs * msgs) {
    if (msgs->peerSupportsPex && tr_torrentAllowsPex(msgs->torrent)) {
        (...)
        // Seleciona os 50 nós conectados e que são úteis ao torrent sendo baixado.
        const int newCount = tr_peerMgrGetPeers(msgs->torrent, &newPex, TR_AF_INET,
            TR_PEERS_CONNECTED, MAX_PEX_PEER_COUNT);
        (...)

        /* build the diffs */
        diffs.added = tr_new(tr_pex, newCount); diffs.addedCount = 0;
        diffs.dropped = tr_new(tr_pex, msgs->pexCount); diffs.droppedCount = 0;
        diffs.elements = tr_new(tr_pex, newCount + msgs->pexCount); diffs.elementCount = 0;
        tr_set_compare(msgs->pex, msgs->pexCount, newPex, newCount, tr_pexCompare,
                sizeof(tr_pex), pexDroppedCb, pexAddedCb, pexElementCb, &diffs);
        (...)

        // Se tiverem novos peers a serem enviados, o faz.
        if (diffs.addedCount || diffs.droppedCount || !diffs6.addedCount ||
            diffs6.droppedCount) {
            int i; tr_variant val; uint8_t * tmp, *walk; // variáveis temporárias
            struct evbuffer * payload;                   // listas de peers do PEX
            struct evbuffer * out = msgs->outMessages;   // mensagem a ser enviada

            /* update peer */
            (...)
            msgs->pex = diffs.elements; msgs->pexCount = diffs.elementCount;
            (...)

            /* build the pex payload */
            tr_variantInitDict(&val, 3);

            if (diffs.addedCount > 0) {
                /* "added" */
                tmp = walk = tr_new(uint8_t, diffs.addedCount * 6);
                for (i = 0; i < diffs.addedCount; ++i) {
                    memcpy(walk, &diffs.added[i].addr.addr, 4); walk += 4;
                    memcpy(walk, &diffs.added[i].port, 2); walk += 2;
                }
                (...)
                tr_variantDictAddRaw(&val, TR_KEY_added, tmp, walk - tmp);
                (...)
            }
            if (diffs.droppedCount > 0) {
                /* "dropped" */
                tmp = walk = tr_new(uint8_t, diffs.droppedCount * 6);
                for (i = 0; i < diffs.droppedCount; ++i) {
                    memcpy(walk, &diffs.dropped[i].addr.addr, 4); walk += 4;
                    memcpy(walk, &diffs.dropped[i].port, 2); walk += 2;
                }
                (...)
                tr_variantDictAddRaw(&val, TR_KEY_dropped, tmp, walk - tmp);
                (...)
            }
            (...)

            /* write the pex message */
            payload = tr_variantToBuf(&val, TR_VARIANT_FMT_BENC);
            evbuffer_add_uint32(out, 2 * sizeof(uint8_t) + evbuffer_get_length(payload));
            evbuffer_add_uint8(out, BT_LTEP);
            evbuffer_add_uint8(out, msgs->ut_pex_id);
            evbuffer_add_buffer(out, payload);
            pokeBatchPeriod(msgs, HIGH_PRIORITY_INTERVAL_SECS);
            dbgmsg(msgs, "sending a pex message; outMessage size is now %zu",
                evbuffer_get_length(out));
            (...)
        }
        (...)
    }
}