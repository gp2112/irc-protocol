#ifndef CLIENT_H
#define CLIENT_H

#include "params.h"
#include "queue.h"

typedef struct channel_ CHANNEL;

typedef struct client_ {
    char *nick;
    char *host;
    int socket;
    int port;
    pthread_t *conn_thread;
    char *current_channel;
} CLIENT;

#include "channel.h"

CLIENT *client_create(char *nick, char *hostname, int client_socket, 
                                                int port, pthread_t *conn);

void client_delete(CLIENT **client);

#endif
