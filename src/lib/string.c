/*
	@author = Copyright (c) 2011, 2012, 2014 Jonas 'Sortie' Termansen.
	@license =
		* Permission to use, copy, modify, and distribute this software for any
		* purpose with or without fee is hereby granted, provided that the above
		* copyright notice and this permission notice appear in all copies.
		*
		* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
		* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
		* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
		* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
		* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
		* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
		* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
	@brief = string library
*/


#include <string.h>
#include <stdint.h>
#include <stdbool.h>


void memcpy(void *dest, void *src, size_t n) {	/*CUSTOM*/
   int i;
   char *src_char = (char *)src;
   char *dest_char = (char *)dest;
   for (i=0; i<n; i++)
      dest_char[i] = src_char[i];
}

void* memmove(void* dest_ptr, const void* src_ptr, size_t n)	/*Copy memory between potentionally overlapping regions, SORTIX*/
{
	unsigned char* dest = (unsigned char*) dest_ptr;
	const unsigned char* src = (const unsigned char*) src_ptr;
	if ( (uintptr_t) dest < (uintptr_t) src )
	{
		for ( size_t i = 0; i < n; i++ )
			dest[i] = src[i];
	}
	else
	{
		for ( size_t i = 0; i < n; i++ )
			dest[n-(i+1)] = src[n-(i+1)];
	}
	return dest_ptr;
}

void* memccpy(void* dest_ptr, const void* src_ptr, int c, size_t n)	/*Copy memory until length is met or character is encountered, SORTIX*/
{
	unsigned char* dest = (unsigned char*) dest_ptr;
	const unsigned char* src = (const unsigned char*) src_ptr;
	for ( size_t i = 0; i < n; i++ )
	{
		if ( (dest[i] = src[i]) == (unsigned char) c )
			return dest + i + 1;
	}
	return NULL;
}

void *memset(void *s, int c, size_t n)	/*CUSTOM*/
{
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

int memcmp(const void* a_ptr, const void* b_ptr, size_t size)	/*Compares two memory regions, SORTIX*/
{
	const unsigned char* a = (const unsigned char*) a_ptr;
	const unsigned char* b = (const unsigned char*) b_ptr;
	for ( size_t i = 0; i < size; i++ )
	{
		if ( a[i] < b[i] )
			return -1;
		if ( a[i] > b[i] )
			return +1;
	}
	return 0;
}

void* memchr(const void* s, int c, size_t size)	/*Scans memory for a character, SORTIX*/
{
	const unsigned char* buf = (const unsigned char*) s;
	for ( size_t i = 0; i < size; i++ )
		if ( buf[i] == (unsigned char) c )
			return (void*) (buf + i);
	return NULL;
}

void* memrchr(const void* ptr, int c, size_t n)	/*Scans memory in reverse directory for a character, SORTIX*/
{
	const unsigned char* buf = (const unsigned char*) ptr;
	for ( size_t i = n; i != 0; i-- )
		if ( buf[i-1] == (unsigned char) c )
			return (void*) (buf + i-1);
	return NULL;
}

char* strcpy(char* dest, const char* src)	/*Copies a string and returns dest, SORTIX*/
{
	size_t index;
	for ( index = 0; src[index]; index++ )
		dest[index] = src[index];
	dest[index] = '\0';
	return dest;
}

char* strncpy(char* dest, const char* src, size_t n)	/*Copies a string into a fixed size buffer and returns dest, SORTIX*/
{
	size_t i;
	for ( i = 0; i < n && src[i] != '\0'; i++ )
		dest[i] = src[i];
	for ( ; i < n; i++ )
		dest[i] = '\0';
	return dest;
}

char* strcat(char* dest, const char* src)	/*Appends a string onto another string, SORTIX*/
{
	strcpy(dest + strlen(dest), src);
	return dest;
}

char* strncat(char* dest, const char* src, size_t n)	/*Appends parts of a string onto another string, SORTIX*/
{
	size_t dest_len = strlen(dest);
	size_t i;
	for ( i = 0; i < n && src[i] != '\0'; i++ )
		dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';
	return dest;
}

int strcmp(const char* a, const char* b)	/*Compares two strings, SORTIX*/
{
	for ( size_t i = 0; true; i++ )
	{
		unsigned char ac = (unsigned char) a[i];
		unsigned char bc = (unsigned char) b[i];
		if ( ac == '\0' && bc == '\0' )
			return 0;
		if ( ac < bc )
			return -1;
		if ( ac > bc )
			return 1;
	}
}

int strncmp(const char* a, const char* b, size_t max_count)	/*Compares a prefix of two strings, SORTIX*/
{
	for ( size_t i = 0; i < max_count; i++ )
	{
		unsigned char ac = (unsigned char) a[i];
		unsigned char bc = (unsigned char) b[i];
		if ( ac == '\0' && bc == '\0' )
			return 0;
		if ( ac < bc )
			return -1;
		if ( ac > bc )
			return 1;
	}
	return 0;
}

int strcoll(const char* s1, const char* s2)	/*Compare two strings using the current locale, SORTIX*/
{
	// TODO: Pay attention to locales.
	return strcmp(s1, s2);
}

size_t strxfrm(char* dest, const char* src, size_t n)	/*Transform a string such that the result of strcmp is the same as strcoll, SORTIX*/
{
	size_t srclen = strlen(src);
	strncpy(dest, src, n);
	return srclen;
}

char* strdup(const char* input)	/*Creates a copy of a string, SORTIX*/
{
	size_t input_length = strlen(input);
	char* result = (char*) malloc(input_length + 1);
	if ( !result )
		return NULL;
	memcpy(result, input, input_length + 1);
	return result;
}

char* strndup(const char* input, size_t n)	/*Creates a copy of a string, SORTIX*/
{
	size_t input_size = strnlen(input, n);
	char* result = (char*) malloc(input_size + 1);
	if ( !result )
		return NULL;
	memcpy(result, input, input_size);
	result[input_size] = 0;
	return result;
}

char* strchr(const char* str, int uc)	/*Searches a string for a specific character, SORTIX*/
{
	char* ret = strchrnul(str, uc);
	return (unsigned char) uc == ((unsigned char*) ret)[0] ? ret : NULL;
}

char* strrchr(const char* str, int uc)	/*Searches a string for a specific character, SORTIX*/
{
	const unsigned char* ustr = (const unsigned char*) str;
	const char* last = NULL;
	for ( size_t i = 0; true; i++ )
	{
		if ( ustr[i] == (unsigned char) uc )
			last = str + i;
		if ( !ustr[i] )
			break;
	}
	return (char*) last;
}

size_t strcspn(const char* str, const char* reject)	/*Search a string for a set of characters, SORTIX*/
{
	size_t reject_length = 0;
	while ( reject[reject_length] )
		reject_length++;
	for ( size_t result = 0; true; result++ )
	{
		char c = str[result];
		if ( !c )
			return result;
		bool matches = false;
		for ( size_t i = 0; i < reject_length; i++ )
		{
			if ( str[result] != reject[i] )
				continue;
			matches = true;
			break;
		}
		if ( matches )
			return result;
	}
}

size_t strspn(const char* str, const char* accept) /*Search a string for a set of characters, SORTIX*/
{
	size_t accept_length = 0;
	while ( accept[accept_length] )
		accept_length++;
	for ( size_t result = 0; true; result++ )
	{
		char c = str[result];
		if ( !c )
			return result;
		bool matches = false;
		for ( size_t i = 0; i < accept_length; i++ )
		{
			if ( str[result] != accept[i] )
				continue;
			matches = true;
			break;
		}
		if ( !matches )
			return result;
	}
}

char* strpbrk(const char* str, const char* accept)	/*Search a string for any of a set of characters, SORTIX*/
{
	size_t reject_length = strcspn(str, accept);
	if ( !str[reject_length] )
		return NULL;
	return (char*) str + reject_length;
}

char* strstr(const char* haystack, const char* needle)	/*Locate a substring, SORTIX*/
{
	if ( !needle[0] )
		return (char*) haystack;
	for ( size_t i = 0; haystack[i]; i++ )
	{
		bool diff = false;
		for ( size_t j = 0; needle[j]; j++ )
		{
			if ( haystack[i+j] != needle[j] ) { diff = true; break; }
		}
		if ( diff )
			continue;
		return (char*) haystack + i;
	}
	return NULL;
}

char* strtok(char* str, const char* delim)	/*Extract tokens from strings, SORTIX*/
{
	static char* lasttokensaveptr = NULL;
	return strtok_r(str, delim, &lasttokensaveptr);
}

char* strtok_r(char* str, const char* delim, char** saveptr)	/*Extract tokens from strings, SORTIX*/
{
	if ( !str && !*saveptr )
		return NULL;
	if ( !str )
		str = *saveptr;
	str += strspn(str, delim); // Skip leading
	if ( !*str )
		return *saveptr = NULL;
	size_t amount = strcspn(str, delim);
	if ( str[amount] )
		*saveptr = str + amount + 1;
	else
		*saveptr = NULL;
	str[amount] = '\0';
	return str;
}

size_t strlen(const char* str)	/*Returns the length of a string, SORTIX*/
{
	size_t ret = 0;
	while ( str[ret] )
		ret++;
	return ret;
}

char* strchrnul(const char* str, int uc)	/*Searches a string for a specific character, SORTIX*/
{
	const unsigned char* ustr = (const unsigned char*) str;
	for ( size_t i = 0; true; i++)
		if ( ustr[i] == (unsigned char) uc || !ustr[i] )
			return (char*) str + i;
}
