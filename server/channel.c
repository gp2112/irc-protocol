#include "errors.h"
#include <stdlib.h>
#include <string.h>

/*
 *
 * RFC 1459              Internet Relay Chat Protocol              May 1993


 A channel is a named group of one or more clients which will all
   receive messages addressed to that channel.  The channel is created
   implicitly when the first client joins it, and the channel ceases to
   exist when the last client leaves it.  While channel exists, any
   client can reference the channel using the name of the channel.

   Channels names are strings (beginning with a '&' or '#' character) of
   length up to 200 characters.  Apart from the the requirement that the
   first character being either '&' or '#'; the only restriction on a
   channel name is that it may not contain any spaces (' '), a control G
   (^G or ASCII 7), or a comma (',' which is used as a list item
   separator by the protocol).

   To create a new channel or become part of an existing channel, a user
   is required to JOIN the channel.  If the channel doesn't exist prior
   to joining, the channel is created and the creating user becomes a
   channel operator.  If the channel already exists, whether or not your
   request to JOIN that channel is honoured depends on the current modes
   of the channel. For example, if the channel is invite-only, (+i),
   then you may only join if invited.  As part of the protocol, a user
   may be a part of several channels at once, but a limit of ten (10)
   channels is recommended as being ample for both experienced and
   novice users.  See section 8.13 for more information on this.

   If the IRC network becomes disjoint because of a split between two
   servers, the channel on each side is only composed of those clients
   which are connected to servers on the respective sides of the split,
   possibly ceasing to exist on one side of the split.  When the split
   is healed, the connecting servers announce to each other who they
   think is in each channel and the mode of that channel.  If the
   channel exists on both sides, the JOINs and MODEs are interpreted in
   an inclusive manner so that both sides of the new connection will
   agree about which clients are in the channel and what modes the
   channel has.
 *
 * 1.3.1 Channel Operators

   The channel operator (also referred to as a "chop" or "chanop") on a
   given channel is considered to 'own' that channel.  In recognition of
   this status, channel operators are endowed with certain powers which
   enable them to keep control and some sort of sanity in their channel.
   As an owner of a channel, a channel operator is not required to have
   reasons for their actions, although if their actions are generally
   antisocial or otherwise abusive, it might be reasonable to ask an IRC
   operator to intervene, or for the usersjust leave and go elsewhere
   and form their own channel.

   The commands which may only be used by channel operators are:

        KICK    - Eject a client from the channel
        MODE    - Change the channel's mode
        INVITE  - Invite a client to an invite-only channel (mode +i)
        TOPIC   - Change the channel topic in a mode +t channel
 *
 * Oikarinen & Reed - https://datatracker.ietf.org/doc/html/rfc1459#section-1.1
 */

typedef struct list_ LIST;
struct list_ {
    LIST *next;
    CLIENT *client;
}

struct channel_ {
    char *name;
    CLIENT *chanop;
    bool inv_only;
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

int channel_remove_client(LIST *list, CLIENT *client) {
    while (l->next != NULL && l->next->client != client) l = l->next;
    if (l->client != client && l->next == NULL) 
        return ERR_NOTONCHANNEL;

    LIST *tmp;

    if (l->next != NULL)
        tmp = l->next->next;

    l->next = tmp;
    return 0;
}


CHANNEL *channel_create(CLIENT *client_op, char *name, bool inv_only, int users_limit) {
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


bool channel_is_invited(CHANNEL *ch, CLIENT *client) {
    return find_client(ch->invited, client);
}

CLIENT_LIST *client_list_append(LIST *clients, CLIENT *client) {
    LIST *new_clients = (LIST*) malloc(sizeof(LIST));
    new_clients->next = clients;
    new_clients->client = client;

    return new_clients;
}

// join channel #name
//
int channel_join(CHANNEL *ch, CLIENT *client) {

    ch->clients = client_list_append(ch->clients, client);
    
    return 0;
}

int channel_exit(CHANNEL *ch, CLIENT *client) {
    while (l->next != NULL && l->next->client != client) l = l->next;
    if (l->client != client && l->next == NULL) 
        return ERR_NOTONCHANNEL;

    remove_client(ch->connected, client);

    return 0;
}

int channel_ban(CHANNEL *ch, CLIENT *client_by, CLIENT *client_to) {
    if (ch->chanop != client_by)
        return ERR_CHANOPRIVSNEEDED;
    if (channel_is_banned(client_to))
        return 0;
    remove_client(ch->connected, client_to);
    LIST *tmp = ch->ban_list;
    ch->ban_list = list_create(client_to);
    ch->ban_list->next = tmp;
    return 0;
}

int channel_kick(CHANNEL *ch, CLIENT *client_by, CLIENT *client_to) {
    if (ch->chanop != client_by)
        return ERR_CHANOPRIVSNEEDED;
    remove_client(ch->connected, client_to);
    return 0;
}

void channel_delete(CHANNEL **ch) {
    if (*ch == NULL) return ;

    free((*ch)->name);
    free(*ch);
    *ch = NULL;
}
