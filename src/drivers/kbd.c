// File: kbd.c
// Descriptions: keyboard driver



// Includes
#include "../kernel/x86.h"
#include "../kernel/kernel.h"
#include <stdint.h>

// Defines
#define NO 0x0
#define ESC 0x01
#define CONTROL 0x1D
#define SHIFT 0x2A
#define ALT 0x38
#define CAPSLOCK 0x3A
#define INSERT 0xE0 // 0x52
#define PAGE_UP 0xE0 // 0x7D
#define PAGE_DOWN 0xE0 // 0x7A
// cursor up 0x48, cursor down 0x50, cursor right 0x4D, cursor left 0x4B

extern void backspace();
extern void enter();
extern void arrows();

// Scancode set 1
char keys[] = {
    NO, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',        // 0x0*
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', CONTROL, 'a', 's',    // 0x1*
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', SHIFT, '\\', 'z', 'x', 'c', 'v',     // 0x2*
    'b', 'n', 'm', ',', '.', '/', SHIFT, '*', ALT, ' ', CAPSLOCK, NO, NO, NO, NO, NO, 
    NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, '<', NO, '>', NO, NO, NO                                
};

void keyboard_handler(){
    uint32_t status = inb(0x64);
    char scancode;

    if (status & 0x01) {
        scancode = inb(0x60);
        if(scancode < 0)
			return;
    
        if (keys[scancode] == NO || keys[scancode] == ESC || keys[scancode] == CONTROL || keys[scancode] == SHIFT || keys[scancode] == SHIFT || keys[scancode] == ALT)
            /* Do not print special or not setup keys */;
        else if (keys[scancode] == '\b')
            backspace();
        else if (keys[scancode] == '\t')
            for (int i = 0; i <= 4; i++)
                printc(green, black, ' ');
        else if (keys[scancode] == '\n')
            enter();
        else if (keys[scancode] == '<')
            arrows('<');
        else if (keys[scancode] == '>')
            arrows('>');
        else 
            printc(green, black, keys[scancode]);
    }
}

