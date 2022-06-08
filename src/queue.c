#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "queue.h"

#define BUFF_SIZE 1024

#define seterr(num)(errno=num)

typedef struct node_ NODE;

struct node_ {
    NODE *next;
    MSG *msg;
};

struct queue_ {
    NODE *head;
    NODE *end;
    char empty;
};

NODE *node_create(MSG *msg) {
    NODE *node = (NODE*)malloc(sizeof(NODE));
    if (node==NULL) {
        seterr(ENOMEM);
        return NULL;
    }
    node->next = NULL;
    node->msg = msg;
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

char queue_insert(QUEUE *q, MSG *msg) {
    if (q->empty) {
        q->head = node_create(msg);
        q->end = q->head;
        q->empty = 0;
        return 0;
    }
    
    if (q->head == q->end) {
        q->head->next = node_create(msg);
    }
    else q->end->next = node_create(msg);

    q->end = q->end->next;
    return 0;
}

MSG *queue_pop(QUEUE *q) {
    if (q->empty) {
        seterr(EADDRNOTAVAIL);
        return NULL;
    }

    NODE *n = q->head;
    
    q->head = q->head->next;

    if (q->head==NULL)
        q->empty = 1;
    MSG *msg = n->msg;
    free(n);
    return msg;
}

char queue_delete(QUEUE **q) {
    if (*q == NULL) {
        seterr(EADDRNOTAVAIL);
        return -1;
    }
    MSG *msg; 
    while (!(*q)->empty) {
        msg = queue_pop(*q);
        free(msg->content);
        free(msg);
    }
    
    free(*q);
    *q = NULL;
    return 0;
}
