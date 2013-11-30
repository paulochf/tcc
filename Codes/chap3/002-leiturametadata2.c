  if (!b) { // Não possui a chave "info" no dicionário.
    // Será que não é um magnet link?
    if (tr_variantDictFindDict(meta, TR_KEY_magnet_info, &d)) {
        isMagnet = true;

        if (!tr_variantDictFindRaw(d, TR_KEY_info_hash, &raw, &len) ||
            len != SHA_DIGEST_LENGTH) // Se tiver a chave "info_hash" válida, a usa.
          return "info_hash";

        memcpy(inf->hash, raw, len);
        tr_sha1_to_hex(inf->hashString, inf->hash);
        (...)
    }
    else return "info"; // Não é magnet link e não possui a chave "info".
  }