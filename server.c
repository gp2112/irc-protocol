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
    mvprintw((*y)++, 0, "%s: %s", usr, msg);
    move(y0, x0);
}




int main() {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);

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

    QUEUE *msg_queue = queue_create();
    pthread_t t1, t2;

    int mutex = 0;

    struct listen_args largs;
    largs.msg_queue = msg_queue;
    largs.new_fd = new_fd;
    largs.mutex = &mutex;

    pthread_create(&t1, NULL, listenMsgs, (void *)&largs);
    pthread_create(&t2, NULL, sendMsg, (void *)&largs);

    MSG *msg;

    
    do {

        while (!queue_empty(msg_queue)) {
            msg = queue_pop(msg_queue);
            writeMsg(msg->content, msg->peer_id, &msg_pos);
        } 
        
        
        refresh();


    } while (strcmp(buffer, "/exit") != 0);

    endwin();
    queue_delete(&msg_queue);
    close(sockfd);

    return 0;
}


