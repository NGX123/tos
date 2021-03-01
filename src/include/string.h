/*
	@author 	= ngx, Includes code taken from Sortix
	@brief 		= basic c string library header
	@original 	= https://gitlab.com/sortix/sortix
	@license	= ISC
		Copyright 2011-2016 Jonas 'Sortie' Termansen and contributors.

		Permission to use, copy, modify, and distribute this software for any
		purpose with or without fee is hereby granted, provided that the above
		copyright notice and this permission notice appear in all copies.

		THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
		WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
		MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
		ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
		WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
		ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
		OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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
// char* strdup(const char*);
// char* strndup(const char*, size_t);
char* strchr(const char*, int);
char* strchrnul(const char* str, int c);
char* strrchr(const char*, int);
size_t strcspn(const char*, const char*);
size_t strspn(const char*, const char*);
char* strpbrk(const char*, const char*);
char* strstr(const char*, const char*);
char* strtok(char*, const char*);
char* strtok_r(char*, const char*, char**);
size_t strlen(const char*);
#endif