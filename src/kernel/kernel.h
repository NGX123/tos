// File: kernel.h
// Description: incldes declaration for the functions



#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <driver.h>

// External functions
extern void  printk(char* string);
extern void  clear();

extern void  init_serial();
extern void  write_serial(char* a);

extern void  setFlat();
extern void  idt_init();
extern void  keyboard_init();
#endif