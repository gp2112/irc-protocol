#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <ncurses.h>
#include <ctype.h>
#include "irc.h"
#include "interface.h"
#include "controller.h"

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

MSG *msg_create(char *content, char *peer_ip, int port) {
    MSG *msg = (MSG *)malloc(sizeof(MSG));
    msg->peer_ip = (char *)malloc(20);
    msg->content = (char *)malloc(BUFF_SIZE);
    msg->peer_port = port;
    strncpy(msg->peer_ip, peer_ip, 20);
    strncpy(msg->content, content, BUFF_SIZE);

    return msg;
}

void *listenMsgs(void *args) {
    struct listen_args *largs = (struct listen_args*)args;
    int new_fd = largs->new_fd;
    QUEUE *msg_rcvd = largs->msg_rcvd;
    int *mutex = largs->mutex;
    struct sockaddr_in address;
    int len = sizeof(address);

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
        getpeername(new_fd, (struct sockaddr *)&address, &len);
        msg = msg_create(buffer, inet_ntoa(address.sin_addr), ntohs(address.sin_port)); 
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
    BUFFER *msgStr;

    int *mutex = largs->mutex, r;;

    MSG *msg;
    while (1) {
        
        *largs->kill = read_input(buffer, BUFF_SIZE);
        if (*largs->kill)
            return NULL;
        msg = msg_create(buffer_content(buffer), "Me", 0);
        msgStr = parseMsg( inet_ntoa(address.sin_addr), buffer_content(buffer));
        queue_insert(msg_rcvd, msg);

        r = send(new_fd, buffer_content(buffer), buffer_len(buffer), 0);
        //mvprintw(4,4,"%s", strerror(errno));
        if (r < 0)
            break;

        buffer_clear(buffer);
    }

    return NULL;
}



