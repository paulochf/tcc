static const char*
tr_metainfoParseImpl(const tr_session * session, tr_info * inf,
bool * hasInfoDict, int * infoDictLength, const tr_variant * meta) {
  // variáveis temporárias
  int64_t i; size_t len; const char * str; const uint8_t * raw;
  tr_variant * infoDict = NULL;

  // flags
  bool b, isMagnet = false;

  /* info_hash: urlencoded 20-byte SHA1 hash of the value of the info key from the
   * Metainfo file. (...) */
  b = tr_variantDictFindDict(meta, TR_KEY_info, &infoDict);
  if (hasInfoDict != NULL) *hasInfoDict = b;