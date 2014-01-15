typedef struct tr_list {
    void *  data;
    struct tr_list  * next; // ponteiro para o próximo elemento
    struct tr_list  * prev; // ponteiro para o elemento anterior
} tr_list;