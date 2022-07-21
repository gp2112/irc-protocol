#ifndef DATETIME_H
#define DATETIME_H

typedef struct datetime_ DATETIME;

DATETIME *datetime_create(int y, int mon, int d, int h, int min, int s);
DATETIME *datetime_now();
char *datetime_isoformat(DATETIME *dt);
void datetime_delete(DATETIME **dt);

#endif
