#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "channel.h"
#include "controller.h"
#include "commands.h"
#include "errors.h"
#include "logger.h"



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
    client->current_channel = channel_name(channel);
    return 0;
}

int cmd_nickname(SERVER *server, CLIENT *client, char *buffer) {
    char name[MAX_CLIENT_NAME]; int i=0;
    while (buffer[i] != MSGEND && i < MAX_CLIENT_NAME)
        name[i] = buffer[i++];

    name[i] = '\0';

    if (client->nick == NULL)
        client->nick = (char*)malloc(MAX_CLIENT_NAME*sizeof(char));

    strncpy(client->nick, name, MAX_CLIENT_NAME);


    return 0;

}

int cmd_kick(SERVER *server, CLIENT *client, char *buffer) {
    CHANNEL *
    client_channel = server_find_channel_by_name(server, client->current_channel);

    if (client_channel == NULL)
        return ERR_NOSUCHCHANNEL;

    if (client->host != channel_mod(client_channel)->host)
        return ERR_NOPRIVILEGES;

    char name[MAX_CLIENT_NAME]; int i=0;
    while (buffer[i] != MSGEND && i < MAX_CLIENT_NAME)
        name[i] = buffer[i++];

    name[i] = '\0';

    CLIENT *kicked_client = channel_find_client(client_channel, name);
    if (kicked_client == NULL)
        return ERR_USERNOTINCHANNEL;

    channel_kick(client_channel, client, kicked_client);
    return 0;
}

int cmd_mute(SERVER *server, CLIENT *client, char *buffer, char mute) {

    CHANNEL *
    client_channel = server_find_channel_by_name(server, client->current_channel);

    if (client != channel_mod(client_channel))
        return ERR_NOPRIVILEGES;

    char name[MAX_CLIENT_NAME]; int i=0;
    while (buffer[i] != MSGEND && i < MAX_CLIENT_NAME)
        name[i] = buffer[i++];

    name[i] = '\0';
    
    CLIENT *muted_client = channel_find_client(client_channel, name);
    if (muted_client == NULL)
        return ERR_USERNOTINCHANNEL;

    if (mute)
        channel_client_mute(client_channel, muted_client);
    else
        channel_client_unmute(client_channel, muted_client);

    return 0;
}

int cmd_whois(SERVER *server, CLIENT *client, char *buffer) {

    CHANNEL *
    client_channel = server_find_channel_by_name(server, client->current_channel);

    logger_info("%s", client->host, "#", client_channel, "WHOIS command");

    if (client != channel_mod(client_channel))
        return ERR_NOPRIVILEGES;

    char name[MAX_CLIENT_NAME]; int i=0;
    while (buffer[i] != MSGEND && i < MAX_CLIENT_NAME)
        name[i] = buffer[i++];

    name[i] = '\0';

    CLIENT *whois_client = channel_find_client(client_channel, name);
    if (whois_client == NULL)
        return ERR_USERNOTINCHANNEL;

    int r = send(client->socket, client->host, strlen(client->host)*sizeof(char), 0);
    if (r == -1) {
        logger_warning("%s %s", "Failed to reply ", client->host);
    }

    return -1;
}


int control_parse_msg(SERVER *server, CLIENT *client, char *buffer) {
    
    logger_debug("%s", "Ennterer control_parse_msg");

    if (*buffer != MSGSTART) {
        logger_warning("Invalid message: it doesnt start with MSGSTART!");
        return ERR_BADFORMAT;
    }
   
    buffer++;

    logger_debug("%s", "control parsing command");

    int command; memcpy(&command, buffer, sizeof(int)); 
    buffer+=sizeof(int);

    logger_debug("%s %d", "Command value: ", command);

    switch (command) {

        case JOIN:
            logger_debug("%s", "JOIN");
            return cmd_join(server, client, buffer);

        case NICKNAME:
            logger_debug("%s", "NICKNAME");
            return cmd_nickname(server, client, buffer);
    
        case PING:
            logger_debug("%s", "PING");
            return PONG;

        case KICK:
            logger_debug("%s", "KICK");
            return cmd_kick(server, client, buffer);

        case MUTE:
            logger_debug("%s", "MUTE");
            return cmd_mute(server, client, buffer, 1);
            break;

        case UNMUTE:
            logger_debug("%s", "UNMUTE");
            return cmd_mute(server, client, buffer, 0);
            break;

        case WHOIS:
            logger_debug("%s", "WHOIS");
            return cmd_whois(server, client, buffer);
            break;

        default:
            logger_debug("%s", "unknow cmd");
            return ERR_UNKNOWNCOMMAND; 



    }
    return 0;

}
