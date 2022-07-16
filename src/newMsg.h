#ifndef NEW_MSG_H
#define NEW_MSG_H


typedef struct n_msg_ {
    // servername / user
    char *prefix;
    // Usar defines
    int command;
    char **params; 
} N_MSG;

#endif
