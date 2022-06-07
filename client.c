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
    noecho();
    curs_set(0);
    intrflush(stdscr, 0);
    leaveok(stdscr, 1);

    int x, y, msg_pos=0, rcv_size;

    struct sockaddr_in address;
    int sin_size = sizeof(struct sockaddr);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    
    char cmd = 0;

    int server_fd = connectServer(&address, sockfd, argv[1], PORT);

    char *nbuffer;
    
    int addrlen = sizeof(struct sockaddr);

    QUEUE *msg_rcvd = queue_create(),
          *msg_sent = queue_create();
    pthread_t t1, t2;

    int mutex = 0;

    BUFFER *buffer = buffer_create(BUFF_SIZE);
    struct listen_args largs;
    largs.msg_rcvd = msg_rcvd;
    largs.msg_sent = msg_sent;
    largs.new_fd = sockfd;
    largs.mutex = &mutex;
    largs.buffer = buffer;
    
    pthread_create(&t1, NULL, listenMsgs, (void *)&largs);
    pthread_create(&t2, NULL, sendMsg, (void *)&largs);

    int n, i;
    MSG *msg;

    do {
        getmaxyx(stdscr, y, x);
        eraseChars(0, y, 0, x);
        /*while (!queue_empty(msg_rcvd)) {
            msg = queue_pop(msg_rcvd);
            writeMsg(msg->content, msg->peer_id, &msg_pos);
        } */
        mvprintw(3, 30, "%d %d", x, y); 
        printBar(y-3, 0, x);
        printBar(y-1, 0, x);
        mvprintw(y-2, 0, "Message: ");

        buffer_print(buffer, y-2, 10);
        if (buffer_ended(buffer)) 
            eraseChars(y-2, y-2, 10, x);
        sleep(0.1);
        refresh();
        
    } while (strcmp("ha", "/exit") != 0);

    endwin();
    queue_delete(&msg_rcvd);
    queue_delete(&msg_sent);
    close(sockfd);

    return 0;


}
