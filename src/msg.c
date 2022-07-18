#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <errno.h>
#include <ncurses.h>
#include <ctype.h>
#include "irc.h"
#include "buffer.h"
#include "msg.h"

BUFFER *createMsg (int command, char **params, int nParams) {
	char *msgString[BUFF_SIZE];

	char *msgStringInit = msgString;

	int msgLen;

	strcpy (msgString, prefix);
	msgString += strlen(prefix);

	msgString[] = ":";
	//Anda com o 'cursor'
	msgString++;

	strncpy(msgString, HOST, HOST_LEN);
	msgString += HOST_LEN;

	msgString[] = " ";
	msgString++;


	strcpy(msgString, &command, sizeof(int));
	msgString += sizeof(int);

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

	BUFFER *buf = buffer_create(BUFF_SIZE);
	
	for (int i=0; i<msgLen; i++) {
		buffer_insert(buf, msgStringInit[i]);
	}

	return buf;
}
