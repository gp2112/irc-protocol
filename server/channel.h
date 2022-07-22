#ifndef CHANNEL_H
#define CHANNEL_H


#include "client.h"
#include "params.h"

char channel_is_banned(CHANNEL *ch, CLIENT *client);
char channel_is_invited(CHANNEL *ch, CLIENT *client);
int channel_join(CHANNEL *ch, CLIENT *client);
int channel_exit(CHANNEL *ch, CLIENT *client);
int channel_ban(CHANNEL *ch, CLIENT *client_by, CLIENT *client_to);
int channel_kick(CHANNEL *ch, CLIENT *client_by, CLIENT *client_to);
CHANNEL *channel_create(CLIENT *client_op, char *name, char inv_only, int users_limit);
CLIENT *channel_mod(CHANNEL *channel);
CLIENT *channel_find_client(CHANNEL *channel, char *name);
char channel_client_is_muted(CHANNEL *channel, CLIENT *client);
void channel_client_mute(CHANNEL *channel, CLIENT *client);
void channel_client_unmute(CHANNEL *channel, CLIENT *client);
char *channel_name(CHANNEL *channel);
void channel_delete(CHANNEL **ch);
void channel_transmit_message(CHANNEL *channel, CLIENT *sender, char *text);

#endif
