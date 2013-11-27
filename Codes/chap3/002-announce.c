// ./libtransmission/announcer-common.h:127
typedef struct {
  (...)
  bool partial_seed;
  int port;                   // porta escutada para aguardo de chamada de peers
  int key;                    // chave de sessão
  int numwant;                // qtd de peers que o cliente deseja receber do tracker
  uint64_t up;                // qtd bytes enviados desde o último evento 'started'
  uint64_t down;              // qtd bytes "bons" recebidos desde o último 'started'
  uint64_t corrupt;           // qtd bytes corrompidos recebidos desde o último 'started'
  uint64_t leftUntilComplete; // qtd de bytes que faltam para o fim do download
  char * url;                 // a URL de announce do tracker
  char * tracker_id_str;      // chave gerada por um tracker HTTP e devolvida
  char peer_id[PEER_ID_LEN];  // o ID do peer
  uint8_t info_hash[SHA_DIGEST_LENGTH]; // info_hash do torrent
}
tr_announce_request;

// ./libtransmission/announcer.c:1200
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