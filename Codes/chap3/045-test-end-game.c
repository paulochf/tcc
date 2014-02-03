static bool testForEndgame(const tr_swarm * s) {
    // Modo de fim de jogo acontece quando, em um dado momento:
    // qtd de bytes requisitados $\geq$ qtd de bytes para completar o torrent
    return (s->requestCount * s->tor->blockSize) >= tr_cpLeftUntilDone(&s->tor->completion);
}