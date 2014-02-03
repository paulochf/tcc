struct node {
    // elemento de lista ligada de nós DHT
    (...)
    unsigned char id[20]; // ID do nó
    time_t time;          // horário da última mensagem recebida
    time_t reply_time;    // horário da última resposta recebida corretamente
    time_t pinged_time;   // horário da última requisição
    int pinged;           // quantidade de requisições feitas desde a última resposta
    struct node *next;    // ponteiro para o próximo elemento da lista ligada
};

struct bucket {
    // elemento de lista ligada de baldes
    (...)
    unsigned char first[20];        // ID do primeiro nó do balde
    int count;                      // quantidade de nós no balde
    int time;                       // horário da última resposta neste balde
    struct sockaddr_storage cached; // endereços de possíveis candidatos
    int cachedlen;                  // tamanho da lista de candidatos
    struct node *nodes;             // ponteiro para lista ligada de nós
    struct bucket *next;            // ponteiro para o próximo elemento da lista ligada
};