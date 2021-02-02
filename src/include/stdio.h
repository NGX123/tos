/*
    @author = ngx123
    @brief = standard c input/output library header
*/


#ifndef STDIO_H
#define STDIO_H


#define PRINTSYS_STATUS_SUCCESS 0
#define PRINTSYS_STATUS_FAIL 1


/*
    @brief = prints a system initialization message
    @param string = the string to print
    @param status = the status of the print - FAIL/SUCCESS
*/
extern void printsys(char* string, int status);

extern int putchar(int chara);

extern int puts(char* str);

extern void printf(const char *fmt, ...);

int atoi(const char *s);
#endif