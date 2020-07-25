// File: vga_print.h
// Description: includes functions to print strings and numbers to the VGA text buffer at 0xb8000
// Problem: backspace can clean outside text buffer, print can print outside text buffer, no scrolling, cursor disappears when letter is removed and only appears when letter is added on the places where it was peviously removed from


/// Includes ///
#include "../kernel/kernel.h"
#include "../kernel/x86.h"

/// Defines ///
#define VGA_WIDTH 80
#define COLUMNS 80
#define ROWS 25  
#define BLANK 0

/// Declarations ///
volatile char *cursor = (volatile char*)VGA;
int cell; // Counts cells(2 bytes - char + color)
int byte; // Counts each byte in video memory
int x;
int y;




/// Cursor functions ///
// Enable the cursor
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

// Disable the cursor 
void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

// Update the position of the cursor 
void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void updatexy(){
    x = cell % 80; 
    y = (int)(cell / 80);
    update_cursor(x, y);
}




/// Display Output functions ///
// Outputs a character
void printc(enum VGA_COLOR fg, enum VGA_COLOR bg, const char character){
    // Mixing colours into one byte
    uint8_t color = fg | bg << 4;

    cursor[byte++] = character;
    cursor[byte++] = color;

    cell++;
    updatexy();
}

// Outputs the text and colors to the VGA text buffer
void prints(enum VGA_COLOR fg, enum VGA_COLOR bg, const char *string){
    // Mixing colours into one byte
    uint8_t color = fg | bg << 4;

    // Pointer to VGA character buffer
    while(*string != 0){
        cursor[byte++] = *string++;
        cursor[byte++] = color;
        
        cell++;
    }
    
    updatexy();
}

// Removes last printed letter
void backspace(){
    cursor[byte--] = BLANK;
    cursor[byte--] = BLANK;
    //cursor[byte] = BLANK;

    --cell;
    updatexy();
}




// Info prints ///
void display_gdt(){
    prints(green, black, "Initialized: GDT, ");
}

void display_idt(){
    prints(green, black, "Initialised: IDT, ");
}

void software_int_test(){
    prints(green, black, "Initialised: Interrupts");
}




// EXTRA ///
void clear(){
    volatile char* text_buffer = (char*)VGA;
    for (int i; i <= 2000; i++){
        text_buffer[i] = BLANK;
    }
}