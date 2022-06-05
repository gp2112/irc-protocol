#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "peer.h"

#define PORT 9802
#define BUFF_SIZE 4098
#define LOCALHOST "127.0.0.1"



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


int main() {
    PEER *host = peerCreate(LOCALHOST, PORT),
        *peer = peerCreate(NULL, 11);
    
    int r,
        sockfd = socket(PF_INET, SOCK_STREAM, 0),
        sin_size = sizeof(struct sockaddr);

    r = bind(sockfd, (struct sockaddr*)peerGetSckAddr(host), sin_size);
    if (r==-1) {
        fprintf(stderr, strerror(errno));
        exit(1);
    }

    listen(sockfd, 10);

    printf("Listening...\n");
    int new_fd = accept(sockfd, (struct sockaddr*)peerGetSckAddr(peer), &sin_size);
    char *buffer;
    
    int rcv_size;

    while (1) {
        rcv_size = recv(new_fd, buffer, BUFF_SIZE, 0);
        if (rcv_size==0)
            break;
        if (rcv_size < 0) {
            printf(strerror(errno));
            break;
        }
        printf("\r%s:port  -  %s\n\n", peerGetIP(peer), buffer);

        printf("send: ");
        buffer = readline();

        send(new_fd, buffer, strlen(buffer)+1, 0);

        printf("\rme - %s\n\n", buffer);
        printf("send: ");
    }
    
    close(sockfd);
    

    return 0;
}
