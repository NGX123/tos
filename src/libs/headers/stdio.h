// File: stdio.h
// Description: stdio input/output library header



#ifndef STDIO_H
#define STDIO_H

// Prints a character to the screen
extern void putchar(int chara);

// Prints a string to the screen
extern void puts(char* str);

// Prints formatted output to the screen - only understands %d, %x, %p, %s
extern void printf(const char *fmt, ...);
#endif