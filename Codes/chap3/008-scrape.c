static void scrape_request_delegate(tr_announcer * announcer,
    const tr_scrape_request * request, tr_scrape_response_func * callback,
    void * callback_data) {
  (...)
  if (!memcmp(request->url, "http", 4))
    tr_tracker_http_scrape(session, request, callback, callback_data);
  else if (!memcmp(request->url, "udp://", 6))
    tr_tracker_udp_scrape(session, request, callback, callback_data);
  else
    tr_logAddError("Unsupported url: %s", request->url);
}