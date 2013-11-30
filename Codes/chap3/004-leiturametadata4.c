  if (!tr_variantDictFindInt(infoDict, TR_KEY_private, &i)) // privacidade do torrent
    if (!tr_variantDictFindInt(meta, TR_KEY_private, &i)) i = 0;
  inf->isPrivate = i != 0;

  if (!isMagnet) { // quantidade de partes
    if (!tr_variantDictFindInt(infoDict, TR_KEY_piece_length, &i) || (i < 1))
      return "piece length";
    inf->pieceSize = i;
  }

  if (!isMagnet) { // hashes das partes
    if (!tr_variantDictFindRaw(infoDict, TR_KEY_pieces, &raw, &len) ||
        len % SHA_DIGEST_LENGTH) return "pieces";

    inf->pieceCount = len / SHA_DIGEST_LENGTH;
    inf->pieces = tr_new0(tr_piece, inf->pieceCount);
    for (i = 0; i < inf->pieceCount; i++)
      memcpy(inf->pieces[i].hash, &raw[i * SHA_DIGEST_LENGTH], SHA_DIGEST_LENGTH);
  }

  if (!isMagnet) { // lista de arquivos
    if ((str = parseFiles(inf, tr_variantDictFind(infoDict, TR_KEY_files),
                               tr_variantDictFind(infoDict, TR_KEY_length))))
      return str;

    if (!inf->fileCount || !inf->totalSize ||
        (uint64_t) inf->pieceCount != (inf->totalSize+inf->pieceSize-1)/inf->pieceSize)
      return "files";
  }

  if ((str = getannounce(inf, meta))) return str; // announce(s)
  (...)
  return NULL;
}