int tr_fdSocketAccept(tr_session * s, int sockfd, tr_address * addr, tr_port * port) {
    int fd; unsigned int len;
    struct tr_fdInfo * gFd; struct sockaddr_storage sock;
    (...)
    len = sizeof(struct sockaddr_storage);

    // recebe dados do socket e armazena os dados lidos em 'sock'
    fd = accept(sockfd, (struct sockaddr *) &sock, &len);
    if (fd >= 0) {
        (...)
    }

    return fd;
}