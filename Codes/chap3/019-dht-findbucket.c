static int id_cmp(const unsigned char *restrict id1, const unsigned char *restrict id2) {
    /* Memcmp is guaranteed to perform an unsigned comparison. */
    return memcmp(id1, id2, 20);
}

static struct bucket * find_bucket(unsigned const char *id, int af) {
    struct bucket *b = af == AF_INET ? buckets : buckets6; // seletor de buckets IPv4/IPv6

    if (b == NULL) return NULL;
    while (1) {
        if (b->next == NULL) return b;
        if (id_cmp(id, b->next->first) < 0) // id tem valor binário menor que b->next->first,
            return b;                       // logo deve ficar neste bucket
        b = b->next;
    }
}