#ifndef CLIENT_H
#define CLIENT_H

#define MAX_CLIENT_NAME 20

#include "queue.h"

typedef struct channel_ CHANNEL;

typedef struct client_ {
    char *nick;
    char *host;
    int socket;
    int port;
    pthread_t *conn_thread;
    CHANNEL *current_channel;
    QUEUE *out_queue;
} CLIENT;

#include "channel.h"

CLIENT *client_create(char *nick, char *hostname, int client_socket, 
                                                int port, pthread_t *conn);

#endif
