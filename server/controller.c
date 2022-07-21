#include <stdlib.h>
#include "controller.h"
#include "commands.h"
#include "errors.h"


int cmd_join(SERVER *server, CLIENT *client, char *buffer) {
    char name[MAX_CHANNEL_NAME]; int i=0;
    while (buffer[i] != MSGEND && i < MAX_CHANNEL_NAME)
        name[i] = buffer[i++];

    name[i] = '\0';
    CHANNEL *channel = server_find_channel_by_name(server, name);
    if (channel == NULL) {
        server_create_channel(server, client, name);
        return 0;
    }

    channel_join(channel, client);
    client->current_channel = channel;
    return 0;
}

int cmd_nickname(SERVER *server, CLIENT *client, char *buffer) {
    char name[MAX_CLIENT_NAME]; int i=0;
    while (buffer[i] != MSGEND && i < MAX_CLIENT_NAME)
        name[i] = buffer[i++];

    name[i] = '\0';

    if (client->name == NULL)
        client->name = (char*)malloc(MAX_CLIENT_NAME*sizeof(char));

    strncpy(client->name, name, MAX_CLIENT_NAME);


    return 0;

}

int cmd_kick(SERVER *server, CLIENT *client, char *buffer) {
    if (client != channel_mod(client->current_channel))
        return ERR_NOPRIVILEGES;

    char name[MAX_CLIENT_NAME]; int i=0;
    while (buffer[i] != MSGEND && i < MAX_CLIENT_NAME)
        name[i] = buffer[i++];

    name[i] = '\0';

    CLIENT *kicked_client = channel_find_client(client->current_channel, name);
    if (kicked_client == NULL)
        return ERR_USERNOTINCHANNEL;
    channel_remove_client(client->current_channel, kicked_client);
    return 0;
}

int cmd_mute(SERVER *server, CLIENT *client, char *buffer, char mute) {
    if (client != channel_mod(client->current_channel))
        return ERR_NOPRIVILEGES;

    char name[MAX_CLIENT_NAME]; int i=0;
    while (buffer[i] != MSGEND && i < MAX_CLIENT_NAME)
        name[i] = buffer[i++];

    name[i] = '\0';
    
    CLIENT *muted_client = channel_find_client(client->current_channel, name);
    if (muted_client == NULL)
        return ERR_USERNOTINCHANNEL;

    if (mute)
        channel_client_mute(client->current_channel, muted_client);
    else
        channel_client_unmute(client->current_channel, muted_client);

    return 0;
}

int cmd_whois(SERVER *server, CLIENT *client, char *buffer) {
    if (client != channel_mod(client->current_channel))
        return ERR_NOPRIVILEGES;

    char name[MAX_CLIENT_NAME]; int i=0;
    while (buffer[i] != MSGEND && i < MAX_CLIENT_NAME)
        name[i] = buffer[i++];

    name[i] = '\0';

    CLIENT *whois_client = channel_find_client(client->current_channel, name);
    if (whois_client === NULL)
        return ERR_USERNOTINCHANNEL;

    int r = send(client->socket, client->host, strlen(client->host)*sizeof(char));
    if (r == -1) {
        logger_error("Failed to reply client.");
    }

    return -1;
}


int control_parse_msg(SERVER *server, CLIENT *client, char *buffer) {
    int command; memcpy(&command, buffer, sizeof(int)); 
    buffer+=sizeof(int);

    switch (command) {

        case JOIN:
            return cmd_join(server, client, buffer);

        case NICKNAME:
            return cmd_nickname(server, client, buffer);
    
        case PING;
            return PONG;

        case KICK:
            return cmd_kick(server, client, buffer);

        case MUTE:
            return cmd_mute(server, client, buffer, 1);
            break;

        case UNMUTE:
            return cmd_mute(server, client, buffer, 0);
            break;

        case WHOIS:
            return cmd_whois(server, client, buffer);
            break;

        default:
            return ERR_UNKNOWNCOMMAND; 


    }

}

