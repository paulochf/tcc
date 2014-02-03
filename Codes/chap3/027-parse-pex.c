// Recebimento e processamento de uma mensagem PEX.
static void parseUtPex(tr_peerMsgs * msgs, int msglen, struct evbuffer * inbuf) {
    int loaded = 0; size_t added_len; tr_variant val; const uint8_t * added;
    uint8_t * tmp = tr_new(uint8_t, msglen);
    tr_torrent * tor = msgs->torrent;

    tr_peerIoReadBytes(msgs->io, inbuf, tmp, msglen);

    if (tr_torrentAllowsPex(tor) && (
        (loaded = !tr_variantFromBenc(&val, tmp, msglen)) )) {
        if (tr_variantDictFindRaw(&val, TR_KEY_added, &added, &added_len)) {
            tr_pex * pex; size_t i, n; size_t added_f_len = 0; const uint8_t * added_f = NULL;

            tr_variantDictFindRaw(&val, TR_KEY_added_f, &added_f, &added_f_len);
            pex = tr_peerMgrCompactToPex(added, added_len, added_f, added_f_len, &n);

            n = MIN(n, MAX_PEX_PEER_COUNT);
            for (i = 0; i < n; ++i) {
                (...)
                tr_peerMgrAddPex(tor, TR_PEER_FROM_PEX, pex + i, seedProbability);
            }
            (...)
        }
    }
    (...)
}