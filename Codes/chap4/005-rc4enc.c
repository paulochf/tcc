void tr_cryptoEncrypt(tr_crypto * crypto, size_t buf_len, const void * buf_in, void * buf_out)
{
    RC4(&crypto->enc_key, buf_len, (const unsigned char*) buf_in, (unsigned char*) buf_out);
}