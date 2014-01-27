/**
 * @brief Announce the given torrent on the local network
 *
 * @param[in] t Torrent to announce
 * @return Returns true on success
 *
 * Send a query for torrent t out to the LPD multicast group (or the LAN, for that
 * matter). A listening client on the same network might react by adding us to his
 * peer pool for torrent t.
 */
bool tr_lpdSendAnnounce(const tr_torrent* t) {
    size_t i;
    const char fmt[] = "BT-SEARCH * HTTP/1.1\r\n" // BT-SEARCH * HTTP/1.1
    "Host: %s:%u\r\n"                             // Host: 239.192.152.143:6771
    "Port: %u\r\n"                                // Port: <porta do Transmission>
    "Infohash: %s\r\n"                            // Infohash: <hash do torrent>
    "\r\n"
    "\r\n";

    // info_hash do torrent
    char hashString[lengthof(t->info.hashString)];

    // mensagem de announce multicast montada
    char query[lpd_maxDatagramLength + 1] = { };

    (...)

    /* prepare a zero-terminated announce message */
    tr_snprintf(query, lpd_maxDatagramLength + 1, fmt, (...), "239.192.152.143", 6771,
        lpd_port, hashString); // porta de conexão do Transmission e hash do torrent

    /* actually send the query out using [lpd_socket2] */
    {
        const int len = strlen(query);

        /* destination address info has already been set up in tr_lpdInit (),
         * so we refrain from preparing another sockaddr_in here */
        int res = sendto(lpd_socket2, query, len, 0,
                (const struct sockaddr*) &lpd_mcastAddr, sizeof lpd_mcastAddr);

        if (res != len) return false;
    }
    tr_logAddTorDbg(t, "LPD announce message away");
    return true;
}