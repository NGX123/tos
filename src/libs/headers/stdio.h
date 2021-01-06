/*
    @author = ngx123
    @brief = standard c input/output library header
*/


#ifndef STDIO_H
#define STDIO_H


extern int putchar(int chara);

extern int puts(char* str);

extern void printf(const char *fmt, ...);

int atoi(const char *s);
#endif