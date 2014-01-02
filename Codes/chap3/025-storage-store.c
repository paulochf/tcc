static int storage_store(const unsigned char *id, const struct sockaddr *sa,
    unsigned short port) {
    int i, len; struct storage *st; unsigned char *ip;

    (...)
    st = find_storage(id); // Encontra o banco para o ID (hash do torrent) dado.

    if (st == NULL) {   // Se não existe banco, cria um para o torrent e adiciona na...
        (...)           // ... lista de bancos.
        st = calloc(1, sizeof(struct storage));
        (...)
        memcpy(st->id, id, 20);
        st->next = storage;
        storage = st;
        numstorage++;
    }

    // Procura pelo peer passado.
    for (i = 0; i < st->numpeers; i++) {
        if (st->peers[i].port == port &&
            st->peers[i].len == len &&
            memcmp(st->peers[i].ip, ip, len) == 0) break;
    }

    // Se o peer já existir, só atualiza o momento da última notificação.
    if (i < st->numpeers) {
        /* Already there, only need to refresh */
        st->peers[i].time = now.tv_sec;
        return 0;
    }
    else {
        struct peer *p;
        // Se não tiver espaço para o peer novo, expande o banco.
        if (i >= st->maxpeers) {
            /* Need to expand the array. */
            struct peer *new_peers; int n;

            // Não tem mais espaço.
            if (st->maxpeers >= DHT_MAX_PEERS) return 0;

            // Expande o banco criando 2 espaços ou dobrando de tamanho, limitando em...
            // ...DHT_MAX_PEERS espaços;
            n = st->maxpeers == 0 ? 2 : 2 * st->maxpeers;
            n = MIN(n, DHT_MAX_PEERS);
            new_peers = realloc(st->peers, n * sizeof(struct peer));
            if (new_peers == NULL) return -1;
            st->peers = new_peers; st->maxpeers = n;
        }
        // Adiciona o novo peer no banco.
        p = &st->peers[st->numpeers++];
        p->time = now.tv_sec; p->len = len;
        p->port = port; memcpy(p->ip, ip, len);
        return 1;
    }
}