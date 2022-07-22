#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "datetime.h"

#define ITOABUFFER 11;

struct datetime_ {

    int year;
    int month;
    int day;
    int hour;
    int minute;
    int seconds;

};

DATETIME *datetime_create(int year, int month, int day, int hour, int min, int sec) {
    DATETIME *dt = (DATETIME *)malloc(sizeof(DATETIME));
    if (dt == NULL) exit(1);
    dt->year = year;
    dt->month = month;
    dt->day = day;
    dt->hour = hour;
    dt->minute = min;
    dt->seconds = sec;

    return dt;
}

DATETIME *datetime_now() {
    
    int year, month, day, hour, min, sec;


    time_t timestamp = time(NULL);
    if (timestamp == -1) exit(1);

    struct tm *ptm = localtime(&timestamp);
    if (timestamp == -1) exit(1);

    year = 1900 + ptm->tm_year;
    month = 1 + ptm->tm_mon;
    day = ptm->tm_mday;
    hour = ptm->tm_hour;
    min = ptm->tm_min;
    sec = ptm->tm_sec;
    

    return datetime_create(year, month, day, hour, min, sec);
}

void itoa(int num, char *buffer, int n) {
    int size=0, d=num;
    
    
    for (int i=n-1; i>=0; i--) {
        buffer[i] = '0' + (d % 10);
        d = (int)(d/10);
    }

}

// with buffer overflow protection
char *datetime_isoformat(DATETIME *dt) {
    char *buffer = (char *)calloc(20, sizeof(char)), // AAAA-MM-DDTHH:mm:SS
         *tmp = buffer;

    itoa(dt->year, tmp, 4);  tmp+=4;
    tmp[0] = '-';
    itoa(dt->month, tmp+1, 2); tmp+=3;
    tmp[0] = '-';
    itoa(dt->day, tmp+1, 2);   tmp+=3;
    tmp[0] = ' '; tmp++;

    itoa(dt->hour, tmp, 2); tmp+=2;
    tmp[0] = ':';
    itoa(dt->minute, tmp+1, 2); tmp+=3;
    tmp[0] = ':';
    itoa(dt->seconds, tmp+1, 2); tmp+=3;
    tmp[0] = '\0';

    return buffer;

}

void datetime_delete(DATETIME **dt) {
    free(*dt);
    *dt=NULL;
}
