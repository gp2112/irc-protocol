#include <ncurses.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "interface.h"
#include "msg.h"

#define MAINSCR 0.8

void interface_init() {

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    intrflush(stdscr, 0);
    //leaveok(stdscr, 1);
}

void writeMsg(char *msg, char *usr, int port, int *y) {
    int y0, x0, max_y, max_x;
    getyx(stdscr, y0, x0);
    getmaxyx(stdscr, max_y, max_x);
   
    if (port > 0)
        mvprintw(*y, 2, "%s:%d - %s", usr, port, msg);
    else
        mvprintw(*y, 2, "%s - %s", usr, msg);

    *y += 1+(int)((6+strlen(usr)+strlen(msg))/(max_x));
    move(y0, x0);
}


void eraseChars(int y0, int y, int x0, int x) {
    for (int i=y0; i<=y; i++)
        for (int j=x0; j<=x; j++)
            mvprintw(i, j, " ");
}

void printBar(int y, int min_x, int max_x) {
    move(y, min_x);
    for (int x=min_x; x<max_x; x++) {
        mvprintw(y, x, "=");
    }
}

// create a TUI page.
void inter_page(int y0, int y, int x0, int x, char full) {
    for (int i=y0; i<=y; i++) {
        if (full || i==y0 || i==y) 
            for (int j=x0; j<=x; j++)
                mvprintw(i, j, "=");
    }
}

void print_error(int err) {
    return;
}

void msg_box(BUFFER *buffer, int w, int h, char *msg, int n) {
    int y, x, max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    echo();
    curs_set(1);

    y = max_y >> 1;
    x = (max_x >> 1);

    inter_page(y-(h>>1), y+(h>>1), x - w, x + w, 0);

    mvprintw(y, x-strlen(msg), msg);
    move(y,x);
    read_input(buffer, n);
    eraseChars(y, y, x, max_x); 
    
    eraseChars(y-(h>>1), y+(h>>1), x - w, x + w);

    noecho();
    curs_set(0);

}

int read_input(BUFFER *buffer, int n) {
    int c;
    int i=0;
    buffer_start(buffer);
    do {
        c = getch();
        if(!(c & KEY_CODE_YES) && isprint(c)) {
            buffer_insert(buffer, c);
            continue;
        }
        //mvprintw(9, 9, "%c", c);
        switch (c) {
        

            case ERR: break;
            case KEY_LEFT:  
                buffer_mv(buffer, -1);
                break;
            case KEY_RIGHT: 
                buffer_mv(buffer, 1);
                break;
            case KEY_HOME:
                buffer_setcursor(buffer, 0);
                break;
            case KEY_END:
                buffer_setcursor(buffer, -1);
                break;
            case 27:
                return 1;
            case '\t':
                buffer_insert(buffer, '\t');
                break;
            case KEY_BACKSPACE:
            case 127:
            case 8:
                buffer_del(buffer);
                // Fall-through
                break;
            
        }
    } while (i<n && c != '\n');
    buffer_end(buffer);
    return 0;
}


void inter_getIpPort(BUFFER *buffer, char *ip, int *port) {
    
        
    msg_box(buffer, 20, 10, "Server Address: ", 20);
    
    strcpy(ip, buffer_content(buffer));
    buffer_clear(buffer);
    
    msg_box(buffer, 20, 10, "Server Port: ", 20);

    *port = atoi(buffer_content(buffer));
    buffer_clear(buffer);
}

void print_messages(QUEUE *msg_rcvd, BUFFER *buffer, int *kill) {
    int x, y, msg_pos=2;
    MSG *msg;

    while(!(*kill)) {
        getmaxyx(stdscr, y, x);

        while (!queue_empty(msg_rcvd)) {
            msg = queue_pop(msg_rcvd);
            writeMsg(msg->content, msg->peer_ip, msg->peer_port, &msg_pos);
        }

        printBar(y-3, 0, x);
        printBar(y-1, 0, x);
        mvprintw(y-2, 0, "Message: ");

        buffer_print(buffer, y-2, 10);
        if (buffer_ended(buffer))
            eraseChars(y-2, y-2, 10, x);
        else
            eraseChars(y-2, y-2, 11+buffer_len(buffer), x);
        sleep(0.3);
        refresh();
        
    }
}

void interface_close(int sockfd) {
    endwin();
    close(sockfd);
}
