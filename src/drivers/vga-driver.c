// File: vga_print.h
// Description: includes functions to print strings and numbers to the VGA text buffer at 0xb8000
// Problem: backspace can clear outside the buffer


#include "../kernel/kernel.h"
volatile char *cursor = (volatile char*)VGA;


// Outputs a character
void printc(enum VGA_COLOR fg, enum VGA_COLOR bg, const char character){
    // Mixing colours into one byte
    uint8_t color = fg | bg << 4;

    *cursor++ = character;
    *cursor++ = color;
}

// Outputs the text and colors to the VGA text buffer
void prints(enum VGA_COLOR fg, enum VGA_COLOR bg, const char *string){
    // Mixing colours into one byte
    uint8_t color = fg | bg << 4;

    // Pointer to VGA character buffer
    while(*string != 0){
        *cursor++ = *string++;
        *cursor++ = color;
    }
}

void backspace(){
    *cursor-- = 0;
    *cursor-- = 0;
}




// EXTRA
void display_gdt(){
    prints(green, black, "Initialized: GDT, ");
}

void display_idt(){
    prints(green, black, "Initialised: IDT, ");
}

void software_int_test(){
    prints(green, black, "Initialised: Inturreupts");
}