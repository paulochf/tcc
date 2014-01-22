void tr_peerIoWriteBytes(tr_peerIo * io, const void * bytes, size_t byteCount,
    bool isPieceData)
{
    struct evbuffer_iovec iovec;
    (...)
    if (io->encryption_type == PEER_ENCRYPTION_RC4) // ENCRYPT2(Payload Stream)
        tr_cryptoEncrypt(&io->crypto, iovec.iov_len, bytes, iovec.iov_base);
    (...)
}