// File: kbd.c
// Descriptions: keyboard driver



// Includes
#include <stdint.h>
#include "stdio.h"
#include "ringbuf.h"

#include "drivers/x86.h"
#include "drivers/vga.h"
#include "drivers/kbd.h"

#define BUFSIZE 255
static keypacket_t packetBuffer[BUFSIZE];
static ring_buffer_t packetRingBufferStruct;

#define BUFSIZE 255
static uint8_t charBuffer[BUFSIZE];
static ring_buffer_t charRingBufferStruct;

static uint8_t kbd_mode = 0;

/// MODES ///
// Keyboard standard mode interpretation function
// Standard mode - arrows keys are straight passed to the display, all shortcuts are passed to system as interrupts(not made yet), normal keys are passed to the read buffer
static void keyboardStdMode(uint32_t scancode, uint8_t character, uint8_t capsStatus){
    // if (capsStatus == 0){
    //     if (scanset1[scancode] >= 32 && scanset1[scancode] <= 127)
    //         writeBuf(&charRingBufferStruct, character);
    // }
    // else 
    //     if (shiftmap[scancode] >= 32 && shiftmap[scancode] <= 127)
    //         //readBuffer[0] = shiftmap[scancode]; // change to appropriote circle buffer function 
}

// Keyboard display mode interpretation function
// Display mode - nothing is sent to the buffer and everything gets automatically displayed on the screen, shortcuts are as interrupts
static void keyboardDisplayMode(uint32_t scancode, uint8_t character){
    // Print normal characters
    if (character >= 32 && character <= 127)
        printScreen(character);

    // Print the backspace
    if (character == '\b')
        printScreen('\b');

    // Handle the arrows
    if (scancode == LARROW_SCAN)
        printScreen(LARROW);
    else if (scancode == RARROW_SCAN)
        printScreen(RARROW);
}

// Keyboard key packet mode interpretation function
// Packet mode - all keys(including shortcuts) are sent in a special keypress packet to a extra buffer
static void keyboardPacketMode(uint32_t scancode, uint8_t capslock){
    // keypacket_t scancodeStruct;

    // scancodeStruct.scancode = scancode;
    // scancodeStruct.capslockStatus = capslock;
    // //specalReadBuffer[0] = scancodeStruct;   // change to appropriote circle buffer function
}


/// INTERRUPT HANDLER ///
void keyboard_handler(){
    static uint8_t ctrlStatus = 0, altStatus = 0, shiftStatus = 0;              // Hold key statuses
    static uint8_t capslockStatus = 0, insertStatus = 0;                        // Press key statuses
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

    // Check Insert
    if (!insertStatus && scancode == 0xE52)
        insertStatus = 1;
    else if (insertStatus && scancode == 0xE52)
        insertStatus = 0;

    // Combine to get either low or high
    caps = shiftStatus ^ capslockStatus;

    // Initialize the character variable
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

    // printf("\n%d %d %d %d %d %d\n", altStatus, ctrlStatus, shiftStatus, capslockStatus, insertStatus, caps); 
    // Keyboard mode handling
    if (kbd_mode == 0)
        keyboardStdMode(scancode, character, caps);
    else if (kbd_mode == 1)
        keyboardDisplayMode(scancode, character);
    else if (kbd_mode == 2)
        keyboardPacketMode(scancode, caps);
    else if (kbd_mode == 3) // Off mode
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

    // Get config byte
    outb(KEYBOARD_COMMAND_PORT, 0x20);
    printf("%x\n", inb(KEYBOARD_DATA_PORT));

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