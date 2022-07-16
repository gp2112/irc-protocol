#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <ncurses.h>
#include <ctype.h>
#include "irc.h"
#include "buffer.h"


char *readUntilSpace (char *str) {
	char output = (char *) malloc(BUFF_SIZE);

	int i = 0;
	char c = str[i];

	while (c != EOF && c != " ") {
		output[i++] = c;
		c = str[i];
	}

	output = (char *) realloc (output, i+1);

	// Mudando a posicao da str
	str = str+i;

	return output;
}

BUFFER *cirarMsg (int command, char **params, int nParams) {
	N_MSG *msg = (N_MSG *) malloc (sizeof(N_MSG));

	msg->command = command;
	msg->params = params;


	char *msgString = (char *) malloc (4096);

	char *msgStringInit = msgString;

	int msgLen;

	msgString[] = ":";
	//Anda com o 'cursor'
	msgString++;

	strncpy(msgString, HOST, HOST_LEN);
	msgString += HOST_LEN;

	msgString[] = " ";
	msgString++;


	strcpy(msgString, command, strlen(command));
	msgString += strlen(command);

	for (int i=0; i<nParams; i++) {
		msgString[] = " ";
		msgString++;

		strcpy(msgString, param[i], strlen(param[i]));
		msgString += strlen(param[i]);

		msgString[] = " ";
		msgString++;
		msgString[] = ":";
		msgString++;
	}


	msgString[] = 13;
	msgString++;
	msgString[] = 10;
	msgString++;

	msgLen = msgString - msgStringInit;

	BUFFER *buf = buffer_create(4096);
	
	for (int i=0; i<msgLen; i++) {
		buffer_insert(buf, msgStringInit[i]);
	}

	return buf;
}

N_MSG *connectCommand (char *params) {
	char *targetServer = readUntilSpace (params);
	char *port = readUntilSpace (params);

	char **params = (char **)malloc(2*sizeof(char *));
	params[0] = targetServer;
	params[1] = port;

	N_MSG *msg = cirarMsg (CONNECT, params);

	return msg;
}


void parseMesage (char *input) {
	BUFFER *msgFinal;

	if (input[0] == '/') {
		// É um comando

		if (strncmp(input, '/quit', 4)) {
			// Comando QUIT
			
		}

		if (strncmp(input, '/ping', 4)) {
			// Comando PING
			
		}
		if (strncmp(input, '/connect', 8)) {
			// Comando CONNECT
			msgFinal = connectCommand (*(input+8));
		}
	}

}