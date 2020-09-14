// File: kbd.c
// Descriptions: keyboard driver



// Includes
#include <stdint.h>
#include "stdio.h"
#include "ringbuf.h"
#include "headers/kbd-defines.h"

#include "drivers/x86.h"
#include "drivers/vga.h"
#include "drivers/kbd.h"


#define BUFSIZE 255
static uint8_t charBuffer[BUFSIZE];
static ring_buffer_t charRingBufferStruct;

static uint8_t kbd_mode = 0;

/// MODES ///
// Keyboard standard mode interpretation function
// Standard mode - arrows keys are straight passed to the display, all shortcuts are passed to system as interrupts(not made yet), normal keys are passed to the read buffer
static void keyboardStdMode(uint32_t scancode, uint8_t character){
    // Write normal characters to the buffer
    if (character >= 32 && character <= 127 && scancode <= 80)
        writeBuf(&charRingBufferStruct, character);

    // Escape sequences
    else if (character == '\b' || character == '\t' || character == '\n') 
        writeBuf(&charRingBufferStruct, character);

    // Arrows
    else if (scancode == LARROW_SCAN)
        writeBuf(&charRingBufferStruct, scancode);
    else if (scancode == RARROW_SCAN)
        writeBuf(&charRingBufferStruct, scancode);
}

// Keyboard display mode interpretation function
// Display mode - nothing is sent to the buffer and everything gets automatically displayed on the screen, shortcuts are as interrupts
static void keyboardDisplayMode(uint32_t scancode, uint8_t character){
    // Print normal characters
    if (character >= 32 && character <= 127 && scancode <= 80)
        printScreen(character);

    // Escape sequences
    if (character == '\b' || character == '\t' || character == '\n')
        printScreen(character);

    // Handle the arrows
    if (scancode == LARROW_SCAN)
        printScreen(LARROW);
    else if (scancode == RARROW_SCAN)
        printScreen(RARROW);
}

/// INTERRUPT HANDLER ///
void keyboard_handler(){
    static uint8_t ctrlStatus = 0, altStatus = 0, shiftStatus = 0;              // Hold key statuses
    static uint8_t capslockStatus = 0;                                          // Press key statuses
    static uint8_t caps = 0;                                                    // Status of keys being lower case or higher case
    uint32_t keyboardStatus, scancode;                                          // Data from ports
    uint8_t character, scancode_byte2;                                           
    
    if ((keyboardStatus = inb(KEYBOARD_STATUS_PORT)) & KBS_DIB)
        scancode = inb(KEYBOARD_DATA_PORT);
    else 
        return;

    // Handle E0
    if (scancode == 0xE0){
        scancode_byte2 = inb(KEYBOARD_DATA_PORT);
        scancode = scancode << 4 | scancode_byte2;
        // MAYBE WRONG BECUASE OUTPUT IS NOT 0xE0xx, but 0xExx
    }

    // Hold Key Statuses //
    // Check alt 
    if (scancode == 0x38 || scancode == 0xE38)
        altStatus = 1;
    else if (scancode == 0xB8 && altStatus == 1)
        altStatus = 0;

    // Check ctrl 
    if (scancode == 0x1D)
        ctrlStatus = 1;
    else if (scancode == 0x9D && ctrlStatus == 1)
        ctrlStatus = 0;

    // Check shift
    if ((scancode == 0x2A || scancode == 0x36) && !shiftStatus)
        shiftStatus = 1;
    else if (scancode == 0xAA || scancode == 0xB6)
        shiftStatus = 0;

    
    // Toggle Key Statuses //
    // Check capslock
    if (!capslockStatus && scancode == 0x3A)
        capslockStatus = 1;
    else if (capslockStatus && scancode == 0x3A)
        capslockStatus = 0;

    // Combine to get either low or high
    caps = shiftStatus ^ capslockStatus;


    // Initialize the character variable // 
    character = 0;

    // Lowercase normal character
    if (!caps && !ctrlStatus && !altStatus && !(scancode & 0x80))
        character = scanset1[scancode];
    
    // Uppercase normal character
    else if (caps && !ctrlStatus && !altStatus && !(scancode & 0x80))
        character = shiftmap[scancode];
    
    // Control shortcut
    else if (ctrlStatus && !altStatus)
        character = ctlmap[scancode];


    // Keyboard mode handling //
    if (kbd_mode == 0)
        keyboardStdMode(scancode, character);
    else if (kbd_mode == 1)
        keyboardDisplayMode(scancode, character);
    else if (kbd_mode == 2) // Off mode
        return;
}


/// DRIVER INTERFACE ///
// Initializes the PS/2 keyboard
int keyboardInit(uint8_t mode){
    kbd_mode = mode;

    // Disable the ps/2 controller
    outb(KEYBOARD_STATUS_PORT, 0xAD);                           // Disable Second PS/2 Port
    outb(KEYBOARD_STATUS_PORT, 0xA7);                           // Disable Second PS/2 Port

    inb(KEYBOARD_DATA_PORT);                                    // Flush the input buffer

    // Send Keyboard Command to do a self-test(0xAA command)
    outb(KEYBOARD_STATUS_PORT, 0xAA);
    while(!(inb(KEYBOARD_DATA_PORT) == 0x55));                  // Wait until test succesfully passed

    // Check if PS/2 controller is dual channel
    outb(KEYBOARD_STATUS_PORT, 0xA8);                           // Enable 2nd PS/2 port
    // NEED TO READ BYTE AND CHECK bit 5
    outb(KEYBOARD_STATUS_PORT, 0xA7);                           // Disable second PS/2 port

    // Intialises the character buffer
    RingBufferInit(&charRingBufferStruct, BUFSIZE, charBuffer); 

    // Initialises the packet buffer
    //RingBufferInit(&packetRingBufferStruct, BUFSIZE, packetBuffer);

    // Enable first PS/2 port
    outb(KEYBOARD_COMMAND_PORT, 0xAE);                          // Enable 1st PS/2 port

    return 0;  
}

// Reads from the keyboard into the buffer, if there is an error returns -1
int keyboardBufRead(uint8_t* data, int amount){
    int i;
    int tmpVar;
    for (i = 0; i < amount; i++)
        if((tmpVar = readBuf(&charRingBufferStruct)) == -1){
            if (i == 0)
                return -1;
            else
                return i;
        }
        else 
            data[i] = tmpVar;
            

    return i;
}

// Changes the keyboard mode or gives the current mode - c - current mode, integer - switches the mode
uint8_t keyboardMode(int command){
    // Current mode
    if (command == KEYBOARD_MODE_CURRENTMODE)
        return kbd_mode;

    // Change mode
    kbd_mode = command;
    return 0;

    // Set keyboard custom mode - function pointer to the handler for it
}