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
#include "interface.h"

#define SA struct sockaddr

int main(int argc, char *argv[]) {

    interface_init();

    BUFFER *buffer = buffer_create(BUFF_SIZE);

   
    char ip[20];
    int port;

    inter_getIpPort(buffer, ip, &port);

    struct sockaddr_in address;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket failed to create: %s\n", strerror(errno));
        return 1;
    }

    struct sockaddr_in servaddr;
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port = htons(port);

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) == -1) {
        printw("Fail to connect to server :(");
        return 1;
    }


    
    
    int mutex = 0, kill=0;

    QUEUE *msg_rcvd = queue_create();
    
    struct listen_args largs;
    largs.msg_rcvd = msg_rcvd;
    largs.new_fd = sockfd;
    largs.mutex = &mutex;
    largs.buffer = buffer;
    largs.kill = &kill;
   
    pthread_t t1, t2;
    pthread_create(&t1, NULL, listenMsgs, (void *)&largs);
    pthread_create(&t2, NULL, sendMsg, (void *)&largs);

    print_messages(msg_rcvd, buffer, &kill);

    interface_close(sockfd);
    queue_delete(&msg_rcvd);

    return 0;

}
