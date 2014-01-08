static bool buildHandshakeMessage(tr_handshake * handshake, uint8_t * buf) {
    const unsigned char * peer_id = NULL;
    const uint8_t * torrentHash;
    tr_torrent * tor;

    // Cria ou recupera o ID do cliente e carrega o ID do torrent.
    if ((torrentHash = tr_cryptoGetTorrentHash(handshake->crypto)))
        if ((tor = tr_torrentFindFromHash(handshake->session, torrentHash)))
            peer_id = tr_torrentGetPeerId(tor);

    (...)
    uint8_t * str = buf;

    // Começa a montar a string str com a mensagem de handshake
    // OBS: \023 = valor octal para o correspondente binário de 19
    // (tamanho da string "BitTorrent protocol")
    memcpy(str, "\023BitTorrent protocol", 20); str += 20;

    memset(str, 0, HANDSHAKE_FLAGS_LEN); // Zera 8 bytes (bytes reservados).

    HANDSHAKE_SET_LTEP(str);     // Habilita LTEP e Fast Peers, setando...
    HANDSHAKE_SET_FASTEXT(str);  // ... os bits correspondentes.

    /* Note that this doesn't depend on whether the torrent is private.
     * We don't accept DHT peers for a private torrent,
     * but we participate in the DHT regardless. */
    if (tr_dhtEnabled(handshake->session))
        HANDSHAKE_SET_DHT(str); // Habilita DHT, se o usuário tiver escolhido.

    str += HANDSHAKE_FLAGS_LEN;

    // Adiciona o hash identificador do torrent e o id do cliente
    memcpy(str, torrentHash, SHA_DIGEST_LENGTH); str += SHA_DIGEST_LENGTH;
    memcpy(str, peer_id, PEER_ID_LEN); str += PEER_ID_LEN;

    (...)
    return true;
}