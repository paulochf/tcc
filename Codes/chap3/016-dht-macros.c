/* We could use a proper bencoding printer and parser, but the format of DHT messages
is fairly stylised, so this seemed simpler. */
#define CHECK(offset, delta, size)                      \
    if(delta < 0 || offset + delta > size) goto fail

#define INC(offset, delta, size)                        \
    CHECK(offset, delta, size);                         \
    offset += delta

#define COPY(buf, offset, src, delta, size)             \
    CHECK(offset, delta, size);                         \
    memcpy(buf + offset, src, delta);                   \
    offset += delta;

#define ADD_V(buf, offset, size)                        \
    if(have_v) {                                        \
        COPY(buf, offset, my_v, sizeof(my_v), size);    \
    }