#ifndef BUFFER_H
#define BUFFER_H

typedef struct buff_ BUFFER;

#define BUFF_SIZE 4096

BUFFER *buffer_create(int cap);
void buffer_del(BUFFER *b);
char *buffer_content(BUFFER *b); 
void buffer_insert(BUFFER *b, char c);
void buffer_mv(BUFFER *b, int i);
void buffer_setcursor(BUFFER *b, int i);
void buffer_end(BUFFER *b);
int buffer_len(BUFFER *b);
void buffer_clear(BUFFER *b);
void buffer_print(BUFFER *b, int y, int x); 
char buffer_ended(BUFFER *b);
void buffer_start(BUFFER *b);

#endif
