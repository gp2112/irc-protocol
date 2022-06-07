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

    char buffer[BUFF_SIZE];
    
    
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
    largs.msg_rcvd = msg_rcvd;
    largs.msg_sent = msg_sent;
    largs.new_fd = new_fd;
    largs.mutex = &mutex;

    pthread_create(&t1, NULL, listenMsgs, (void *)&largs);
    pthread_create(&t2, NULL, sendMsg, (void *)&largs);

    MSG *msg;

    
    do {

        while (!queue_empty(msg_rcvd)) {
            while (mutex);
            mutex = 1;
            msg = queue_pop(msg_rcvd);
            writeMsg(msg->content, msg->peer_id, &msg_pos);
            mutex = 0;
        } 
         
        
        refresh();


    } while (strcmp(buffer, "/exit") != 0);

    endwin();
    queue_delete(&msg_rcvd);
    queue_delete(&msg_sent);
    close(sockfd);

    return 0;
}


