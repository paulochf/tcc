int send_nodes_peers(const struct sockaddr *sa, int salen, const unsigned char *tid,
    int tid_len, const unsigned char *nodes, int nodes_len, const unsigned char *nodes6,
        int nodes6_len, int af, struct storage *st, const unsigned char *token, int token_len) {
    char buf[2048]; int i = 0, rc, j0, j, k, len;

    rc = snprintf(buf + i, 2048 - i, "d1:rd2:id20:");
    INC(i, rc, 2048);
    COPY(buf, i, myid, 20, 2048);
    if (nodes_len > 0) {
        rc = snprintf(buf + i, 2048 - i, "5:nodes%d:", nodes_len);
        INC(i, rc, 2048);
        COPY(buf, i, nodes, nodes_len, 2048);
    }
    (...)

    return dht_send(buf, i, 0, sa, salen);

    fail:
        errno = ENOSPC;
        return -1;
}