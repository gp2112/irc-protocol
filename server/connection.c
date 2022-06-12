int connectServer(struct sockaddr_in *address, int sockfd, char *ip, int port) {

    
    struct sockaddr_in serv_addr;
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip, &serv_addr.sin_addr);

    int server_fd = connect(sockfd, (struct sockaddr*)&serv_addr, 
                                        sizeof(serv_addr));

    if (server_fd < 0) {
        fprintf(stderr, strerror(errno));
        return -1;
    } 

    return server_fd;
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

    return sockfd;
}

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

