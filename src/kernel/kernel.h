// File: kernel.h
// Description: incldes declaration for the functions



#include <stdint.h>

// External functions
void printk(char* string);
void clear();

void init_serial();
void write_serial(char* a);

void setFlat();
void idt_init(void);
void keyboard_init();

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(int x, int y); 