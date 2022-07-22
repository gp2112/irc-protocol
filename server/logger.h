#ifndef LOG_H
#define LOG_H

#include "params.h"

/* 
 logging levels
 
 When can select more than one logging mode by making an OR betwhen all modes you want.

    Let's say you want a logging mode with DEBUG, ERROR and INFO. 
   Your mode param would be: mode = DEBUG | ERROR | INFO. (| being the or operator in C)

  Than, when can know, for example, if DEBUG is activated by making an AND 
  operation betwhen mode and DEBUG and checking if it's equals DEBUG. 
  This is true because: 

      (A or B or C) and B = (AB or BB or CB ) = B

    

*/


#define DEBUG 1
#define ERROR 2
#define  WARN 4
#define INFO  8


#define ALL 0xffffffff

void logger_info(char *fmt, ...);
void logger_error(char *fmt, ...);
void logger_warning(char *fmt, ...);
void logger_debug(char *fmt, ...);
void logger_setmode(int mode);

#endif
