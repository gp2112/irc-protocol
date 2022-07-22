#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "logger.h"
#include "datetime.h"

int logger_mode = ALL;

#define is_logmode(mode)((logger_mode & mode) == mode)


void logger_debug(char *fmt, ...) {
    if (!is_logmode(DEBUG)) return ;

    DATETIME *dt = datetime_now();
    char *dt_string = datetime_isoformat(dt);

    

    va_list ap;
    int d;
    char c;
    char *s;

    printf("\033[1;32m[ %s ] DEBUG: ", dt_string);
 
    va_start(ap, fmt);
    while (*fmt)
        switch (*fmt++) {
        case 's':              /* string */
            s = va_arg(ap, char *);
            printf("%s", s);
            break;
        case 'd':              /* int */
            d = va_arg(ap, int);
            printf("%d", d);
            break;
        case 'c':              /* char */
            /* need a cast here since va_arg only
               takes fully promoted types */
            c = (char) va_arg(ap, int);
            printf("%c", c);
            break;
        }
    printf("\033[0m\n");
    va_end(ap);


    free(dt_string);
    datetime_delete(&dt);
}

void logger_error(char *fmt, ...) {
    if (!is_logmode(ERROR)) return ;

    DATETIME *dt = datetime_now();
    char *dt_string = datetime_isoformat(dt);

    va_list ap;
    int d;
    char c;
    char *s;

    printf("\033[1;31m[ %s ] ", dt_string);
 
    va_start(ap, fmt);
    while (*fmt)
        switch (*fmt++) {
        case 's':              /* string */
            s = va_arg(ap, char *);
            printf("%s", s);
            break;
        case 'd':              /* int */
            d = va_arg(ap, int);
            printf("%d", d);
            break;
        case 'c':              /* char */
            /* need a cast here since va_arg only
               takes fully promoted types */
            c = (char) va_arg(ap, int);
            printf("%c", c);
            break;
        }
    printf("\033[0m\n");
    va_end(ap);


    free(dt_string);
    datetime_delete(&dt);
}

void logger_warning(char *fmt, ...) {
    if (!is_logmode(WARN)) return ;

    DATETIME *dt = datetime_now();
    char *dt_string = datetime_isoformat(dt);

    va_list ap;
    int d;
    char c;
    char *s;

    printf("\033[1;33m[ %s ] ", dt_string);
 
    va_start(ap, fmt);
    while (*fmt)
        switch (*fmt++) {
        case 's':              /* string */
            s = va_arg(ap, char *);
            printf("%s", s);
            break;
        case 'd':              /* int */
            d = va_arg(ap, int);
            printf("%d", d);
            break;
        case 'c':              /* char */
            /* need a cast here since va_arg only
               takes fully promoted types */
            c = (char) va_arg(ap, int);
            printf("%c", c);
            break;
        }
    printf("\033[0m\n");
    va_end(ap);

    free(dt_string);
    datetime_delete(&dt);
}

void logger_info(char *fmt, ...) {
    if (!is_logmode(INFO)) return ;

    DATETIME *dt = datetime_now();
    char *dt_string = datetime_isoformat(dt);

    va_list ap;
    int d;
    char c;
    char *s;

    printf("\033[1;37m[ %s ] ", dt_string);
 
    va_start(ap, fmt);
    while (*fmt)
        switch (*fmt++) {
        case 's':              /* string */
            s = va_arg(ap, char *);
            printf("%s ", s);
            break;
        case 'd':              /* int */
            d = va_arg(ap, int);
            printf("%d ", d);
            break;
        case 'c':              /* char */
            c = (char) va_arg(ap, int);
            printf("%c ", c);
            break;
        }
    printf("\033[0m\n");
    va_end(ap);

    free(dt_string);
    datetime_delete(&dt);
}

void logger_setmode(int mode) {
    logger_mode = mode;
}
