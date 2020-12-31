// File: stdio.h
// Description: stdio input/output library header

#ifndef STDIO_H
#define STDIO_H

// Prints a character to the screen
extern int putchar(int chara);

// Prints a string to the screen
extern int puts(char* str);

// Prints formatted output to the screen - only understands %d, %x, %p, %s, %c
extern void printf(const char *fmt, ...);

// Transfer to appropriate library later
// Converts string to integer
int atoi(const char *s);

#endif