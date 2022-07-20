#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

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
    int port;
    CLIENT_LIST *clients;
    CHANNEL_LIST *channels;
    int n_clients;
    int n_channels;
    int sockfd;
};

CLIENT_LIST *client_list_append(CLIENT_LIST *clients, CLIENT *client) {
    CLIENT_LIST *new_clients = (CLIENT_LIST*) malloc(sizeof(CLIENT_LIST));
    new_clients->next = clients;
    new_clients->client = client;

    return new_clients;
}

CHANNEL_LIST *channel_list_append(CHANNEL_LIST *channels, CHANNEL *channel) {
    CHANNEL_LIST *new_channels = (CHANNEL_LIST*) malloc(sizeof(CHANNEL_LIST));
    new_channels->next = channels;
    new_channels->channel = channel;

    return new_channels;
}

SERVER *server_init(char *hostname, int port) {
    SERVER *server = (SERVER *)malloc(sizeof(SERVER));
    server->hostname = (char*)malloc((strlen(hostname)+1)*sizeof(char));
    server->clients = NULL;
    server->channels = NULL;
    server->n_clients = 0; 
    server->n_channels=0;

    struct sockaddr_in address;
    
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    int r, sockfd = socket(PF_INET, SOCK_STREAM, 0),
           sin_size = sizeof(struct sockaddr);

    r = bind(sockfd, (struct sockaddr*)(&address), sin_size);
    
    if (r < 0) {
        fprintf(stderr, strerror(errno));
        exit(1);
    }

    server->sockfd = sockfd;

    listen(sockfd, 10);

    return server;
}

CLIENT *server_find_client_by_hostname(SERVER *server, char *hostname) {
    CLIENT_LIST *client_list = server->clients;
    while (client_list != NULL) {
        if (strcmp(client_list->client->host, hostname)==0)
            return client_list->client;
    }
    return NULL;
}

int server_connect_client(SERVER *server, char *hostname, int port) {

    CLIENT *client = client_create(NULL, hostname, port);

    server->clients = client_list_append(server->clients, client);
    
    

    return 0;
}

CHANNEL *server_find_channel_by_name(SERVER *server, char *name) {
    CHANNEL_LIST *channel_list = server->channels;
    while (channel_list != NULL) {
        if (strcmp(channel_list->channel->name, nname)==0)
            return client_list->client;
    }
    return NULL;
}


int server_create_channel(SERVER *server, CLIENT *client, char *name) {
    if (server == NULL) return -1;

    CHANNEL *channel = channel_create(client, name, 0, 100);
    server->channels = channel_list_append(server->channels, channel);
    
    return 0;
}


int server_client_join_channel(SERVER *server, CLIENT *client, char *channel_name) {
    
    CHANNEL *channel = server_find_channel_by_name(server, channel_name);
    if (!channel)
        return server_create_channel(server, client, channel_name);

    channel_join(channel, client);

}
