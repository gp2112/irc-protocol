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


char *readUntilSpace (char *str) {
	char output = (char *) malloc(BUFF_SIZE);

	int i = 0;
	char c = str[i];

	while (c != "\n" && c != " ") {
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


void parseMesage (char *input) {
	BUFFER *msgFinal;

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

}