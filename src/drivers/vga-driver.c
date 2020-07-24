// File: vga_print.h
// Description: includes functions to print strings and numbers to the VGA text buffer at 0xb8000
// Problem: backspace can clean outside text buffer, print can print outside text buffer, no scrolling


#include "../kernel/kernel.h"

// Declarations and defines 
#define COLUMNS 80
#define ROWS 25  
#define BLANK 0
volatile char *cursor = (volatile char*)VGA;
int x;
int y;




void updatexy(){
    
}

// Outputting using x and y matrix
// Outputs a character
void printcxy(enum VGA_COLOR fg, enum VGA_COLOR bg, const char character){
    uint8_t color = fg | bg << 4;
    
    cursor[y * COLUMNS + x] = character;
    x++;
    cursor[y * COLUMNS + x] = color;
    x++;
    update_cursor(x, y);
}

// Outputs a string
void printsxy(enum VGA_COLOR fg, enum VGA_COLOR bg, const char *string){
    uint8_t color = fg | bg << 4;

    while (*string != 0){
        cursor[y * COLUMNS + x] = *string++;
        x++;
        cursor[y * COLUMNS + x] = color;
        x++;
    }
    update_cursor(x, y);
}

// Removes last letter
void backspacexy(){
    cursor[y * COLUMNS + x] = BLANK;
    x--;
    cursor[y * COLUMNS + x] = BLANK;
    x--;
    update_cursor(x, y);
}




// Outputting by just going forward in memory
// Outputs a character
void printc(enum VGA_COLOR fg, enum VGA_COLOR bg, const char character){
    // Mixing colours into one byte
    uint8_t color = fg | bg << 4;

    *cursor++ = character;
    *cursor++ = color;
    update_cursor(x, y);
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
    update_cursor(x, y);
}

// Removes last printed letter
void backspace(){
    *cursor-- = BLANK;
    *cursor-- = BLANK;
    update_cursor(x, y);
}




// Info prints
void display_gdt(){
    //prints(green, black, "Initialized: GDT, ");
    printsxy(green, black, "Initialized: GDT, ");
}

void display_idt(){
    //prints(green, black, "Initialised: IDT, ");
    printsxy(green, black, "Initialised: IDT, ");
}

void software_int_test(){
    //prints(green, black, "Initialised: Interrupts");
    printsxy(green, black, "Initialised: Interrupts, ");
}




// EXTRA
void clear(){
    volatile char* text_buffer = (char*)VGA;
    for (int i; i <= 2000; i++){
        text_buffer[i] = BLANK;
    }
}