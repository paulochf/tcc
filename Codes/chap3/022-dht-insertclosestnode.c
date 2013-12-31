static int insert_closest_node(unsigned char *nodes, int numnodes, const unsigned char *id,
    struct node *n) {
    int i, size; // variáveis temporárias

    (...)

    // Itera sobre os $k$ (neste caso, 8) nós mais próximos atuais (nodes) e verifica
    // se algum é o nó procurado ou se é mais próximo que algum nó conhecido (n).
    for (i = 0; i < numnodes; i++) {
        if (id_cmp(n->id, nodes + size * i) == 0) return numnodes;
        if (xorcmp(n->id, nodes + size * i, id) < 0) break;
    }

    // Se tiver iterado por todos os nós próximos atuais, eles são os mais próximos; ...
    if (i == 8) return numnodes;

    if (numnodes < 8) numnodes++; // ... caso contrário, o nó atual (n) entra para a lista.

    if (i < numnodes - 1) // Abre espaço para o nó atual (n) na lista.
        memmove(nodes + size * (i + 1), nodes + size * i, size * (numnodes - i - 1));

    (...)
    // Adiciona o nó atual (n) para a lista, convertendo os dados de endereço IP e porta.
    struct sockaddr_in *sin = (struct sockaddr_in*) &n->ss;
    memcpy(nodes + size * i, n->id, 20);
    memcpy(nodes + size * i + 20, &sin->sin_addr, 4);
    memcpy(nodes + size * i + 24, &sin->sin_port, 2);
    (...)

    return numnodes;
}