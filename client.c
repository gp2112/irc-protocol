#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <ncurses.h>
#include <pthread.h>

#include "irc.h"

#define max(a,b)(a>b ? a : b)




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


int connectServer(struct sockaddr_in *address, int sockfd, char *ip, int port) {

    
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    int server_fd = connect(sockfd, (struct sockaddr*)&serv_addr, 
                                        sizeof(serv_addr));

    if (server_fd < 0) {
        fprintf(stderr, strerror(errno));
        return -1;
    } 

    return server_fd;
}

int main(int argc, char *argv[]) {

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    int x, y, msg_pos=0, rcv_size;

    struct sockaddr_in address;
    int sin_size = sizeof(struct sockaddr);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    getmaxyx(stdscr, y, x);

    char cmd = 0;

    char buffer[BUFF_SIZE];

        
    int server_fd = connectServer(&address, sockfd, argv[1], PORT);

    char *nbuffer;
    
    int addrlen = sizeof(struct sockaddr);

    QUEUE *msg_rcvd = queue_create(),
          *msg_sent = queue_create();
    pthread_t t1, t2;

    int mutex = 0;

    struct listen_args largs;
    largs.msg_rcvd = msg_rcvd;
    largs.msg_sent = msg_sent;
    largs.new_fd = sockfd;
    largs.mutex = &mutex;

    pthread_create(&t1, NULL, listenMsgs, (void *)&largs);
    pthread_create(&t2, NULL, sendMsg, (void *)&largs);

    int n, i;
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
