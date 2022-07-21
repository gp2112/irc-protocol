#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "signalhandler.h"


int main(int argc, char *argv[]) {
    signal(SIGINT, sig_handle_sigint);
    return 0;

}

