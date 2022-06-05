#include <netinet/in.h>
#include <assert.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

#include "peer.h"

#define ADDRSIZE 16


struct peer_ {

    char addr[ADDRSIZE];
    int port;
    struct sockaddr_in address;

    
};

PEER *peerCreate(char *addr, int port) {
    PEER *peer = (PEER *)malloc(sizeof(PEER));
    assert(peer!=NULL);

    peer->port = port;
    peer->address.sin_family = AF_INET;
    peer->address.sin_port = htons(port);

    

    if (addr != NULL) {
        strcpy(peer->addr, addr);
        inet_aton(addr, &peer->address.sin_addr);
    }
    else 
        peer->address.sin_addr.s_addr = INADDR_ANY;

    return peer;

}

char *peerGetIP(PEER *peer) {
    assert(peer!=NULL);
    return inet_ntoa(peer->address.sin_addr);
}


struct sockaddr_in *
peerGetSckAddr(PEER *peer) {

    assert(peer!=NULL);
    return &peer->address;
}

int peerGetPort(PEER *p) {
    assert(p!=NULL);
    return p->port;
}


