#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>



void *sendMsg(void *args) {
    struct listen_args *largs = (struct listen_args*)args;

    int new_fd = largs->new_fd;
    QUEUE *msg_rcvd = largs->msg_rcvd;
    
    BUFFER *buffer = largs->buffer;

    int *mutex = largs->mutex, r;;

    MSG *msg;
    while (1) {
        
        *largs->kill = read_input(buffer, BUFF_SIZE);
        if (*largs->kill)
            return NULL;
        msg = msg_create(buffer_content(buffer), "Me", 0);
        queue_insert(msg_rcvd, msg);

        r = send(new_fd, buffer_content(buffer), buffer_len(buffer), 0);
        //mvprintw(4,4,"%s", strerror(errno));
        if (r < 0)
            break;

        buffer_clear(buffer);
    }

    return NULL;
}

