// File: vga_print.h
// Description: includes functions to print strings and numbers to the VGA text buffer at 0xb8000
// Problem: backspace can clean outside text buffer, print can print outside text buffer, no scrolling, cursor shows green after deletion and then turns white after returned to the place where letter was deleted


/// Includes ///
#include "../kernel/kernel.h"
#include "../kernel/x86.h"

/// Defines ///
#define VGA_WIDTH 80
#define COLUMNS 80
#define ROWS 25  
#define BLANK 0

/// Declarations ///
volatile char *text_buffer = (volatile char*)VGA;
enum VGA_COLOR terminal_fg = green;
enum VGA_COLOR terminal_bg = black;
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

    text_buffer[byte++] = character;
    text_buffer[byte++] = color;

    ++cell;
    // Change the color of the cursor by changing next cell color
    text_buffer[cell * 2 + 1] = color; 
    updatexy();
}

// Outputs the text and colors to the VGA text buffer
void prints(enum VGA_COLOR fg, enum VGA_COLOR bg, const char *string){
    uint8_t color = fg | bg << 4;

    while(*string != 0){
        text_buffer[byte++] = *string++;
        text_buffer[byte++] = color;
        
        ++cell;
    }
    
    text_buffer[cell * 2 + 1] = color;
    updatexy();
}

// Removes last printed letter
void backspace(){
    uint8_t color = terminal_fg | terminal_bg << 4;
    
    --byte;
    --byte;
    text_buffer[byte] = BLANK;

    --cell;
    text_buffer[cell * 2 + 1] = color; 
    updatexy();
}

// Moves onto the next line
void enter(){
    uint8_t color = terminal_fg | terminal_bg << 4;

    ++y;
    x = 0;
    cell = y * 80 + x;
    byte = cell * 2;

    text_buffer[cell * 2 + 1] = color;
    updatexy();
}

// Can print with all symbols like \t and automatiacally uses the console color without the need to set it up
void printk(char *string){
    uint8_t color = terminal_fg | terminal_bg << 4;

    for (int i = 0; string[i] != 0; i++){
        if (string[i] == '\n')
            enter();
        else if (string[i] == '\t')
            for (int i = 0; i <= 4; i++)
                printc(green, black, ' ');
        else 
            printc(terminal_fg, terminal_bg, string[i]);        
    }
    updatexy();
}




// Info prints ///
void display_gdt(){
    // prints(terminal_fg, terminal_bg, "Initialized: GDT, ");
    printk("Initialized: GDT\n");
}

void display_idt(){
    // prints(terminal_fg, terminal_bg, "Initialised: IDT, ");
    printk("Initialised: IDT\n");
}




// EXTRA ///
void clear(){
    volatile char* text_buffer = (char*)VGA;
    for (int i; i <= 2000; i++){
        text_buffer[i] = BLANK;
    }
}