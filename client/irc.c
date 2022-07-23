#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <ncurses.h>
#include <ctype.h>
#include "irc.h"
#include "interface.h"
#include "controller.h"




void *listenMsgs(void *args) {
    struct listen_args *largs = (struct listen_args*)args;
    int new_fd = largs->new_fd;
    
    int *mutex = largs->mutex;
    struct sockaddr_in address;
    int len = sizeof(address);

    int rcv_size;
    QUEUE *msg_rcvd = largs->msg_rcvd;
    char *buffer;

    const int rcvd = 222;

    char *msg, is_reply=0;
    while (1) {

        buffer = (char*)malloc(BUFF_SIZE);

        rcv_size = recv(new_fd, buffer, BUFF_SIZE, 0);
       

        if (rcv_size==0)
            break;
        if (rcv_size < 0) {
            printw("Error when receiving: %s\n", strerror(errno));
            break;
        }
       
        msg = recvParse(buffer, &is_reply);
        if (msg == NULL)
            continue;

        queue_insert(msg_rcvd, msg);

        if (!is_reply)
            send(new_fd, &rcvd, sizeof(int), 0);
    }
    return NULL;

}




void *sendMsg(void *args) {
    struct listen_args *largs = (struct listen_args*)args;

    int new_fd = largs->new_fd;
    QUEUE *msg_rcvd = largs->msg_rcvd;
    
    BUFFER *buffer = largs->buffer;
    BUFFER *msgStr;

    int *mutex = largs->mutex, r;

    char *buff, *input, is_reply, *msg;
    int buff_size;
    const int rcvd = RCVD;
    while (1) {
        
        *largs->kill = read_input(buffer, BUFF_SIZE);
        if (*largs->kill)
            return NULL;

        
        input = buffer_content(buffer);
        buff_size = parseMesage(&buff, input);
        
        
        r = send(new_fd, buff, buff_size, 0);

        //mvprintw(4,4,"%s", strerror(errno));
        if (r < 0)
            break;
        
        strcpy(buff, "Me: ");
        strcpy(buff+4, input);
        
        queue_insert(msg_rcvd, buff);

        /*while (*mutex);
        *mutex = 1;
        r = recv(new_fd, buff, BUFF_SIZE, 0);
        *mutex = 0;

        msg = recvParse(buff, &is_reply);
        queue_insert(msg_rcvd, msg);
        if (!is_reply)
            send(new_fd, &rcvd, sizeof(int), 0);*/


        buffer_clear(buffer);
    }

    return NULL;
}



