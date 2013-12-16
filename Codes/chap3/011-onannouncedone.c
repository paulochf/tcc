static void on_announce_done(const tr_announce_response * response, void * vdata) {
    struct announce_data * data = vdata;
    (...)
    tr_tier * tier = getTier(announcer, response->info_hash, data->tierId);
    const time_t now = tr_time();

    (...)

    if (tier != NULL) {
        tr_tracker * tracker;
        tier->lastAnnounceTime = now;
        tier->lastAnnounceTimedOut = response->did_timeout;
        tier->lastAnnounceSucceeded = tier->isAnnouncing = false;
        tier->manualAnnounceAllowedAt = now + tier->announceMinIntervalSec;

        (...)
        int i = scrape_fields = seeders = leechers = 0; const char * str;

        (...)

        if (response->seeders >= 0) {
            tracker->seederCount = seeders = response->seeders;
            (...)
        }
        if (response->leechers >= 0) {
            tracker->leecherCount = leechers = response->leechers;
            (...)
        }

        (...)

        if ((i = response->min_interval)) tier->announceMinIntervalSec = i;
        if ((i = response->interval)) tier->announceIntervalSec = i;

        if (response->pex_count > 0)
            publishPeersPex(tier, seeders, leechers, response->pex, response->pex_count);

        (...)
        tier->lastAnnounceSucceeded = true;
        (...)
    }
    (...)
}