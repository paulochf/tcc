// ./libtransmission/metainfo.c:367
static const char*
tr_metainfoParseImpl(const tr_session * session, tr_info * inf,
bool * hasInfoDict, int * infoDictLength, const tr_variant * meta) {
  int64_t i; size_t len; const char * str; const uint8_t * raw; // variáveis temporárias
  tr_variant * infoDict = NULL;
  bool b, isMagnet = false;

  /* info_hash: urlencoded 20-byte SHA1 hash of the value of the info key from the
   * Metainfo file. (...) */
  b = tr_variantDictFindDict(meta, TR_KEY_info, &infoDict);
  if (hasInfoDict != NULL) *hasInfoDict = b;

  if (!b) { // Não possui a chave "info" no dicionário. Será que não é um magnet link?
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
  else {
    int len;
    char * bstr = tr_variantToStr(infoDict, TR_VARIANT_FMT_BENC, &len);
    tr_sha1(inf->hash, bstr, len, NULL);          // Calcula o hash SHA-1 do .torrent...
    tr_sha1_to_hex(inf->hashString, inf->hash);   // ...e converte de base2 para base16
    (...)
  }
  (...)
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