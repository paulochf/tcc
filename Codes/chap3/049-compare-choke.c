static int compareChoke(const void * va, const void * vb) {
    const struct ChokeData * a = va;
    const struct ChokeData * b = vb;

    if (a->rate != b->rate) /* prefer higher overall speeds */
    return a->rate > b->rate ? -1 : 1;

    if (a->wasChoked != b->wasChoked) /* prefer unchoked */
    return a->wasChoked ? 1 : -1;

    if (a->salt != b->salt) /* random order */
    return a->salt - b->salt;

    return 0;
}