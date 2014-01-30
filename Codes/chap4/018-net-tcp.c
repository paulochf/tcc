static int tr_netBindTCPImpl(const tr_address * addr, tr_port port, bool suppressMsgs,
    int * errOut)
{
    static const int domains[NUM_TR_AF_INET_TYPES] = { AF_INET, AF_INET6 };
    struct sockaddr_storage sock; int fd, addrlen, optval;
    (...)

    // criação do descritor de arquivo para socket para TCP (SOCK_STREAM)
    fd = socket(domains[addr->type], SOCK_STREAM, 0);
    (...)

    // parte principal do ./libtransmission/net.c:196
    // executada durante a função, e avalia o protocolo de conexão a ser usado (IPv4 ou IPv6)
    if (addr->type == TR_AF_INET) {
        // configura os dados do computador de destino para conexão por IPv4

        struct sockaddr_in  sock4;
        memset (&sock4, 0, sizeof (sock4));
        sock4.sin_family      = AF_INET;         // família de endereçamento
        sock4.sin_addr.s_addr = addr->addr.addr4.s_addr; // endereço IPv4
        sock4.sin_port        = htons(port);     // porta de conexão em "big endian"
        memcpy (sock, &sock4, sizeof (sock4));   // grava os dados em 'sock'
        addrlen =  sizeof (struct sockaddr_in);
    }
    else {
        // configura os dados do computador de destino para conexão por IPv6
        struct sockaddr_in6 sock6;
        memset (&sock6, 0, sizeof (sock6));
        sock6.sin6_family   = AF_INET6;          // família de endereçamento
        sock6.sin6_port     = htons(port);       // porta de conexão em "big endian"
        sock6.sin6_flowinfo = 0;                 // info de fluxo IPv6
        sock6.sin6_addr     = addr->addr.addr6;  // endereço IPv6
        memcpy (sock, &sock6, sizeof (sock6));   // grava os dados em 'sock'
        addrlen = sizeof (struct sockaddr_in6);
    }
    // fim da parte principal do ./libtransmission/net.c:196

    // Ligação do socket com a porta indicada
    if (bind(fd, (struct sockaddr *) &sock, addrlen)) {
        (...) // erro de ligação
    }

    (...)
    if (listen(fd, 128) == -1) { // aguarda conexões na porta indicada
        (...) // erro de escuta
    }

    return fd;
}