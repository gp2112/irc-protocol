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


char *readUntilSpace (char *str, char C) {
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
	paramsArr[0] = readUntilSpace(params);

	BUFFER *msg = cirarMsg (QUIT, paramsArr, 1);

	return msg;
}

BUFFER *pingCommand (char *params) {
	char **paramsArr = (*char)[1];
	// param: server1 => server who is sending the PING
	paramsArr[0] = readUntilSpace(params);

	BUFFER *msg = cirarMsg (QUIT, paramsArr, 1);

	return msg;
}

BUFFER *connectCommand (char *params) {
	char *targetServer = readUntilSpace (params);
	char *port = readUntilSpace (params);

	char **paramsArr = (*char)[2];
	paramsArr[0] = targetServer;
	paramsArr[1] = port;

	BUFFER *msg = cirarMsg (CONNECT, paramsArr, 2);

	return msg;
}

BUFFER *prvmsgCommand (char *input) {
	BUFFER *msg = cirarMsg (PRVMSG, input, 1);
	return msg;
}


void parseMesage (char *peer_ip ,char *input) {
	BUFFER *msgFinal;
	char sourcePrefix[50];
	sourcePrefix[] = ":";
	sourcePrefix ++;

	if (nickname != NULL) {
		strcpy(sourcePrefix, nickname);
		sourcePrefix += strlen(nickname);
	}

	*(sourcePrefix++) = "!";
	strcpy(sourcePrefix, peer_ip);

	if (input[0] == "/") {
		// É um comando

		input++;

		if (strncmp(input, "quit", 4)) {
			// Comando QUIT
			msgFinal = quitCommand(input+4);
			
		}

		if (strncmp(input, "ping", 4)) {
			// Comando PING
			msgFinal = pingCommand(input+4);
			
		}
		if (strncmp(input, "connect", 8)) {
			// Comando CONNECT
			msgFinal = connectCommand (input+8);
		}
	}

	else {
		msg = prvmsgCommand(input);
	}

}

void reparseMessage (char *inString) {
	if (inString[] == ":") {
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
}