int send_nodes_peers(const struct sockaddr *sa, int salen, const unsigned char *tid,
    int tid_len, const unsigned char *nodes, int nodes_len, const unsigned char *nodes6,
    int nodes6_len, int af, struct storage *st, const unsigned char *token, int token_len) {
    char buf[2048]; int i = 0, rc, j0, j, k, len; // variáveis temporárias

    rc = snprintf(buf + i, 2048 - i, "d1:rd2:id20:");
    INC(i, rc, 2048);
    COPY(buf, i, myid, 20, 2048);
    if (nodes_len > 0) {
        // Monta a string bencode dos nós a serem enviados.
        rc = snprintf(buf + i, 2048 - i, "5:nodes%d:", nodes_len);
        INC(i, rc, 2048);
        COPY(buf, i, nodes, nodes_len, 2048);
    }

    (...)

    // find_node: token_len = 0
    // get_peers: token_len > 0
    if (token_len > 0) {
        // Adiciona o token na resposta da chamada.
        rc = snprintf(buf + i, 2048 - i, "5:token%d:", token_len);
        INC(i, rc, 2048);
        COPY(buf, i, token, token_len, 2048);
    }

    // find_node: st = NULL
    // get_peers: st != NULL
    if (st && st->numpeers > 0) {
        /* We treat the storage as a circular list, and serve a randomly
         chosen slice.  In order to make sure we fit within 1024 octets,
         we limit ourselves to 50 peers. */

        len = af == AF_INET ? 4 : 16;
        j0 = random() % st->numpeers;
        j = j0;
        k = 0;

        rc = snprintf(buf + i, 2048 - i, "6:valuesl");
        INC(i, rc, 2048);
        do {
            if (st->peers[j].len == len) {
                unsigned short swapped;
                swapped = htons(st->peers[j].port);
                rc = snprintf(buf + i, 2048 - i, "%d:", len + 2);
                INC(i, rc, 2048);
                COPY(buf, i, st->peers[j].ip, len, 2048);
                COPY(buf, i, &swapped, 2, 2048);
                k++;
            }
            j = (j + 1) % st->numpeers;
        } while (j != j0 && k < 50);
        rc = snprintf(buf + i, 2048 - i, "e");
        INC(i, rc, 2048);
    }

    rc = snprintf(buf + i, 2048 - i, "e1:t%d:", tid_len);
    INC(i, rc, 2048);
    COPY(buf, i, tid, tid_len, 2048);
    ADD_V(buf, i, 2048);
    rc = snprintf(buf + i, 2048 - i, "1:y1:re");
    INC(i, rc, 2048);

    return dht_send(buf, i, 0, sa, salen);
    (...)
}