#include <stdlib.h>
#include <string.h>

#include "channel.h"
#include "errors.h"


typedef struct list_ LIST;
struct list_ {
    LIST *next;
    CLIENT *client;
    char is_muted;
};

struct channel_ {
    char *name;
    CLIENT *chanop;
    char inv_only;
    LIST *connected;
    LIST *pending; // invited users
    int users_limit;
    int n_users;
};

LIST *list_create(CLIENT *client) {
    LIST *l = (LIST *)malloc(sizeof(CLIENT));
    l->client = client;
    l->next = NULL;
    return l;
}

char channel_client_is_muted(CHANNEL *channel, CLIENT *client) {
    LIST *l = channel->connected;
    while (l != NULL) {
        if (l->client == client)
            return l->is_muted;

        l = l->next;
    }
    return -1;
}

void channel_client_mute(CHANNEL *channel, CLIENT *client) {
    LIST *l = channel->connected;
    while (l != NULL) {
        if (l->client == client) {
             l->is_muted = 1;
        }

        l = l->next;
    }

}

void channel_client_unmute(CHANNEL *channel, CLIENT *client) {
    LIST *l = channel->connected;
    while (l != NULL) {
        if (l->client == client) {
             l->is_muted = 0;
        }

        l = l->next;
    }

}

void channel_transmit_message(CHANNEL *channel, CLIENT *sender, char *text) {
    char buffer[BUFFERSIZE];
    LIST *l = channel->connected;

    char *nick = sender->nick;
    if (nick == NULL)
        nick = sender->host;

    strncpy(buffer, nick, MAX_CLIENT_NAME);
    char *next = buffer + strlen(nick);
    strncpy(next, " : ", 3); next += 3;
    strncpy(next, text, BUFFERSIZE-strlen(nick)-4);


    while (l != NULL) {
        if (l->client == sender) continue;
        
        queue_insert(l->client->out_queue, buffer);

    }


}

int channel_remove_client(LIST *l, CLIENT *client) {
    while (l->next != NULL && l->next->client != client) l = l->next;
    if (l->client != client && l->next == NULL) 
        return ERR_NOTONCHANNEL;

    LIST *tmp;

    if (l->next != NULL)
        tmp = l->next->next;

    l->next = tmp;
    return 0;
}

CLIENT *channel_find_client(CHANNEL *channel, char *name) {
    LIST *l = channel->connected;
    while (l != NULL) {
        if (strcmp(l->client->nick, name))
            return l->client;

        l = l->next;
    }
    return NULL;
}

CHANNEL *channel_create(CLIENT *client_op, char *name, char inv_only, int users_limit) {
    if (client_op == NULL) return NULL;

    CHANNEL *chan = (CHANNEL *)malloc(sizeof(CHANNEL));
    chan->name = (char*)malloc((strlen(name)+1)*sizeof(char));
    strcpy(chan->name, name);
    chan->inv_only = inv_only;
    chan->n_users = 1;
    chan->users_limit = users_limit;

    chan->connected = list_create(client_op);

    return chan;

}

CLIENT *channel_mod(CHANNEL *channel) {
    if (channel == NULL) return NULL;
    return channel->chanop;
}

char channel_is_invited(CHANNEL *ch, CLIENT *client) {
    LIST *l = ch->pending;
    while (l != NULL) {
        if (strcmp(l->client->nick, client->nick))
            return 1;

        l = l->next;
    }
    return 0;
    // return channel_find_client(ch->pending, client);
}

LIST *client_list_append(LIST *clients, CLIENT *client) {
    LIST *new_clients = (LIST*) malloc(sizeof(LIST));
    new_clients->next = clients;
    new_clients->client = client;

    return new_clients;
}

// join channel #name
//
int channel_join(CHANNEL *ch, CLIENT *client) {

    ch->connected = client_list_append(ch->connected, client);
    
    return 0;
}

int channel_exit(CHANNEL *ch, CLIENT *client) {
    // while (l->next != NULL && l->next->client != client) l = l->next;
    // if (l->client != client && l->next == NULL) 
    //     return ERR_NOTONCHANNEL;

    channel_remove_client(ch->connected, client);

    return 0;
}


int channel_kick(CHANNEL *ch, CLIENT *client_by, CLIENT *client_to) {
    channel_remove_client(ch->connected, client_to);
    return 0;
}

void channel_delete(CHANNEL **ch) {
    if (*ch == NULL) return ;

    free((*ch)->name);
    free(*ch);
    *ch = NULL;
}
