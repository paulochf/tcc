int send_closest_nodes(const struct sockaddr *sa, int salen, const unsigned char *tid,
    int tid_len, const unsigned char *id, int want, int af, struct storage *st,
    const unsigned char *token, int token_len) {
    unsigned char nodes[8 * 26]; unsigned char nodes6[8 * 38];  // varíáveis...
    int numnodes = 0, numnodes6 = 0; struct bucket *b;          // ...temporárias

    (...)
    b = find_bucket(id, AF_INET); // Busca o bucket provável.
    if (b) {
        // Procura por nós no bucket encontrado e nos vizinhos anterior e/ou posterior,...
        // ...se possuir.
        numnodes = buffer_closest_nodes(nodes, numnodes, id, b);
        if (b->next) numnodes = buffer_closest_nodes(nodes, numnodes, id, b->next);
        b = previous_bucket(b);
        if (b) numnodes = buffer_closest_nodes(nodes, numnodes, id, b);
    }
    (...)

    // Envia os nós encontrados.
    return send_nodes_peers(sa, salen, tid, tid_len, nodes, numnodes * 26, nodes6,
        numnodes6 * 38, af, st, token, token_len);
}