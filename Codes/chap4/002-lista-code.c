static tr_list * recycled_nodes = NULL;

static tr_list* node_alloc(void) {
    tr_list * ret;                  // ponteiro que aponta para a região alocada

    if (recycled_nodes == NULL) {   // Se não houver elementos reciclados,...
        ret = tr_new(tr_list, 1);   // ... aloque um novo.
    }
    else {                                  // Caso contrário, reutilize, reapontando...
        ret = recycled_nodes;               // ... os ponteiros para os elementos adjacentes.
        recycled_nodes = recycled_nodes->next;
    }

    *ret = TR_LIST_CLEAR;           // limpa campos do elemento
    return ret;                     // devolve o ponteiro para o elemento
}