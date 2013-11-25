// ./libtransmission/announcer-common.h:127
typedef struct {
  tr_announce_event event;
  bool partial_seed;
  int port;                   // porta escutada para aguardo de chamada de peers
  int key;                    // chave de sessão
  int numwant;                // qtd de peers que o cliente deseja receber do tracker
  uint64_t up;                // qtd de bytes enviados desde o último evento 'started'
  uint64_t down;              // qtd de bytes "bons" recebidos desde o último 'started'
  uint64_t corrupt;       // qtd de bytes corrompidos recebidos desde o último 'started'
  uint64_t leftUntilComplete; // qtd de bytes que faltam para o fim do download
  char * url;                 // a URL de announce do tracker
  char * tracker_id_str;      // chave gerada por um tracker HTTP e devolvida
  char peer_id[PEER_ID_LEN];  // o ID do peer
  uint8_t info_hash[SHA_DIGEST_LENGTH]; // info_hash do torrent
  char log_name[128];
}
tr_announce_request;

// ./libtransmission/announcer.c:1200
static void announce_request_delegate(tr_announcer * announcer,
    tr_announce_request * request, tr_announce_response_func * callback,
    void * callback_data) {
  tr_session * session = announcer->session;
  (...)
  if (!memcmp(request->url, "http", 4))        // announce começa com "http://"
    tr_tracker_http_announce(session, request, callback, callback_data);
  else if (!memcmp(request->url, "udp://", 6)) // announce começa com "udp://"
    tr_tracker_udp_announce(session, request, callback, callback_data);
  else
    tr_logAddError("Unsupported url: %s", request->url);
  (...)
}

// ./libtransmission/web.c:260
static struct tr_web_task *
tr_webRunImpl(tr_session * session, int torrentId, const char * url,
    const char * range, const char * cookies, tr_web_done_func done_func,
    void * done_func_user_data, struct evbuffer * buffer) {
  struct tr_web_task * task = NULL;

  if (!session->isClosing) {
    if (session->web == NULL) {
      tr_threadNew(tr_webThreadFunc, session);

      while (session->web == NULL)
        tr_wait_msec(20);
    }

    task = tr_new0(struct tr_web_task, 1);
    task->session = session;
    task->torrentId = torrentId;
    task->url = tr_strdup(url);
    task->range = tr_strdup(range);
    task->cookies = tr_strdup(cookies);
    task->done_func = done_func;
    task->done_func_user_data = done_func_user_data;
    task->response = buffer ? buffer : evbuffer_new();
    task->freebuf = buffer ? NULL : task->response;

    tr_lockLock(session->web->taskLock);
    task->next = session->web->tasks;
    session->web->tasks = task;
    tr_lockUnlock(session->web->taskLock);
  }

  return task;
}