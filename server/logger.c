#include <stdlib.h>
#include <stdio.h>
#include "logger.h"
#include "datetime.h"

int logger_mode = ALL;

#define is_logmode(mode)((logger_mode & mode) == mode)


void logger_debug(char *msg) {
    if (!is_logmode(DEBUG)) return ;

    DATETIME *dt = datetime_now();
    char *dt_string = datetime_isoformat(dt);

    printf("\033[1;32m[ %s ] %s \033[0m\n", dt_string, msg);

    free(dt_string);
    datetime_delete(&dt);
}

void logger_error(char *msg) {
    if (!is_logmode(ERROR)) return ;

    DATETIME *dt = datetime_now();
    char *dt_string = datetime_isoformat(dt);

    printf("\033[1;31m[ %s ] %s \033[0m\n", dt_string, msg);

    free(dt_string);
    datetime_delete(&dt);
}

void logger_warning(char *msg) {
    if (!is_logmode(WARN)) return ;

    DATETIME *dt = datetime_now();
    char *dt_string = datetime_isoformat(dt);

    printf("\033[1;33m[ %s ] %s \033[0m\n", dt_string, msg);

    free(dt_string);
    datetime_delete(&dt);
}

void logger_info(char *msg) {
    if (!is_logmode(INFO)) return ;

    DATETIME *dt = datetime_now();
    char *dt_string = datetime_isoformat(dt);

    printf("\033[1;37m[ %s ] %s \033[0m\n", dt_string, msg);

    free(dt_string);
    datetime_delete(&dt);
}

void logger_setmode(int mode) {
    logger_mode = mode;
}
