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
#include <pthread.h>
#include <string.h>

#include "client.h"
#include "queue.h"



CLIENT *client_create(char *nick, char *hostname, int client_socket, 
                                                int port, pthread_t *conn) {

    CLIENT *client = (CLIENT *)malloc(sizeof(CLIENT));
    client->nick = NULL;
    if (nick != NULL) {
        client->nick = (char*)malloc(strlen(nick)+1);
        strcpy(client->nick, nick);
    }

    client->host = (char*)malloc(strlen(hostname)+1);
    strcpy(client->host, hostname);

    client->port = port;
    client->socket = client_socket;
    client->conn_thread = conn;
    client->current_channel = NULL;
    client->out_queue = queue_create();

    return client;

}
