// File: string.c
// Description: includes defines of string functions


#include <stdint.h>
#include <stddef.h>

#ifndef STRING_H
#define STRING_H

#ifndef NULL
#define NULL ((void *) 0)
#endif

extern char * ___strtok;
extern char * strcpy(char *,const char *);
extern char * strncpy(char *,const char *,size_t);
extern char * strcat(char *, const char *);
extern char * strncat(char *, const char *, size_t);
extern char * strchr(char *,int);
extern char * strrchr(char *,int);
extern char * strpbrk(char *,char *);
extern char * strtok(char *,char *);
extern const char * strstr(const char *,const char *);
extern uint32_t strlen(const char *);
extern size_t strnlen(const char *,size_t);
extern size_t strspn(const char *,const char *);
extern int strcmp(const char *,const char *);
extern int strncmp(const char *,const char *,size_t);
char * bcopy(const char * src, char * dest, int count);

extern void * memcpy(void *,void *,size_t);
extern void * memmove(void *,void *,size_t);
extern int memcmp(const void *,const void *,size_t);
//extern void * memset(void *,int,size_t);
//extern void * memscan(void *,int,size_t);
#endif