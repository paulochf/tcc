typedef struct {
  // lista de info_hash de torrents para serem pesquisados por scrape
  uint8_t info_hash[TR_MULTISCRAPE_MAX][SHA_DIGEST_LENGTH];
  char * url;  // URL de scrape
  (...)
} tr_scrape_request;

struct tr_scrape_response_row { // usado para conter os dados respondidos do scrape
  uint8_t info_hash[SHA_DIGEST_LENGTH]; // info_hash do torrent
  int seeders;     // qtd de peers que são seeders deste torrent
  int leechers;    // qtd de peers baixando este torrent
  int downloads;   // qtd de vezes que este torrent foi baixado
  int downloaders; // qtd de leechers ativos no swarm (suportado por alguns trackers)
};