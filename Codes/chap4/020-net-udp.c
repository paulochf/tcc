static void event_callback(int s, short type UNUSED, void *sv) {
    int rc; socklen_t fromlen; unsigned char buf[4096];
    struct sockaddr_storage from; tr_session *ss = sv;
    (...)

    // recebimento de dados do socket 's'
    rc = recvfrom(s, buf, 4096 - 1, 0, (struct sockaddr*) &from, &fromlen);
    (...)

    if (rc > 0) {
        (...) // uso do conteúdo de 'buf'
    }
}

void tr_udpInit(tr_session *ss) {
    (...)
    ss->udp_socket = socket(PF_INET, SOCK_DGRAM, 0); // configuração de socket para UDP
    (...)

    // quando recebe pacotes UDP, chama a função de retorno 'event_callback'
    ss->udp_event = event_new ( (...), event_callback, ss);
}