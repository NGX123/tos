// File: x86.h
// Description: includes defines to use with the x86 CPUs



#ifndef X86_H
#define X86_H

extern unsigned char inb(unsigned short port);
extern void outb(unsigned short port, unsigned char data);
#endif

