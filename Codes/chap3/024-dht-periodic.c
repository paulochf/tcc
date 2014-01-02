int dht_periodic(const void *buf, size_t buflen, const struct sockaddr *from, int fromlen,
    time_t *tosleep, dht_callback *callback, void *closure) {
    (...)
    // variáveis temporárias
    int message;
    unsigned char tid[16], id[20], info_hash[20], target[20];
    unsigned char nodes[256], nodes6[1024], token[128];
    int tid_len = 16, token_len = 128;
    int nodes_len = 256, nodes6_len = 1024;
    unsigned short port;
    unsigned char values[2048], values6[2048];
    int values_len = 2048, values6_len = 2048;
    int want;

    (...)
    // Processa a mensagem recebida e identifica seu tipo.
    message = parse_message(buf, buflen, tid, &tid_len, id, info_hash, target,
            &port, token, &token_len, nodes, &nodes_len, nodes6, &nodes6_len,
            values, &values_len, values6, &values6_len, &want);
    (...)

    // Realiza os procedimentos conforme o tipo da mensagem recebida.
    switch (message) {
        (...)
        case FIND_NODE:
            // find_node: só envia os nós mais próximos ao 'target'.
            debugf("Find node!\n");
            new_node(id, from, fromlen, 1);
            debugf("Sending closest nodes (%d).\n", want);
            send_closest_nodes(from, fromlen, tid, tid_len, target, want, 0, NULL, NULL, 0);
            break;
        case GET_PEERS:
            debugf("Get_peers!\n");
            new_node(id, from, fromlen, 1);
            if (id_cmp(info_hash, zeroes) == 0) {
                (...)
            }
            else {
                struct storage *st = find_storage(info_hash);
                unsigned char token[TOKEN_SIZE];
                make_token(from, 0, token);
                // Se conhecer nós para o hash de torrent dado, os indica na resposta...
                if (st && st->numpeers > 0) {
                    debugf("Sending found%s peers.\n",
                            from->sa_family == AF_INET6 ? " IPv6" : "");
                    send_closest_nodes(from, fromlen, tid, tid_len, info_hash, want,
                            from->sa_family, st, token, TOKEN_SIZE);
                }
                else {
                    // ..., senão procura nós próximos, como no comando find_node.
                    debugf("Sending nodes for get_peers.\n");
                    send_closest_nodes(from, fromlen, tid, tid_len, info_hash,
                        want, 0, NULL, token, TOKEN_SIZE);
                }
            }
            break;
        (...)
    }
    (...)
}