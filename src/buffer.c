#include <stdlib.h>
#include <ncurses.h>
#include "buffer.h"

struct buff_ {
    char *content;
    int cursor;
    int length;
    int capacity;
    char end;
};

BUFFER *buffer_create(int cap) {
    BUFFER *buffer = (BUFFER *)malloc(sizeof(BUFFER));
    if (buffer==NULL) return NULL;
    buffer->content = (char*)malloc((cap+1)*sizeof(char));
    buffer->cursor = 0;
    buffer->length = 0;
    buffer->end = 0;
    buffer->capacity = cap;
    return buffer;
}

void buffer_start(BUFFER *b) {
    b->end = 0;
}

// removes element at cursor position
void buffer_del(BUFFER *b) {
    if (b->cursor == 0)
        return;

    for (int i=b->cursor; i<b->length; i++)
        b->content[i] = b->content[i+1];
    //b->content[b->length-1] = '\0';
    b->cursor--;
    b->length--;
}
char buffer_ended(BUFFER *b) {
    return b->end;
}
char *buffer_content(BUFFER *b) {
    return b->content;
}

void buffer_setcursor(BUFFER *b, int i) {
    
    i = i>=0 ? i : abs(b->length + i+1);

    b->cursor = i % (b->length+1);
}

void buffer_insert(BUFFER *b, char c) {
    if (b->length == b->capacity)
        return ;
    
    for (int i=b->length; i>b->cursor; i--) {
        b->content[i] = b->content[i-1];
    }
    b->content[b->cursor++] = c;
    b->length++;
}

void buffer_mv(BUFFER *b, int i) {
    if (b->cursor+i < 0 || b->cursor+i > b->length)
        return ;
    b->cursor += i;
}

void buffer_delete(BUFFER **buffer) {
    free((*buffer)->content);
    free(*buffer);
    *buffer = NULL;
}

void buffer_end(BUFFER *b) {
    b->content[b->length] = '\0';
    b->end = 1;
}

int buffer_len(BUFFER *b) {
    return b->length;
}

void buffer_clear(BUFFER *b) {
    b->length=0;
    b->cursor=0;
}

void buffer_print(BUFFER *b, int y, int x) {
    int x0, y0;
    getyx(stdscr, y0, x0);
    
    if (b->cursor == 0)
        mvprintw(y, x++, "|");

    for (int i=0; i<b->length; i++) {
        mvprintw(y, x++, "%c", b->content[i]);
        if (i == b->cursor-1)
            mvprintw(y, x++, "|");
    }
    move(y0, x0);
}

