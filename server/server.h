#ifndef SERVER_H
#define SERVER_H

#include "client.h"
#include "channel.h"
#include "params.h"

typedef struct server_ SERVER;

SERVER *server_init(char *hostname, int port, char *stop_server);
CLIENT *server_find_client_by_hostname(SERVER *server, char *hostname);
CHANNEL *server_find_channel_by_name(SERVER *server, char *name);
int server_create_channel(SERVER *server, CLIENT *client, char *name);
void server_run(SERVER *server);
void server_delete(SERVER **server);

#endif
