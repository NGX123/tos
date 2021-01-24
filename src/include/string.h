/*
	@author = COPYRIGHT Linus Torvals Linux v1.3
	@brief = basic c string library header
*/


#ifndef STRING_H
#define STRING_H


#include <stdint.h>
#include <stddef.h>


#ifndef NULL
#define NULL ((void *) 0)
#endif


void* memcpy(void*, const void*, size_t);
void* memmove(void*, const void*, size_t);
void* memccpy(void*, const void*, int, size_t);
void* memset(void*, int, size_t);
int memcmp(const void*, const void*, size_t);
void* memchr(const void*, int, size_t);
void* memrchr(const void*, int, size_t);

char* strcpy(char*, const char*);
char* strncpy(char*, const char*, size_t);
char* strcat(char*, const char*);
char* strncat(char*, const char*, size_t);
int strcmp(const char*, const char*);
int strncmp(const char*, const char*, size_t);

int strcoll(const char*, const char*);
size_t strxfrm(char*, const char*, size_t);
char* strdup(const char*);
char* strndup(const char*, size_t);
char* strchr(const char*, int);
char* strrchr(const char*, int);
size_t strcspn(const char*, const char*);
char* strpbrk(const char*, const char*);
char* strstr(const char*, const char*);
char* strtok(char*, const char*);
char* strtok_r(char*, const char*, char**);
size_t strlen(const char*);
#endif