// File: kbd.c
// Descriptions: keyboard driver



#include "../kernel/x86.h"
#include "../kernel/kernel.h"
#define NO 0
extern void backspace();

char keys[] = {
    NO, '~', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', NO, 
    NO, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '\\',
    NO, NO, 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', NO,
    NO, NO, NO, ' ', NO, NO, NO, 'o'
};


// Initializes keyboard before run
void keyboard_init(){
    // outb(0x64, 0xAD);
    // outb(0x64, 0xA7);
}

void keyboard_handler(){
    unsigned int status = inb(0x64);
    char scancode;

    if (status & 0x01) {
        scancode = inb(0x60);
        if(scancode < 0)
			return;
        if (keys[scancode] == '\b')
            backspace();
        else if (keys[scancode] == '\t')
            for (int i = 0; i <= 4; i++)
                printc(green, black, ' ');
        else 
            printc(green, black, keys[scancode]);
    }
}