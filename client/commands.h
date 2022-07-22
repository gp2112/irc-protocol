#ifndef COMMANDS_H
#define COMMANDS_H


#define MSGSTART 0xff
#define MSGEND 0xfe

#define KEEPALIVE 100

// receive

#define PING        0b0001000
#define NICKNAME    0b0001001
#define JOIN        0b0001010
#define KICK        0b0001011
#define MUTE        0b0001100
#define UNMUTE      0b0001101
#define WHOIS       0b0001110
#define TEXT        0b0001111
#define QUIT        0b0010000
#define CONNECT     0b0010001
#define PRVMSG      0b0010010
#define CMD_ERROR   0b0010011



// response

#define PONG ~PING




#endif
