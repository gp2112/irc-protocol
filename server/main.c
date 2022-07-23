#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "server.h"
#include "logger.h"
#include "params.h"


char stop_server = 0;


void sig_handle_sigint(int sig) {
    logger_debug("%s", "\rSIGINT pressed. Server should be closed now...");
    stop_server = 1;
}


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
    
    SERVER *server = server_init(hostname, port, &stop_server);
    
    if (server == NULL) {
        logger_error("%s", "PANIC: Server creating have failed!");
        return 1;
    }


    logger_info("%s", "Server started with success!");
    logger_info("%s", "Listening for connections... (Press CTRL+C to leave)\n");

    server_run(server);
    
    logger_debug("%s", "Closing server...");

    server_delete(&server); 

    return 0;

}

