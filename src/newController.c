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
#include "msg.h"
#include "commands.h"


char *readUntilC (char *str, char C) {
	char output = (char *) malloc(BUFF_SIZE);

	int i = 0;
	char c = str[i];

	while (c != "\n" && c != C) {
		output[i++] = c;
		c = str[i];
	}

	output[i] = "\0";

	output = (char *) realloc (output, i+1);

	// Mudando a posicao da str
	str = str+i;

	return output;
}

BUFFER *quitCommand (char *params) {
	char **paramsArr = (*char)[1];
	// param: quit message
	paramsArr[0] = readUntilC(params, " ");

	BUFFER *msg = cirarMsg (QUIT, paramsArr, 1);
    free(paramsArr[0]);
	return msg;
}

BUFFER *pingCommand (char *params) {
	char **paramsArr = (*char)[1];
	// param: server1 => server who is sending the PING
	paramsArr[0] = readUntilC(params, " ");

	BUFFER *msg = cirarMsg (PING, paramsArr, 1);
    free(paramsArr[0]);
	return msg;
}

BUFFER *connectCommand (char *params) {
	char *targetServer = readUntilC (params, " ");
	char *port = readUntilC (params, " ");

	char **paramsArr = (*char)[2];
	paramsArr[0] = targetServer;
	paramsArr[1] = port;

	BUFFER *msg = cirarMsg (CONNECT, paramsArr, 2);
    
    free(paramsArr[0]);
    free(paramsArr[1]);

	return msg;
}

BUFFER *prvmsgCommand (char *input) {
	BUFFER *msg = cirarMsg (PRVMSG, input, 1);
	return msg;
}

BUFFER *oneParamCommand (char *params) {
    char **paramsArr = (char *)[1];
    paramsArr[0] = readUntilC(params, " ");
	BUFFER *msg = cirarMsg (JOIN, paramsArr, 1);
    free(paramsArr[0]);
	return msg;
}



void parseMesage (char *peer_ip ,char *input) {
	BUFFER *msgFinal;
	char sourcePrefix[50];

	if (input[0] == "/") {
		// É um comando

		input++;

		if (strncmp(input, "quit", 4)) {
			msgFinal = quitCommand(input+4);
		}
		else if (strncmp(input, "ping", 4)) {
			msgFinal = pingCommand(input+4);
		}
		else if (strncmp(input, "connect", 8)) {
			msgFinal = connectCommand (input+8);
		}
		else if (strncmp(input, "join", 4)) {
			msgFinal = oneParamCommand (input+4);
		}
		else if (strncmp(input, "nickname", 8)) {
			msgFinal = oneParamCommand (input+8);
		}
		else if (strncmp(input, "kick", 4)) {
			msgFinal = oneParamCommand (input+8);
		}
		else if (strncmp(input, "mute", 4)) {
			msgFinal = oneParamCommand (input+4);
		}
		else if (strncmp(input, "unmute", 6)) {
			msgFinal = oneParamCommand (input+6);
		}
		else if (strncmp(input, "whois", 5)) {
			msgFinal = oneParamCommand (input+5);
		}
	}

	else {
		msg = prvmsgCommand(input);
	}

}

void reparseMessage (char *inString) {
	if (inString[] == MSGSTART) {
		inString++;
		char *nickname = readUntilC (inString, "!");
		char *host = readUntilC(inString, " ");
		int command = *((int *)inString);

		char (*params)[14]; 
		for(int i=0; i<14; i++) {
			// Consumir espaço (" ")
			inString++;
			params[i] = readUntilC(inString, " ");
			// Consumir ":"
			inString++;
			if (inString[] == 13 && *(++inString)==10) {
				// fim
				nParams = i+1;
				break;
			}
		}
	}
	/*
	Obtemos:
		nickname : char *
		host : char * (ip)
		command : int
		params : (char *)[14]
		nParams : int
	*/

	MSG *msg = (MSG *) malloc (sizeof(MSG));
	msg->nickname = nickname;
	msg->host = host;
	msg->command = command;
	msg->params = params;
	msg->nParams = nParams;

	return msg;
}