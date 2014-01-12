static void rechokeUploads(tr_swarm * s, const uint64_t now) {
    int i, size, unchokedInterested;
    const int peerCount = tr_ptrArraySize(&s->peers);
    tr_peer ** peers = (tr_peer**) tr_ptrArrayBase(&s->peers);
    struct ChokeData * choke = tr_new0(struct ChokeData, peerCount);
    const tr_session * session = s->manager->session;
    const int chokeAll = !tr_torrentIsPieceTransferAllowed(s->tor, TR_CLIENT_TO_PEER);
    const bool isMaxedOut = isBandwidthMaxedOut(&s->tor->bandwidth, now, TR_UP);

    // Confere se existe um peer que está unchoked de forma otimista
    // (unchoke otimista dura por 4 execuções da função)
    if (s->optimisticUnchokeTimeScaler > 0) s->optimisticUnchokeTimeScaler--;
    else s->optimistic = NULL;

    // Ordena os peers por preferência e rate
    for (i = 0, size = 0; i < peerCount; ++i) {
        tr_peer * peer = peers[i];
        tr_peerMsgs * msgs = PEER_MSGS(peer);
        (...)

        if (tr_peerIsSeed(peer)) {
            // Choke se peer é seeder total ou parcial (não tem o que baixar,...
            // ... só envia dados).
            tr_peerMsgsSetChoke(PEER_MSGS(peer), true);
        }
        else if (chokeAll) { // Choke todos se não está fazendo upload.
            tr_peerMsgsSetChoke(PEER_MSGS(peer), true);
        }
        else if (msgs != s->optimistic) { // Se o peer ainda não estiver no modo otimista.
            struct ChokeData * n = &choke[size++];
            n->msgs = msgs;
            n->isInterested = tr_peerMsgsIsPeerInterested(msgs);
            n->wasChoked = tr_peerMsgsIsPeerChoked(msgs);
            n->rate = getRate(s->tor, atom, now);
            n->salt = tr_cryptoWeakRandInt(INT_MAX); // valor aleatório entre 0 e INT_MAX
            n->isChoked = true;
        }
    }

    qsort(choke, size, sizeof(struct ChokeData), compareChoke);
    (...)