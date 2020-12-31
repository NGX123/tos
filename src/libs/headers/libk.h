// File: libk.h
// Description: header file for declarations of special functions for kernel

#ifndef LIBK_H
#define LIBK_H

// Prints a system message with special formatting
extern void printsys(char* string);

// Print a message when gdt was initialised
extern void display_gdt();

// Print a message when idt was initialised
extern void display_idt();
#endif