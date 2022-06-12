#ifndef CHANNEL_H
#define CHANNEL_H


typedef struct channel_ CHANNEL;


bool channel_is_banned(CHANNEL *ch, CLIENT *client);
bool channel_is_invited(CHANNEL *ch, CLIENT *client);
int channel_join(CHANNEL *ch, CLIENT *client, MSG *msg);
int channel_exit(CHANNEL *ch, CLIENT *client);
int channel_ban(CHANNEL *ch, CLIENT *client_by, CLIENT *client_to);
int channel_kick(CHANNEL *ch, CLIENT *client_by, CLIENT *client_to);


#endif
