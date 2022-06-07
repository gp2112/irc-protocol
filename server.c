#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <ncurses.h>

#include <netinet/in.h>
#include <assert.h>
#include <arpa/inet.h>

#include "irc.h"



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


void writeMsg(char *msg, char *usr, int *y) {
    int y0, x0;
    getyx(stdscr, y0, x0);
    int x = 0;
    while (*usr!='\0') {
        mvprintw(*y, x++, "%c", *usr);
        usr++;
    }
    mvprintw(*y, x++, ": ");
    while (*msg != '\0') {
        mvprintw(*y, ++x, "%c", *msg);
        msg++;
    }
    *y += 1;
    move(y0, x0);
}




int main() {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    intrflush(stdscr, 0);
    leaveok(stdscr, 1);
    int x, y, msg_pos=0, rcv_size;

    
    getmaxyx(stdscr, y, x);

    char cmd = 0;

    struct sockaddr_in address;
    int sin_size = sizeof(struct sockaddr);

    int sockfd = serve(&address, LOCALHOST, PORT);
    writeMsg("Waiting peer to connect...", "system", &msg_pos);

    int new_fd = accept(sockfd, (struct sockaddr*)&address, &sin_size);
    if (new_fd < 0) {
        fprintf(stderr, "Error when accepting: %s\n", strerror(errno));
        exit(1);
    }

    writeMsg("127.0.0.1 connect to this room!", "system", &msg_pos);

    QUEUE *msg_rcvd = queue_create(),
          *msg_sent = queue_create();

    pthread_t t1, t2;

    int mutex = 0;

    struct listen_args largs;

    BUFFER *buffer = buffer_create(BUFF_SIZE);

    largs.msg_rcvd = msg_rcvd;
    largs.msg_sent = msg_sent;
    largs.new_fd = new_fd;
    largs.mutex = &mutex;
    largs.buffer = buffer;

    pthread_create(&t1, NULL, listenMsgs, (void *)&largs);
    pthread_create(&t2, NULL, sendMsg, (void *)&largs);

    MSG *msg;

    
    do {

        while (!queue_empty(msg_rcvd)) {
            msg = queue_pop(msg_rcvd);
            writeMsg(msg->content, msg->peer_id, &msg_pos);
        } 
         
        
        printBar(y-3, 0, x);
        printBar(y-1, 0, x);
        mvprintw(y-2, 0, "Message: ");
        buffer_print(buffer, y-2, 10);

        if (buffer_ended(buffer))
            eraseChars(y-2, y-2, 10, x);
        refresh();


    } while (strcmp("ha", "/exit") != 0);

    endwin();
    queue_delete(&msg_rcvd);
    queue_delete(&msg_sent);
    close(sockfd);

    return 0;
}


