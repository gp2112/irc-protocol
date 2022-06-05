#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include "peer.h"

#define PORT 9802

#define BUFF_SIZE 128

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

    char *buffer, nbuffer;
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    int client_fd = connect(sock, (struct sockaddr*)&serv_addr, 
                                        sizeof(serv_addr));

    if (client_fd < 0) {
        fprintf(stderr, strerror(errno));
        exit(1);
    } 

    int addrlen = 20;
    char *peer_addr = inet_ntoa(getpeername(sock, (struct sockaddr *)&serv_addr, sizeof()));

    int n, i;
    while (1) {

        printf("send: ");
        buffer = readline();
        n = strlen(buffer); 
        
        n = max(n, BUFF_SIZE);

        nbuffer = buffer;
        while ((i=send(sock, nbuffer, n, 0)) != n) {
            n -= i;
            nbuffer += n;
        }
        
        

        while (read(sock, buffer, BUFF_SIZE)>0)
            printf("%s, %s\n", peer_addr, buffer);

        free(buffer);
    
    }

    return 0;


}
