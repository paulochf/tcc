static int buffer_closest_nodes(unsigned char *nodes, int numnodes, const unsigned char *id,
    struct bucket *b) {
    struct node *n = b->nodes;
    while (n) {             // Itera sobre os nós...
        if (node_good(n))   // ... e, caso ele tenha feito contato recentemente, ...
            numnodes = insert_closest_node(nodes, numnodes, id, n); // ...o usa.
        n = n->next;
    }
    return numnodes; // Retorna a quantidade de nós encontrados.
}