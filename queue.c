#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "queue.h"

#define BUFF_SIZE 1024

#define seterr(num)(errno=num)

typedef struct node_ NODE;

struct node_ {
    NODE *next;
    char msg[BUFF_SIZE];
};

struct queue_ {
    NODE *head;
    NODE *end;
    char empty;
};

NODE *node_create(char *msg) {
    NODE *node = (NODE*)malloc(sizeof(NODE));
    if (node==NULL) {
        seterr(ENOMEM);
        return NULL;
    }
    node->next = NULL;
    strncpy(node->msg, msg, BUFF_SIZE);
    return node;
}

QUEUE *queue_create() {
    QUEUE *q = (QUEUE*)malloc(sizeof(QUEUE));
    if (q==NULL) {
        seterr(ENOMEM);
        return NULL;
    }
    q->head = NULL;
    q->end = NULL;
    q->empty = 1;

    return q;
}

char queue_empty(QUEUE *q) {
    if (q == NULL) {
        seterr(EADDRNOTAVAIL);
        return -1;
    }
    return q->empty;
}

char queue_insert(QUEUE *q, char *msg) {
    if (q->empty) {
        q->head = node_create(msg);
        q->end = q->head;
        q->empty = 0;
        return 0;
    }
    
    q->end->next = node_create(msg);
    q->end = q->end->next;
    return -1;
}

char *queue_pop(QUEUE *q) {
    if (q->empty) {
        seterr(EADDRNOTAVAIL);
        return NULL;
    }

    NODE *n = q->head;
    
    q->head = q->head->next;

    free(n);
    return n->msg;
}

char queue_delete(QUEUE **q) {
    if (*q == NULL) {
        seterr(EADDRNOTAVAIL);
        return -1;
    }
    while (queue_pop(*q) != NULL);
    *q = NULL;
    return 0;
}
