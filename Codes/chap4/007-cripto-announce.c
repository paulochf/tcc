static char* announce_url_new(const tr_session * session, const tr_announce_request * req) {
    (...)
    tr_http_escape_sha1(escaped_info_hash, req->info_hash);
    (...)

    evbuffer_add_printf(buf, "%s%cinfo_hash=%s(...)&supportcrypto=1",
        req->url, strchr(req->url, '?') ? '&' : '?', escaped_info_hash, (...));

    if (session->encryptionMode == TR_ENCRYPTION_REQUIRED)
        evbuffer_add_printf(buf, "&requirecrypto=1");

    (...)
    return evbuffer_free_to_str(buf);
}