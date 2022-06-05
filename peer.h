#ifndef SOCKET_H
#define SOCKET_H

typedef struct peer_ PEER;


PEER *peerCreate(char *addr, int port);


struct sockaddr_in *
peerGetSckAddr(PEER *peer);

int peerGetPORT(PEER *p);

char *peerGetIP(PEER *p);



#endif
