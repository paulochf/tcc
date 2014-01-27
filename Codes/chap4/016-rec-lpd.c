/**
 * @brief Process incoming unsolicited messages and add the peer to the announced
 * torrent if all checks are passed.
 *
 * @param[in,out] peer Adress information of the peer to add
 * @param[in] msg The announcement message to consider
 * @return Returns 0 if any input parameter or the announce was invalid, 1 if the peer
 * was successfully added, -1 if not; a non-null return value indicates a side-effect to
 * the peer in/out parameter.
 *
 * @note The port information gets added to the peer structure if tr_lpdConsiderAnnounce
 * is able to extract the necessary information from the announce message. That is, if
 * return != 0, the caller may retrieve the value from the passed structure.
 */
static int tr_lpdConsiderAnnounce(tr_pex* peer, const char* const msg) {
    enum {
        maxValueLen = 25,
        maxHashLen = lengthof(lpd_torStaticType->info.hashString)
    };
    (...)
    char value[maxValueLen] = { }, hashString[maxHashLen] = { };
    int res = 0, peerPort = 0;

    if (peer != NULL && msg != NULL) {
        tr_torrent* tor = NULL;

        // retira BT-SEARCH * HTTP/1.1 do cabeçalho da mensagem
        const char* params = lpd_extractHeader(msg, &ver);

        (...)

        // lê a porta enviada na mensagem e a carrega em 'value'
        if (lpd_extractParam(params, "Port", maxValueLen, value) == 0) return 0;

        // processa o número da porta e o guarda em 'peerPort'
        if (sscanf(value, "%d", &peerPort) != 1 || peerPort > (in_port_t) -1) return 0;

        peer->port = htons(peerPort);
        res = -1; /* signal caller side-effect to peer->port via return != 0 */

        // lê o hash do torrent anunciado
        if (lpd_extractParam(params, "Infohash", maxHashLen, hashString) == 0)
            return res;

        // Se o Transmission estiver participando do torrent anunciado, adiciona o...
        // ... endereço do remetente à lista de peers.
        tor = tr_torrentFindFromHashString(session, hashString);
        if (tr_isTorrent(tor) && tr_torrentAllowsLPD(tor)) {
            /* we found a suitable peer, add it to the torrent */
            tr_peerMgrAddPex(tor, TR_PEER_FROM_LPD, peer, -1);
            tr_logAddTorDbg(tor, "Learned %d local peer from LPD (%s:%u)", 1,
                    tr_address_to_string(&peer->addr), peerPort);
            (...)
            return 1;
        }
        (...)
    }

    return res;
}