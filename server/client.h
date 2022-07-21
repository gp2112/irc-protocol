#ifndef CLIENT_H
#define CLIENT_H

typedef
struct client_ {
    char *nick;
    char *host;
    int socket;
    int port;
    pthread_t *conn_thread;
} CLIENT;

#endif
