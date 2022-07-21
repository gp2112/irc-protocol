#ifndef SERVER_H
#define SERVER_H

#include "client.h"
#include "channel.h"
#include "params.h"

typedef struct server_ SERVER;

SERVER *server_init(char *hostname, int port);
CLIENT *server_find_client_by_hostname(SERVER *server, char *hostname);
CHANNEL *server_find_channel_by_name(SERVER *server, char *name);
void server_run(SERVER *server);

#endif
