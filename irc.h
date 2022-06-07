#ifndef IRC_H
#define IRC_H


#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "queue.h"

#define BUFF_SIZE 4096
#define PORT 9802
#define LOCALHOST "127.0.0.1"

typedef struct buff_ BUFFER;

struct listen_args {
    QUEUE *msg_rcvd;
    QUEUE *msg_sent;
    BUFFER *buffer;
    int new_fd;
    int *mutex;
};

BUFFER *buffer_create(int cap);
void buffer_del(BUFFER *b);
char *buffer_content(BUFFER *b); 
void buffer_insert(BUFFER *b, char c);
void buffer_mv(BUFFER *b, int i);
void buffer_end(BUFFER *b);
int buffer_len(BUFFER *b);
void buffer_clear(BUFFER *b);
void buffer_print(BUFFER *b, int y, int x); 
char buffer_ended(BUFFER *b);
void buffer_start(BUFFER *b);

void *listenMsgs(void *args);
int serve(struct sockaddr_in *address, char *ip, int port);
void *sendMsg(void *args);

#endif
