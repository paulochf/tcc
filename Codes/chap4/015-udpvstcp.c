// Classifica dois trackers quaisquer.
static int filter_trackers_compare_func(const void * va, const void * vb) {
    const struct ann_tracker_info * a = va;
    const struct ann_tracker_info * b = vb;

    if (a->info.tier != b->info.tier) // ordem de ocorrência (ou seja, peers diferentes)
        return a->info.tier - b->info.tier;

    return -strcmp(a->scheme, b->scheme); // protocolo usado: UDP é preferido a HTTP (TCP)
}