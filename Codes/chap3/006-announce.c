static void announce_request_delegate(tr_announcer * announcer,
    tr_announce_request * request, tr_announce_response_func * callback,
    void * callback_data) {
  (...)
  if (!memcmp(request->url, "http", 4))        // announce começa com "http://"
    tr_tracker_http_announce(session, request, callback, callback_data);
  else if (!memcmp(request->url, "udp://", 6)) // announce começa com "udp://"
    tr_tracker_udp_announce(session, request, callback, callback_data);
  else
    tr_logAddError("Unsupported url: %s", request->url);
  (...)
}