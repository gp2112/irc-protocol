#ifndef INTERFACE_H
#define INTERFACE_H

#include "buffer.h"
#include "queue.h"

void interface_init();
int read_input(BUFFER *buffer, int n);
void inter_getIpPort(BUFFER *buffer, char *ip, int *port);
void print_messages(QUEUE *msg_rcvd, BUFFER *buffer, int *kill);
void interface_close();
void writeMsg(char *msg, char *usr, int port, int *y);

#endif
