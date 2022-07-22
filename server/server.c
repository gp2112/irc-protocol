
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>


#include "channel.h"
#include "server.h"
#include "errors.h"
#include "logger.h"
#include "queue.h"
#include "controller.h"



typedef struct clients_ CLIENT_LIST;
typedef struct channels_ CHANNEL_LIST;

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
    char *stop_server;
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

SERVER *server_init(char *hostname, int port, char *stop_server) {
    
    logger_info("%s %s %s %d", "Starting server ", hostname, " on port", port);

    SERVER *server = (SERVER *)malloc(sizeof(SERVER));
    server->hostname = (char*)malloc((strlen(hostname)+1)*sizeof(char));
    server->clients = NULL;
    server->channels = NULL;
    server->n_clients = 0; 
    server->n_channels=0;
    server->stop_server = stop_server;

    struct sockaddr_in address;
    
    strcpy(server->hostname, hostname);

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    int r, sockfd = socket(PF_INET, SOCK_STREAM, 0),
           sin_size = sizeof(struct sockaddr);

    logger_debug("%s", "Server init binding");

    r = bind(sockfd, (struct sockaddr*)(&address), sin_size);
    
    if (r < 0) {
        logger_error("%s %s", "Error on bind: ", strerror(errno));
        exit(1);
    }

    server->sockfd = sockfd;

    listen(sockfd, 10);

    return server;
}

CLIENT *server_find_client_by_hostname(SERVER *server, char *hostname) {

    logger_debug("%s %s", "Finding client ", hostname);

    CLIENT_LIST *client_list = server->clients;
    while (client_list != NULL) {
        if (strcmp((client_list->client)->host, hostname)==0)
            return client_list->client;
        client_list = client_list->next;
    }
    return NULL;
}


CHANNEL *server_find_channel_by_name(SERVER *server, char *name) {
    CHANNEL_LIST *channel_list = server->channels;
    while (channel_list != NULL) {
        logger_debug("%s", "Running search channel");
        if (!strcmp(channel_name(channel_list->channel), name))
            return channel_list->channel;
        channel_list = channel_list->next;
    }
    return NULL;
}


int server_create_channel(SERVER *server, CLIENT *client, char *name) {
    if (server == NULL) {
        logger_error("%s", "PANIC: server is NULL");
        exit(1);
    }
    if (client == NULL) {
        logger_warning("%s", "PANIC: Client is NULL");
        exit(1);
    }

    logger_debug("%s %s", "Creating channel ", name);

    CHANNEL *channel = channel_create(client, name, 0, 100);
    strcpy(client->current_channel, name);
    server->channels = channel_list_append(server->channels, channel);
    
    return 0;
}

typedef
struct {
    SERVER *server; 
    CLIENT *client;

} server_client_pair;

void server_response(SERVER *server, CLIENT *client, int resp_code) {
    
    logger_debug("%s %s %s %d", "Replying to message from ", client->host, ":", client->port);

    if ( send(client->socket, &resp_code, sizeof(int), 0) == -1) {
        logger_warning("Error when replying to user.");
        return ;
    }
    
    logger_debug("%s %s %s %d", "Success on replying to ", client->host, ":", client->port);

}

// thread function
void *server_listen_client(void *args) {
    server_client_pair *sc = (server_client_pair *)args;
    CLIENT *client = sc->client;
    SERVER *server = sc->server;

    free(sc);

    int resp_code;

    char buffer[BUFFERSIZE];
    char *tmp;

    ssize_t size;

    while (! *server->stop_server) {
        /*
        while (!queue_empty(client->out_queue)) {
            tmp = queue_pop(client->out_queue);
            logger_debug("%s %s %s %s", "Sending ", tmp, " to ", client->host);

            
            if (size == -1) {
                logger_error("%s %s", "Error when sending message to client: ", strerror(errno));
                continue;
            }
            size = recv(client->socket, buffer, BUFFERSIZE, 0);
            if (size == -1) {
                logger_error("%s %s", "Error when receiving a message: ", strerror(errno));
                continue;
            }
            logger_debug("Message received by client");
        }*/
        
        size = recv(client->socket, buffer, BUFFERSIZE, 0);

        if (size == -1) {
            logger_error("%s %s", "Error when receiving a message: ", strerror(errno));
            sleep(5);
            continue;
        }
        if (size == 0) continue;

        

        logger_info("%s %s", "Message received from ", client->host);
        logger_debug("%s", "Bytes received:");
        for (int i=0; i<size; i++)
            printf("%d ", buffer[i]); printf("\n");
        //logger_debug("%s %d %s %d", "First byte: ", buffer[0], " Second byte: ", buffer[1]);
        
        logger_debug("%s", "Entering function for verifying commands");

        resp_code = control_parse_msg(server, client, buffer);
        if (resp_code == -1) continue; // does not need response: already replyed

        server_response(server, client, resp_code);
    }

    pthread_exit(NULL);
   
    return NULL;

}

int server_connect_client(SERVER *server, char *addr, int cli_socket, int port) {
    
    logger_debug("%s %s %s", "Creating and connecting ", addr," to server...");

    pthread_t thread_id;

    CLIENT *client = client_create(NULL, addr, cli_socket, port, &thread_id);
    server_client_pair *sc = (server_client_pair*)malloc(sizeof(server_client_pair));
    sc->server = server;
    sc->client = client;

    server->clients = client_list_append(server->clients, client);

    pthread_create(&thread_id, NULL, server_listen_client, (void *)sc);
    pthread_detach(thread_id);

    return 0;
}

void *async_server_run(void *s) {
    SERVER *server = (SERVER *)s;
    struct sockaddr_in address;
    int sin_size = sizeof(struct sockaddr);
    int client_socket, cli_port;
    char *cli_address;

    CLIENT *client = NULL;

    // runs until ctrl+c signal (sigint)

    while (! *server->stop_server) {
        client_socket = accept(server->sockfd, 
                                (struct sockaddr*)&address, &sin_size);
        
        if (client_socket == -1) {
            logger_error("%s %s", "Connection failed: ", strerror(errno));
            continue;
        }

        cli_address = inet_ntoa(address.sin_addr);
        cli_port = ntohs(address.sin_port);

        logger_info("%s %s %s %d", "Received connection from ", cli_address, ":", cli_port);

        server_connect_client(server, cli_address, client_socket, cli_port);

    }

    return NULL;
}

void server_run(SERVER *server) { 
    
    pthread_t thread_id;

    pthread_create(&thread_id, NULL, async_server_run, (void *)server);

    pthread_detach(thread_id);

    while (! *server->stop_server);
}

int server_client_join_channel(SERVER *server, CLIENT *client, char *channel_name) {
    logger_info("%s %s %s", client->host, " joining #", channel_name);

    CHANNEL *channel = server_find_channel_by_name(server, channel_name);
    if (!channel)
        return server_create_channel(server, client, channel_name);

    channel_join(channel, client);

}

void list_clients_delete(CLIENT_LIST **clients) {
    logger_debug("%s", "Deleting client list...");

    CLIENT_LIST *tmp = *clients, *tmp2 = NULL;

    while (tmp != NULL) {
        client_delete(&tmp->client);
        tmp2 = tmp->next;
        free(clients);
        tmp = tmp2;
    }

    *clients = NULL;
}

void list_channels_delete(CHANNEL_LIST **channels) {

    logger_debug("%s", "Deleting channel list...");

    CHANNEL_LIST *tmp = *channels, *tmp2 = NULL;

    while (tmp != NULL) {
        channel_delete(&tmp->channel);
        tmp2 = tmp->next;
        free(channels);
        tmp2 = tmp;
    }
    *channels = NULL;
}



void server_delete(SERVER **server) {
    logger_debug("%s", "Deleting server...");
    close((*server)->sockfd);

    if (*server==NULL) {
        logger_debug("%s", "Server already NULL");
        return ;
    }
    list_channels_delete(&(*server)->channels);
    list_clients_delete(&(*server)->clients);

    
}
