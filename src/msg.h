#ifndef MSG_H
#define MSG_H

// typedef struct msg_ {
//     char *peer_ip;
//     int peer_port;
//     char *content;
// } MSG;

typedef struct n_msg_ {
    // servername / user
    char *prefix;

    char *nickname;
    char *host;

    // Usar defines
    int command;
    char **params; 
    int nParams;
} MSG;

BUFFER *createMsg (int , char **, int )

#endif
