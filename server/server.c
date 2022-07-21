#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>


#include "message.h"
#include "errors.h"
#include "client.h"
#include "logger.h"
#include "flags.h"

#define BUFFERSIZE 255;

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
    
    logger_debug("Server init");

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

    logger_debug("Server init binding");

    r = bind(sockfd, (struct sockaddr*)(&address), sin_size);
    
    if (r < 0) {
        logger_error(strerror(errno));
        fprintf(stderr, strerror(errno));
        exit(1);
    }

    server->sockfd = sockfd;

    listen(sockfd, 10);

    return server;
}

CLIENT *server_find_client_by_hostname(SERVER *server, char *hostname) {
    char *dmsg[100] = "Finding hostname: "; 
    logger_debug(strncat(dmsg, hostname, 80));

    CLIENT_LIST *client_list = server->clients;
    while (client_list != NULL) {
        if (strcmp(client_list->client->host, hostname)==0)
            return client_list->client;
    }
    return NULL;
}

int server_connect_client(SERVER *server, char *hostname, int client_socket, int port) {

    CLIENT *client = client_create(NULL, hostname, client_socket, port);

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

typedef
struct {
    SERVER *server; 
    CLIENT *client;

} server_client_pair;


// thread function
void *server_listen_client(void *args) {
    server_client_pair *sc = (server_client_pair *)args;
    CLIENT *client = sc->client;
    SERVER *server = sc->server;

    char buffer[BUFFERSIZE];

    ssize_t size;

    while (!stop_server) {
        size = recv(client->socket, buffer, BUFFERSIZE 0); // may set MSG_WAITALL ?
        if (size == -1) {
            logger_error("Error when receiving a message.");
            sleep(5);
            continue;
        }
    }
    

}

void start_listening_thread(SERVER *server, char *addr, int cli_socket, int port) {

    pthread_t thread_id;

    client = client_create(NULL, cli_address, cli_socket, cli_port, &thread_id);
    server_client_pair *sc = (server_client_pair*)malloc(sizeof(server_client_pair));
    sc->server = server;
    sc->client = client;

    pthread_create(&thread_id, NULL, (void *)sc);
}

void server_run(SERVER *server) {
    
    logger_info("Server started with success!");
    logger_info("Listening for connections... (Press CTRL+C to leave)\n");
    
    struct sockaddr_in address;
    int sin_size = sizeof(struct sockaddr);
    int client_socket, cli_port;
    char msg_buffer[60] = "Received connection from ",
         msg_error[100] = "Connection failed: "
         *cli_address;

    CLIENT *client = NULL;

    // runs until ctrl+c signal (sigint)
    while (!stop_server) {
        client_socket = accept(server->sockfd, 
                                (struct sockaddr*)&address, &sin_size);
        
        if (client_socket == -1) {
            strncat(msg_error, strerror(errno), 78);
            logger_error(msg_error); 
            continue;
        }

        cli_address = inet_ntoa(address.sin_addr);
        cli_port = ntohs(address.sin_port);

        strncat(msg_buffer, cli_address, 30);
        logger_info(msg_buffer);

        
        
    }


};

int server_client_join_channel(SERVER *server, CLIENT *client, char *channel_name) {
    
    CHANNEL *channel = server_find_channel_by_name(server, channel_name);
    if (!channel)
        return server_create_channel(server, client, channel_name);

    channel_join(channel, client);

}
