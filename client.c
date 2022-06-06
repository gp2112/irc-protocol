#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <ncurses.h>

#define PORT 9802

#define BUFF_SIZE 128

#define max(a,b)(a>b ? a : b)


char *readline() {
    int i = 0;
    char c;

    char *buffer = (char*)malloc(BUFF_SIZE*sizeof(char));

    while ((c=getchar()) == '\n');

    do {

        buffer[i++] = c;

        if (i%(BUFF_SIZE+1)==0)
            buffer = realloc(buffer, (BUFF_SIZE * (int)(1+ i/BUFF_SIZE))*sizeof(char));

    } while ((c=getchar()) != '\n' && c != EOF );


    buffer[i] = '\0';

    return buffer;
}

void printBar(int y, int min_x, int max_x) {
    move(y, min_x);
    for (int x=min_x; x<max_x; x++) {
        printw("=");
    }
    printw("\n");
}

void writeMsg(char *msg, char *usr, int *y) {
    mvprintw((*y)++, 0, "%s: %s", usr, msg);
    refresh();
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

int main() {

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

    printBar(y-3, 0, x);
    printBar(y-1, 0, x);
    mvprintw(y-2, 0, "Message: ");

    
    int server_fd = connectServer(&address, sockfd, "127.0.0.1", PORT);

    char *nbuffer;
    
    int addrlen = sizeof(struct sockaddr);

    int n, i;
    do {
        //mvprintw(0, 0, "Hello world! Pressione alguma tecla (q para sair)!\n");

        //cmd = getch();

        move(y-2, 10);
        getnstr(buffer, BUFF_SIZE);
        move(y-2, 10);
        for (int i=10; i<x; i++) printw(" ");
        move(y-2, 10);
        
        send(sockfd, buffer, strlen(buffer)+1, 0);
        writeMsg(buffer, "Me", &msg_pos);

        rcv_size = recv(sockfd, buffer, BUFF_SIZE, 0);
        if (rcv_size==0)
            continue;
        if (rcv_size < 0) {
            printf("Error when receiving: %s\n", strerror(errno));
            break;
        }
        
        writeMsg(buffer, "127.0.0.1:9032", &msg_pos);


        refresh();


    } while (strcmp(buffer, "/exit") != 0); 

    getch();

    endwin();
    close(sockfd);

    return 0;


}
