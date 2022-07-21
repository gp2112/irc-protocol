#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "server.h"
#include "logger.h"
#include "signalhandler.h"
#include "params.h"



int main(int argc, char *argv[]) {
    
    char *hostname = "Localhost";
    int port = DEFAULT_PORT;

    if (argc > 1)
        hostname = argv[1];
   
    if (argc > 2)
        port = atoi(argv[2]);

    logger_info("%s %s %s %d", "Starting server ", hostname, " at port ", port);

    // handles ctrl+c (sigint)
    signal(SIGINT, sig_handle_sigint);
    
    SERVER *server = server_init(hostname, port);
    
    
    server_run(server);
   
    server_delete(&server); 

    return 0;

}

