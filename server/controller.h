#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "server.h"
#include "client.h"

int control_parse_msg(SERVER *server, CLIENT *client, char *buffer);

#endif
