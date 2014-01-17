void tr_sha1(uint8_t * setme, const void * content1, int content1_len, ...) {
    va_list vl;
    SHA_CTX sha;
    const void * content;

    SHA1_Init(&sha);
    SHA1_Update(&sha, content1, content1_len);

    va_start(vl, content1_len);
    while ((content = va_arg(vl, const void*)))
        SHA1_Update(&sha, content, va_arg(vl, int));
    va_end(vl);

    SHA1_Final(setme, &sha);
}