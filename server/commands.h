#ifndef COMMANDS_H
#define COMMANDS_H


#define MSGSTART 0x0f
#define MSGEND 0x0e

#define KEEPALIVE 100

// receive

#define PING        16
#define NICKNAME    17
#define JOIN        18
#define KICK        19
#define MUTE        20
#define UNMUTE      21
#define WHOIS       22
#define PRIVMSG     23
#define QUIT        24
#define RCVD        222

// response

#define PONG ~PING




#endif
