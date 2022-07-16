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
#include "interface.h"



int main(int argc, char *argv[]) {
    int msg_pos = 1;

    char *ip = argc > 0 ? argv[1] : LOCALHOST; 
    int port = argc > 1 ? atoi(argv[2]) : PORT;

    interface_init();

    struct sockaddr_in address;
    int sin_size = sizeof(struct sockaddr);

    int sockfd = serve(&address, ip, port);
    writeMsg("Waiting peer to connect...", "system", 0, &msg_pos);

    int new_fd = accept(sockfd, (struct sockaddr*)&address, &sin_size);
    if (new_fd < 0) {
        fprintf(stderr, "Error when accepting: %s\n", strerror(errno));
        exit(1);
    }
    char *peer_addr = inet_ntoa(address.sin_addr); 
    msg_pos--;

    writeMsg("Someone connect to this room!", "system", 0, &msg_pos);

    QUEUE *msg_rcvd = queue_create();

   
    int mutex = 0, kill = 0;
    BUFFER *buffer = buffer_create(BUFF_SIZE);

    struct listen_args largs;
    largs.msg_rcvd = msg_rcvd;
    largs.new_fd = new_fd;
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

