#include <stdlib.h>
#include "message.h"
#include "errors.h"
#include "client.h"

typedef clients_ CLIENT_LIST;
typedef channels_ CHANNEL_LIST;

struct clients_ {
    CLIENT_LIST* next;
    CLIENT *client;
};

struct channels_ {
    CHANNEL_LIST *next;
    CHANNEL *channel;
};



struct server_ {
    char *hostname;
    CLIENT_LIST *clients;
    CHANNEL_LIST *channels;
    int n_clients;
    int n_channels;
};

CLIENT_LIST *client_list_append(CLIENT_LIST *clients, CLIENT *client) {
    CLIENT_LIST *new_clients = (CLIENT_LIST*) malloc(sizeof(CLIENT_LIST));
    new_clients->next = clients;
    new_clients->client = client;

    return new_clients;
}

int server_connect_client(SERVER *server, CLIENT *client) {
    server->clients = client_list_append(server->clients, client);
    
    

    return 0;
}

int server_create_channel(SERVER *server, CLIENT *client) {

}

int server_client_join_channel(SERVER *server, CLIENT *client, char *channel_name) {
    
}
