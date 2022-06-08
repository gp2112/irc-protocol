#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <ncurses.h>
#include <ctype.h>
#include "irc.h"
#include "interface.h"

int connectServer(struct sockaddr_in *address, int sockfd, char *ip, int port) {

    
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip, &serv_addr.sin_addr);

    int server_fd = connect(sockfd, (struct sockaddr*)&serv_addr, 
                                        sizeof(serv_addr));

    if (server_fd < 0) {
        fprintf(stderr, strerror(errno));
        return -1;
    } 

    return server_fd;
}


int serve(struct sockaddr_in *address, char *ip, int port) {
    
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    address->sin_addr.s_addr = INADDR_ANY;

    int r, sockfd = socket(PF_INET, SOCK_STREAM, 0),
           sin_size = sizeof(struct sockaddr);

    r = bind(sockfd, (struct sockaddr*)(address), sin_size);

    if (r < 0) {
        fprintf(stderr, strerror(errno));
        exit(1);
    }

    listen(sockfd, 10);

    return sockfd;
}

MSG *msg_create(char *content, char *peer_id) {
    MSG *msg = (MSG *)malloc(sizeof(MSG));
    msg->peer_id = (char *)malloc(20);
    msg->content = (char *)malloc(BUFF_SIZE);
    strncpy(msg->peer_id, peer_id, 20);
    strncpy(msg->content, content, BUFF_SIZE);

    return msg;
}

void *listenMsgs(void *args) {
    struct listen_args *largs = (struct listen_args*)args;
    int new_fd = largs->new_fd;
    QUEUE *msg_rcvd = largs->msg_rcvd;
    int *mutex = largs->mutex;
    
    int rcv_size;
    
    char *buffer;

    MSG *msg;
    while (1) {
        msg = (MSG *)malloc(sizeof(MSG));

        buffer = (char*)malloc(BUFF_SIZE);
        rcv_size = recv(new_fd, buffer, BUFF_SIZE, 0);
        if (rcv_size==0)
            break;
        if (rcv_size < 0) {
            printw("Error when receiving: %s\n", strerror(errno));
            break;
        }
        if (strcmp(buffer, "received!")==0)
            continue;
        msg = msg_create(buffer, "127.0.0.1:1233"); 
        queue_insert(msg_rcvd, msg);

        //writeMsg(buffer, "127.0.0.1:9340", &msg_pos);

        send(new_fd, "received!", 10, 0);
    }
    return NULL;

}




void *sendMsg(void *args) {
    struct listen_args *largs = (struct listen_args*)args;

    int new_fd = largs->new_fd;
    QUEUE *msg_rcvd = largs->msg_rcvd;
    
    BUFFER *buffer = largs->buffer;

    int *mutex = largs->mutex, r;;

    MSG *msg;
    while (1) {
        
        *largs->kill = read_input(buffer, BUFF_SIZE);
        if (*largs->kill)
            return NULL;
        msg = msg_create(buffer_content(buffer), "Me");
        queue_insert(msg_rcvd, msg);

        r = send(new_fd, buffer_content(buffer), buffer_len(buffer), 0);
        //mvprintw(4,4,"%s", strerror(errno));
        if (r < 0)
            break;

        buffer_clear(buffer);
    }

    return NULL;
}



