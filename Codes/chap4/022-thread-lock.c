/** @brief portability wrapper around OS-dependent thread mutexes */
struct tr_lock {
    int depth;
#ifdef WIN32
    CRITICAL_SECTION lock;
    DWORD lockThread;
#else
    pthread_mutex_t lock;
    pthread_t lockThread;
#endif
};

tr_lock* tr_lockNew(void) {
    tr_lock * l = tr_new0(tr_lock, 1);

#ifdef WIN32
    InitializeCriticalSection (&l->lock); /* supports recursion */
#else
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&l->lock, &attr);
#endif
    return l;
}

void tr_lockFree(tr_lock * l) {
#ifdef WIN32
    DeleteCriticalSection (&l->lock);
#else
    pthread_mutex_destroy(&l->lock);
#endif
    tr_free(l);
}

void tr_lockLock(tr_lock * l) {
#ifdef WIN32
    EnterCriticalSection (&l->lock);
#else
    pthread_mutex_lock(&l->lock);
#endif
    (...)
    l->lockThread = tr_getCurrentThread();
    ++l->depth;
}
