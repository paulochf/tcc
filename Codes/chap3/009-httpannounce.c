void tr_tracker_http_announce(tr_session * session, const tr_announce_request * request,
        tr_announce_response_func response_func, void * response_func_user_data) {
    char * url = announce_url_new(session, request);
    struct announce_data * d = tr_new0(struct announce_data, 1);
    (...) // preenchimento dos campos da variável 'd'

    tr_webRun(session, url, on_announce_done, d);
    (...)
}