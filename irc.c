#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <ncurses.h>
#include "irc.h"

typedef struct buff_ {
    char content[BUFF_SIZE];
    int cursor;
    int size;

} BUFFER;

BUFFER *create_buffer()

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


void eraseChars(int y0, int y, int x0, int x) {
    for (int i=y0; i<=y; i++)
        for (int j=x0; j<=x; j++)
            mvprintw(i, j, " ");
}

void printBar(int y, int min_x, int max_x) {
    move(y, min_x);
    for (int x=min_x; x<max_x; x++) {
        mvprintw(y, x, "=");
    }
}

void read_input(char *buffer, int x, int y, int n) {
    int x0, y0, i=0;
    getyx(stdscr, y0, x0);
    char c;
    do {
        c = getch();

        switch (c) {

        

            case KEY_BACKSPACE:
                x -= 2;
                mvprintw(y, x, " |");
                break
            case ERR: /* no key pressed */ break;
            case KEY_LEFT:  if(buf->cursor > 0)           { buf->cursor --; } break;
            case KEY_RIGHT: if(buf->cursor < buf->length) { buf->cursor ++; } break;
            case KEY_HOME:  buf->cursor = 0;           break;
            case KEY_END:   buf->cursor = buf->length; break;
            case '\t':
                add_char(buf, '\t');
                break;
            case KEY_BACKSPACE:
            case 127:
            case 8:
                if(buf->cursor <= 0) {
                    break;
                }
                buf->cursor --;
                // Fall-through
                       
            default:
                buffer[i] = c;
                mvprintw(y, x++, "%c|");

            
        }
    } while (i<n && c != '\n');

}

void *sendMsg(void *args) {
    struct listen_args *largs = (struct listen_args*)args;

    int new_fd = largs->new_fd;
    QUEUE *msg_rcvd = largs->msg_rcvd,
          *msg_sent = largs->msg_sent;

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
        
        
        move(y-2, 10);
        getnstr(buffer, BUFF_SIZE);
        
        while (*mutex);
        getyx(stdscr, y0, x0);
        *mutex = 1;
        eraseChars(y-2, y-2, 10, x);
        move(y0, x0);
        *mutex = 0;
        
        msg = msg_create(buffer, "Me");

        send(new_fd, buffer, strlen(buffer)+1, 0);

        queue_insert(msg_rcvd, msg);
        *mutex = 0;
    }

    return NULL;
}



