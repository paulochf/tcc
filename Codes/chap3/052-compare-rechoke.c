static int compare_rechoke_info(const void * va, const void * vb) {
    const struct tr_rechoke_info * a = va, * b = vb;
    if (a->rechoke_state != b->rechoke_state)
        return a->rechoke_state - b->rechoke_state;
    return a->salt - b->salt;
}