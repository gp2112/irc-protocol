#ifndef CLIENT_H
#define CLIENT_H

#include "params.h"
#include "queue.h"

typedef
struct client_ {
    char *nick;
    char *host;
    int socket;
    int port;
    pthread_t *conn_thread;
    char *current_channel;
    QUEUE *out_queue;
} CLIENT;

CLIENT *client_create(char *nick, char *hostname, int client_socket, 
                                                int port, pthread_t *conn);

#endif
