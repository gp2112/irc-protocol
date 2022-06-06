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

#define PORT 9802
#define BUFF_SIZE 4098
#define LOCALHOST "127.0.0.1"



void printBar(int y, int min_x, int max_x) {
    move(y, min_x);
    for (int x=min_x; x<max_x; x++) {
        printw("=");
    }
    printw("\n");
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

    //printf("Listening...\n");

    
    /*while (1) {
        rcv_size = recv(new_fd, buffer, BUFF_SIZE, 0);
        if (rcv_size==0)
            break;
        if (rcv_size < 0) {
            //printf("Error when receiving: %s\n", strerror(errno));
            break;
        }

        free(buffer);

        send(new_fd, buffer, strlen(buffer)+1, 0);

        printf("\rme - %s\n\n", buffer);
        printf("send: ");
    }*/
    

    return sockfd;
}

void writeMsg(char *msg, char *usr, int *y) {
    mvprintw((*y)++, 0, "%s: %s", usr, msg);
    refresh();
}

int main() {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    int x, y, msg_pos=0, rcv_size;

    struct sockaddr_in address;
    int sin_size = sizeof(struct sockaddr);

    getmaxyx(stdscr, y, x);
    printw("%d", y);

    char cmd = 0;

    char buffer[BUFF_SIZE];

    printBar(y-3, 0, x);
    printBar(y-1, 0, x);
    mvprintw(y-2, 0, "Message: ");

    
    int sockfd = serve(&address, "127.0.0.1", PORT);
    writeMsg("Waiting peer to connect...", "system", &msg_pos);

    int new_fd = accept(sockfd, (struct sockaddr*)&address, &sin_size);
    if (new_fd < 0) {
        fprintf(stderr, "Error when accepting: %s\n", strerror(errno));
        exit(1);
    }

    writeMsg("127.0.0.1 connect to this room!", "system", &msg_pos);

    do {
        //mvprintw(0, 0, "Hello world! Pressione alguma tecla (q para sair)!\n");

        //cmd = getch();

        
        rcv_size = recv(new_fd, buffer, BUFF_SIZE, 0);
        if (rcv_size==0)
            continue;
        if (rcv_size < 0) {
            //printf("Error when receiving: %s\n", strerror(errno));
            break;
        }
        
        writeMsg(buffer, "127.0.0.1:9340", &msg_pos);

        move(y-2, 10);
        getnstr(buffer, BUFF_SIZE);
        move(y-2, 10);
        for (int i=10; i<x; i++) printw(" ");
        move(y-2, 10);

        send(new_fd, buffer, strlen(buffer)+1, 0);

        writeMsg(buffer, "Me", &msg_pos);

        refresh();


    } while (strcmp(buffer, "/exit") != 0); 
    //getch();
    endwin();

    close(sockfd);

    return 0;
}


