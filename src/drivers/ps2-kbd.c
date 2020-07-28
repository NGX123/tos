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
#define LARROW 0x4B
#define RARROW 0x4D
// #define INSERT 0xE0 // 0x52
// #define PAGE_UP 0xE0 // 0x7D
// #define PAGE_DOWN 0xE0 // 0x7A
// cursor up 0x48, cursor down 0x50, cursor right 0x4D, cursor left 0x4B

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
    unsigned char caps_flag;
    unsigned char scancode;
    char scanset = scanset1[scancode];

    if (status & 0x01) {
        scancode = inb(0x60);

        
        if (scanset == CAPSLOCK && caps_flag == 0x01){
                scanset = scanset1[scancode];
                return;
        }
        if (caps_flag == 0x01)
            scanset = shiftset[scancode];
        else if (caps_flag == 0x00 || scancode == 0xB6 || scancode == 0xAA){
            scanset = scanset1[scancode];
            caps_flag = 0x00;
        }
        
        switch (scanset){
            case NO:
            case ESC:
            case CONTROL:
            case ALT:
                break;
            case '\b':
                backspace();
                break;
            case '\t':
                for (int i = 0; i <= 4; i++)
                    printc(green, black, ' ');
                break;
            case '\n':
                enter();
                break;
            case LARROW:
                arrows('<');
                break;
            case RARROW:
                arrows('>'); 
                break;
            case CAPSLOCK:
            case SHIFT:
                caps_flag = 0x01;
                break;
            default:
                if (scanset != CAPSLOCK)
                    printc(green, black, scanset);
                break;
        }
    }
}

