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
#include "errors.h"


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

	MSG *msg = (MSG *) malloc (sizeof(MSG));
	msg->nickname = nickname;
	msg->host = host;
	msg->command = command;
	msg->params = params;
	msg->nParams = nParams;

	return msg;
}

void handleRecivied (char *msg) {
	char output[BUFF_SIZE];

	int returnCode = *((int *)msg);
	switch (returnCode) {
		case PONG:
			strcpy(output, "\tSERVIDOR : PONG\0");
			break;
		case ERR_BANNEDFROMCHAN:
			strcpy(output, ERR_BANNEDFROMCHAN_STR);
			break;
		case ERR_INVITEONLYCHAN:
			strcpy(output, ERR_INVITEONLYCHAN_STR);
			break;
		case ERR_CHANNELISFULL:
			strcpy(output, ERR_CHANNELISFULL_STR);
			break;
		case ERR_BADCHANNELKEY:
			strcpy(output, ERR_BADCHANNELKEY_STR);
			break;
		case ERR_BADCHANMASK:
			strcpy(output, ERR_BADCHANMASK_STR);
			break;
		case ERR_NOSUCHNICK:
			strcpy(output, ERR_NOSUCHNICK_STR);
			break;
		case ERR_NOSUCHCHANNEL:
			strcpy(output, ERR_NOSUCHCHANNEL_STR);
			break;
		case ERR_TOOMANYCHANNELS:
			strcpy(output, ERR_TOOMANYCHANNELS_STR);
			break;
		case RPL_TOPIC:
			strcpy(output, RPL_TOPIC_STR);
			break;
		case ERR_NOTONCHANNEL:
			strcpy(output, ERR_NOTONCHANNEL_STR);
			break;
		case ERR_USERNOTINCHANNEL:
			strcpy(output, ERR_USERNOTINCHANNEL_STR);
			break;
		case ERR_CHANOPRIVSNEEDED:
			strcpy(output, ERR_CHANOPRIVSNEEDED_STR);
			break;
		case ERR_TOOMANYCHANNELS:
			strcpy(output, ERR_TOOMANYCHANNELS_STR);
			break;
		case ERR_YOUREBANNEDCREEP:
			strcpy(output, ERR_YOUREBANNEDCREEP_STR);
			break;
		case ERR_ALREADYREGISTRED:
			strcpy(output, ERR_ALREADYREGISTRED_STR);
			break;
		case ERR_NOTREGISTERED:
			strcpy(output, ERR_NOTREGISTERED_STR);
			break;
		case ERR_NOSUCHSERVER:
			strcpy(output, ERR_NOSUCHSERVER_STR);
			break;
		case ERR_NOTEXTTOSEND:
			strcpy(output, ERR_NOTEXTTOSEND_STR);
			break;
		case ERR_UNKNOWNCOMMAND:
			strcpy(output, ERR_UNKNOWNCOMMAND_STR);
			break;
		case ERR_NOPRIVILEGES:
			strcpy(output, ERR_NOPRIVILEGES_STR);
			break;
		
	}
    else if (msg->command == PONG) {
		strcpy(output, msg);
    }

	return output;
}