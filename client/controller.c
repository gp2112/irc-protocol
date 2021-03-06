#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <ncurses.h>
#include <ctype.h>
#include "irc.h"
#include "buffer.h"
#include "controller.h"
#include "commands.h"
#include "errors.h"


char *readUntilC (char *str, char C) {
        char *output = (char *) malloc(BUFF_SIZE);

        int i = 0;
        char c = str[i];

        while (c != '\n' && c != C) {
                output[i++] = c;
                c = str[i];
        }

        output[i] = '\0';

        output = (char *) realloc (output, i+1);

        // Mudando a posicao da str
        str = str+i;

        return output;
}

int parseMesage (char **b, char *input) {
        
        char *param, *buffer;
        int command;

        char cmd_err[] = "Invalid command."; 

        if (input[0] == '/') {
                // É um comando

                input++;

                if (!strncmp(input, "quit", 4)) {
                    command = QUIT;
                }
                else if (!strncmp(input, "ping", 4)) {
                    command = PING;
                }
                else if (!strncmp(input, "connect", 8)) {
                    command = CONNECT;
                    param = input+7;
                }
                else if (!strncmp(input, "join", 4)) {
                    command = JOIN;
                    param = input+5;
                }
                else if (!strncmp(input, "nickname", 8)) {
                    command = NICKNAME;
                    param = input+8;
                }
                else if (!strncmp(input, "kick", 4)) {
                    command = KICK;
                    param = input+4;
                }
                else if (!strncmp(input, "mute", 4)) {
                    command = MUTE;
                    param = input+4;
                }
                else if (!strncmp(input, "unmute", 6)) {
                    command = UNMUTE;
                    param = input+6;
                }
                else if (!strncmp(input, "whois", 5)) {
                    command = WHOIS;
                    param = input+5;
                }

                else {
                    command = CMD_ERROR;
                    param = cmd_err;
                }
        }

        else {
                command = PRVMSG;
                param = input;
        }

        int size = 2+sizeof(int)+strlen(param); 

        buffer = (char*)malloc(size);
        buffer[0] = MSGSTART;
        memcpy(1+buffer, &command, sizeof(int));
        memcpy(1+buffer+sizeof(int), param, strlen(param));
        buffer[size] = MSGEND;

        *b = buffer;

        return size;

}

char *recvParse(char *input, char *is_reply) {
    int code;
    char msg[130], *finalMsg = (char*)malloc(BUFF_SIZE);
        
    FILE *f;
    *is_reply = 1;

    if (*input == MSGSTART) {
        input++;
        char c = 0;
        int i = 0;
        
        *is_reply = 0;
    
        strcpy(finalMsg, input);

        return finalMsg;
    }

    memcpy(&code, input, sizeof(int)); 
    switch (code) {

        case 0:
            return NULL;

        case PONG:
            strcpy(msg, "PONG!");
            break;

        case ERR_INVITEONLYCHAN:
            strcpy(msg, ERR_INVITEONLYCHAN_SRT);
            break;
        case ERR_CHANNELISFULL:
            strcpy(msg, ERR_CHANNELISFULL_SRT);
            break;
        case ERR_NOSUCHNICK:
            strcpy(msg, ERR_NOSUCHNICK_SRT);
            break;
        case ERR_NOSUCHCHANNEL:
            strcpy(msg, ERR_NOSUCHCHANNEL_SRT);
            break;
        case ERR_NOTONCHANNEL:
        case ERR_USERNOTINCHANNEL:
            strcpy(msg, ERR_NOTONCHANNEL_SRT);
            break;
        case ERR_NOPRIVILEGES:
            strcpy(msg, ERR_NOPRIVILEGES_SRT);
            break;

    }
    strcpy(finalMsg, "Server: ");
    strcpy(finalMsg+8, msg);

    return finalMsg;
}


