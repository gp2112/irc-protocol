/*
1.2 Clients

   A client is anything connecting to a server that is not another
   server.  Each client is distinguished from other clients by a unique
   nickname having a maximum length of nine (9) characters.  See the
   protocol grammar rules for what may and may not be used in a
   nickname.  In addition to the nickname, all servers must have the
   following information about all clients: the real name of the host
   that the client is running on, the username of the client on that
   host, and the server to which the client is connected.
*/
#include <stdlib.h>
#include "channel.h"

#define MAXCHANNELS 11
#define MAXCHATS 11

struct client_ {
    char *nick;
    char *host;
    int port;
}


CLIENT *client_create(char *nick, char *hostname, int port) {
    CLIENT *client = (CLIENT *)malloc(sizeof(CLIENT));
    client->nick = NULL;
    if (nick != NULL) {
        client->nick = (char*)malloc(strlen(nick)+1);
        strcpy(client->nick, nick);
    }

    client->hostname = (char*)malloc(strlen(hostname)+1);
    strcpy(client->hostname, hostname);

    client->port = port;

    return client;

}
