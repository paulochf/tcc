typedef struct {
  (...)
  bool partial_seed;
  int port;                   // porta "escutada" para aguardo de chamada de peers
  int key;                    // chave de sessão
  int numwant;                // qtd de peers que o cliente deseja receber do tracker
  uint64_t up;                // qtd de bytes enviados desde o último evento 'started'
  uint64_t down;              // qtd de bytes "bons" recebidos desde o último 'started'
  uint64_t corrupt;           // qtd de bytes corrompidos recebidos desde o último 'started'
  uint64_t leftUntilComplete; // qtd de bytes que faltam para o fim do download
  char * url;                 // a URL de announce do tracker
  char * tracker_id_str;      // chave gerada por um tracker HTTP e devolvida
  char peer_id[PEER_ID_LEN];  // o ID do peer
  uint8_t info_hash[SHA_DIGEST_LENGTH]; // info_hash do torrent
}
tr_announce_request;