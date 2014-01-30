// Recebe uma função e um ponteiro para seus argumentos, e executa a função dada em...
// ...uma nova thread.
tr_thread * tr_threadNew(void (*func)(void *), void * arg) {
    tr_thread * t = tr_new0(tr_thread, 1);  // aloca espaço para uma estrutura 'tr_thread'

    t->func = func;
    t->arg = arg;

#ifdef WIN32   // Se o Transmission for utilizado em um  sistema operacional Windows...
{              // ...usa os comandos de thread específicos para ele.
    unsigned int id;
    t->thread_handle = (HANDLE) _beginthreadex (NULL, 0, &ThreadFunc, t, 0, &id);
    t->thread = (DWORD) id;
}
#else          // ...caso contrário, use pthread, que é padrão do Mac OS e Linux
    pthread_create(&t->thread, NULL, (void* (*)(void*)) ThreadFunc, t);
    pthread_detach(t->thread);
#endif
    return t;
}