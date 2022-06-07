#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <ncurses.h>
#include <ctype.h>
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
    while (!*largs->kill) {
        msg = (MSG *)malloc(sizeof(MSG));

        buffer = (char*)malloc(BUFF_SIZE);

        rcv_size = recv(new_fd, buffer, BUFF_SIZE, 0);
        if (rcv_size==0)
            continue;
        if (rcv_size < 0) {
            //printf("Error when receiving: %s\n", strerror(errno));
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



int read_input(BUFFER *buffer, int n) {
    int c;
    int i=0;
    buffer_start(buffer);
    do {
        c = getch();
        if(!(c & KEY_CODE_YES) && isprint(c)) {
            buffer_insert(buffer, c);
            continue;
        }
        //mvprintw(9, 9, "%c", c);
        switch (c) {
        

            case ERR: break;
            case KEY_LEFT:  
                buffer_mv(buffer, -1);
                break;
            case KEY_RIGHT: 
                buffer_mv(buffer, 1);
                break;
            case KEY_HOME:
                buffer_setcursor(buffer, 0);
                break;
            case KEY_END:
                buffer_setcursor(buffer, -1);
                break;
            case 27:
                return 1;
            case '\t':
                buffer_insert(buffer, '\t');
                break;
            case KEY_BACKSPACE:
            case 127:
            case 8:
                buffer_del(buffer);
                // Fall-through
                break;
            
        }
    } while (i<n && c != '\n');
    buffer_end(buffer);
    return 0;
}

void *sendMsg(void *args) {
    struct listen_args *largs = (struct listen_args*)args;

    int new_fd = largs->new_fd;
    QUEUE *msg_rcvd = largs->msg_rcvd,
          *msg_sent = largs->msg_sent;
    
    BUFFER *buffer = largs->buffer;

    int *mutex = largs->mutex;

    MSG *msg;
    while (1) {
        
        *largs->kill = read_input(buffer, BUFF_SIZE);
        if (*largs->kill)
            return NULL;
        msg = msg_create(buffer_content(buffer), "Me");
        queue_insert(msg_rcvd, msg);

        send(new_fd, buffer_content(buffer), buffer_len(buffer), 0);
        buffer_clear(buffer);
    }

    return NULL;
}



