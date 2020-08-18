// File: kernel.h
// Description: incldes declaration for the functions


#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>

// External functions
extern void  printk(char* string);
extern void  clear();

extern void  init_serial();
extern void  write_serial(char* a);

extern void  setFlat();
extern void  idt_init();
extern void  keyboard_init();

extern void  enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
extern void  disable_cursor();
extern void  update_cursor(int x, int y); 
#endif