#ifndef QUEUE_H
#define QUEUE_H
#include "msg.h"

typedef struct queue_ QUEUE;

QUEUE *queue_create();
char queue_empty(QUEUE *q);
char queue_insert(QUEUE *q, MSG *msg);
MSG *queue_pop(QUEUE *q);
char queue_delete(QUEUE **q);

#endif
