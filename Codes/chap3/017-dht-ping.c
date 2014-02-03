int send_ping(const struct sockaddr *sa, int salen, const unsigned char *tid, int tid_len) {
    int i = 0, rc; // variáveis temporárias
    char buf[512]; // buffer

    rc = snprintf(buf + i, 512 - i, "d1:ad2:id20:"); INC(i, rc, 512);

    // copia o ID do peer (que está em "myid") no buffer
    COPY(buf, i, myid, 20, 512);

    rc = snprintf(buf + i, 512 - i, "e1:q4:ping1:t%d:", tid_len); INC(i, rc, 512);

    // copia o ID de 4 bytes para a transação do Transmission no buffer
    COPY(buf, i, tid, tid_len, 512); ADD_V(buf, i, 512);

    rc = snprintf(buf + i, 512 - i, "1:y1:qe"); INC(i, rc, 512);
    return dht_send(buf, i, 0, sa, salen); // envia a requisição de ping
    (...)
}

int send_pong(const struct sockaddr *sa, int salen, const unsigned char *tid, int tid_len) {
    int i = 0, rc; // variáveis temporárias
    char buf[512]; // buffer

    rc = snprintf(buf + i, 512 - i, "d1:rd2:id20:"); INC(i, rc, 512);

    // copia o ID do peer (que está em "myid") no buffer
    COPY(buf, i, myid, 20, 512);

    rc = snprintf(buf + i, 512 - i, "e1:t%d:", tid_len); INC(i, rc, 512);

    // copia o ID de 4 bytes para a transação do Transmission no buffer
    COPY(buf, i, tid, tid_len, 512); ADD_V(buf, i, 512);

    rc = snprintf(buf + i, 512 - i, "1:y1:re"); INC(i, rc, 512);
    return dht_send(buf, i, 0, sa, salen); // envia a resposta a um ping
    (...)
}