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
extern char * strchr(const char *,int);
extern char * strrchr(const char *,int);
extern char * strpbrk(const char *,const char *);
extern char * strtok(char *,const char *);
extern char * strstr(const char *,const char *);
extern size_t strlen(const char *);
extern size_t strnlen(const char *,size_t);
extern size_t strspn(const char *,const char *);
extern int strcmp(const char *,const char *);
extern int strncmp(const char *,const char *,size_t);

//extern void * memset(void *,int,size_t);
extern void * memcpy(void *,const void *,size_t);
extern void * memmove(void *,const void *,size_t);
//extern void * memscan(void *,int,size_t);
extern int memcmp(const void *,const void *,size_t);
#endif