#include <stdlib.h>
#include <signal.h>
#include "flags.h"

void sig_handle_sigint(int sig) {
    stop_server = 1;

}


