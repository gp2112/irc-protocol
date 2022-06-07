#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <ncurses.h>
#include <ctype.h>
#include "irc.h"

struct buff_ {
    char *content;
    int cursor;
    int length;
    int capacity;
    char end;
};

BUFFER *buffer_create(int cap) {
    BUFFER *buffer = (BUFFER *)malloc(sizeof(BUFFER));
    if (buffer==NULL) return NULL;
    buffer->content = (char*)malloc((cap+1)*sizeof(char));
    buffer->cursor = 0;
    buffer->length = 0;
    buffer->end = 0;
    buffer->capacity = cap;
    return buffer;
}

void buffer_start(BUFFER *b) {
    b->end = 0;
}

// removes element at cursor position
void buffer_del(BUFFER *b) {
    for (int i=b->cursor; i<b->length-1; i++)
        b->content[b->cursor] = b->content[b->cursor+1];
}
char buffer_ended(BUFFER *b) {
    return b->end;
}
char *buffer_content(BUFFER *b) {
    return b->content;
}

void buffer_insert(BUFFER *b, char c) {
    mvprintw(9,9,"%c", c);
    if (b->length == b->capacity)
        return ;
    mvprintw(9,9,"%c", c);
    b->content[b->cursor++] = c;
    b->length++;
}

void buffer_mv(BUFFER *b, int i) {
    if (b->cursor == 0 || b->cursor==b->length)
        return ;
    b->cursor += i;
}

void buffer_delete(BUFFER **buffer) {
    free((*buffer)->content);
    free(*buffer);
    *buffer = NULL;
}

void buffer_end(BUFFER *b) {
    b->content[b->length] = '\0';
    b->end = 1;
}

int buffer_len(BUFFER *b) {
    return b->length;
}

void buffer_clear(BUFFER *b) {
    b->length=0;
    b->cursor=0;
}

void buffer_print(BUFFER *b, int y, int x) {
    int x0, y0;
    getyx(stdscr, y0, x0);
    
    for (int i=0; i<b->length; i++)
        mvprintw(y, x++, "%c", b->content[i]);
    move(y0, x0);
    refresh();
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



void read_input(BUFFER *buffer, int n) {
    int c;
    int i=0;
    buffer_start(buffer);
    do {
        c = getch();
        getch();
        if(!(c & KEY_CODE_YES) && isprint(c)) {
            buffer_insert(buffer, (char) c);
            printw("%c", buffer->content[buffer->length-1]);
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
                buffer->cursor = 0;
                break;
            case KEY_END:
                buffer->cursor = buffer->length;
                break;
            case '\t':
                buffer_insert(buffer, '\t');
                break;
            case KEY_BACKSPACE:
            case 127:
            case 8:
                buffer_mv(buffer, -1);
                buffer_del(buffer);
                // Fall-through
                break;
            
        }
    } while (i<n && c != '\n');
    buffer_end(buffer);
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
        
        buffer = (BUFFER*)malloc(BUFF_SIZE);
        
        read_input(buffer, BUFF_SIZE);

        msg = msg_create(buffer->content, "Me");
        queue_insert(msg_rcvd, msg);

        send(new_fd, buffer->content, buffer_len(buffer), 0);

    }

    return NULL;
}



