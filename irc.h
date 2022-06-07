#ifndef IRC_H
#define IRC_H


#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "queue.h"
#include "buffer.h"

#define PORT 9802
#define LOCALHOST "127.0.0.1"


struct listen_args {
    QUEUE *msg_rcvd;
    QUEUE *msg_sent;
    BUFFER *buffer;
    int new_fd;
    int *mutex;
    int *kill;
};


void *listenMsgs(void *args);
int serve(struct sockaddr_in *address, char *ip, int port);
void *sendMsg(void *args);

#endif
