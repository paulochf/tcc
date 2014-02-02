  else {
    int len;
    char * bstr = tr_variantToStr(infoDict, TR_VARIANT_FMT_BENC, &len);
    tr_sha1(inf->hash, bstr, len, NULL);          // Calcula o hash SHA-1 do .torrent...
    tr_sha1_to_hex(inf->hashString, inf->hash);   // ...e converte de base2 para base16.
    (...)
  }