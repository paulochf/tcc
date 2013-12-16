static void on_announce_done(tr_session * session, bool did_connect, bool did_timeout,
    long response_code, const void * msg, size_t msglen, void * vdata) {
    tr_announce_response * response;
    struct announce_data * data = vdata;
    response = &data->response;
    (...)

    // lê os dados da resposta do tracker
    if (variant_loaded && tr_variantIsDict(&benc)) {
        int64_t i; size_t len; tr_variant * tmp; // variáveis ...
        const char * str; const uint8_t * raw;   // ... temporárias

        if (tr_variantDictFindStr(&benc, TR_KEY_failure_reason, &str, &len))
            response->errmsg = tr_strndup(str, len);

        if (tr_variantDictFindStr(&benc, TR_KEY_tracker_id, &str, &len))
            response->tracker_id_str = tr_strndup(str, len);

        if (tr_variantDictFindInt(&benc, TR_KEY_complete, &i))
            response->seeders = i;

        (...)

        if (tr_variantDictFindRaw(&benc, TR_KEY_peers, &raw, &len)) {
            response->pex = tr_peerMgrCompactToPex(raw, len,
            NULL, 0, &response->pex_count);
        }
        else if (tr_variantDictFindList(&benc, TR_KEY_peers, &tmp)) {
            response->pex = listToPex(tmp, &response->pex_count);
        }
    }

    tr_runInEventThread(session, on_announce_done_eventthread, data);
}