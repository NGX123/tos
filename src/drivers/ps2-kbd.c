// File: kbd.c
// Descriptions: keyboard driver



// Includes
#include "../kernel/x86.h"
#include "driver.h"
#include <stdint.h>

// Defines
#define NO 0x0
#define ESC 0x01
#define CONTROL 0x1D
#define SHIFT 0x2A
#define ALT 0x38
#define CAPSLOCK 0x3A
#define LARROW 0x4B
#define RARROW 0x4D

extern void backspace();
extern void enter();
extern void arrows();

// Scancode set 1
char scanset1[] = {
    NO, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',        // 0x0*
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', CONTROL, 'a', 's',    // 0x1*
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', SHIFT, '\\', 'z', 'x', 'c', 'v',     // 0x2*
    'b', 'n', 'm', ',', '.', '/', SHIFT, '*', ALT, ' ', CAPSLOCK, NO, NO, NO, NO, NO, 
    NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, LARROW, NO, RARROW, NO, NO, NO                                
};

char shiftset[] = {
    NO, ESC, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', CONTROL, 'A', 'S', 
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '|', SHIFT, '>', 'Z', 'X', 'C', 'V', 
    'B', 'N', 'M', '<', '>', '?', SHIFT, '*', ALT, ' ', CAPSLOCK, NO, NO, NO, NO, NO,
    NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, NO, LARROW, NO, RARROW, NO, NO, NO  
};

void keyboard_handler(){
    uint32_t status = inb(0x64);
    char scancode;

    if (status & 0x01) {
        scancode = inb(0x60);



        // RETURN TO TEST
        // if(scancode < 0)
		// 	return;

        // switch (scanset1[scancode]){
        //     case NO:
        //     case ESC:
        //     case CONTROL:
        //     case ALT:
        //     case CAPSLOCK:
        //     case SHIFT:
        //         break;
        //     case '\b':
        //         backspace();
        //         break;
        //     case '\t':
        //         for (int i = 0; i <= 4; i++)
        //             printc(green, black, ' ');
        //         break;
        //     case '\n':
        //         enter();
        //         break;
        //     case LARROW:
        //         arrows('<');
        //         break;
        //     case RARROW:
        //         arrows('>'); 
        //         break;
        //     default:
        //         printc(green, black, scanset1[scancode]);
        //         break;
        // }
    }
}

