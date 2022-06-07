#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <ncurses.h>
#include "irc.h"

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


void *listenMsgs(void *args) {
    struct listen_args *largs = (struct listen_args*)args;
    int new_fd = largs->new_fd;
    QUEUE *msg_queue = largs->msg_queue;
    int *mutex = largs->mutex;
    
    int rcv_size;
    
    char *buffer;

    MSG *msg;
    while (1) {
        msg = (MSG *)malloc(sizeof(MSG));

        buffer = (char*)malloc(BUFF_SIZE);

        rcv_size = recv(new_fd, buffer, BUFF_SIZE, 0);
        while (*mutex);
        *mutex = 1;
        if (rcv_size==0)
            continue;
        if (rcv_size < 0) {
            //printf("Error when receiving: %s\n", strerror(errno));
            break;
        }
        if (strcmp(buffer, "received!")==0)
            continue;
       
        msg->content = buffer;
        msg->peer_id = "127.0.0.1:1233";
        queue_insert(msg_queue, msg);

        //writeMsg(buffer, "127.0.0.1:9340", &msg_pos);

        send(new_fd, "received!", 10, 0);
        *mutex = 0;
    }
    return NULL;

}


void eraseChars(int y0, int y, int x0, int x) {
    for (int i=y0; i<=y; i++)
        for (int j=x0; j<=x; j++)
            mvprintw(i, j, " ");
}

void printBar(int y, int min_x, int max_x) {
    move(y, min_x);
    for (int x=min_x; x<max_x; x++) {
        printw("=");
    }
    printw("\n");
}
void *sendMsg(void *args) {
    struct listen_args *largs = (struct listen_args*)args;

    int new_fd = largs->new_fd;
    QUEUE *msg_queue = largs->msg_queue;
    int *mutex = largs->mutex;

    char *buffer, temp[BUFF_SIZE];
    int x, y, x0, y0;

    getmaxyx(stdscr, y, x);

    MSG *msg;

    while (1) {
        printBar(y-3, 0, x);
        printBar(y-1, 0, x);
        mvprintw(y-2, 0, "Message: ");

        buffer = (char*)malloc(BUFF_SIZE);
        msg = (MSG *)malloc(sizeof(MSG));

        getyx(stdscr, y0, x0);

        move(y-2, 10);
        getnstr(buffer, BUFF_SIZE);
        while (*mutex);
        *mutex = 1;

        eraseChars(y-2, y-2, 10, x);
        move(y0, x0);
        send(new_fd, buffer, strlen(buffer)+1, 0);

        msg->peer_id = "Me";
        msg->content = buffer;
        queue_insert(msg_queue, msg);
        *mutex = 0;
    }

    return NULL;
}



