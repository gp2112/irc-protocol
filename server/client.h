#ifndef CLIENT_H
#define CLIENT_H

#define MAX_CLIENT_NAME 20

#include "queue.h"
#include "channel.h"

typedef
struct client_ {
    char *nick;
    char *host;
    int socket;
    int port;
    pthread_t *conn_thread;
    CHANNEL *current_channel;
    QUEUE *out_queue;
} CLIENT;

CLIENT *client_create(char *nick, char *hostname, int client_socket, 
                                                int port, pthread_t *conn);

#endif
