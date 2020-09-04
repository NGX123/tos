// File: kbd.c
// Descriptions: keyboard driver



// Includes
#include "x86.h"
#include "driver.h"
#include <stdint.h>
#include "kbd.h"
#include "acpi.h"
#include "stdio.h"

keypacket_t specalReadBuffer[255];
uint8_t readBuffer[255];
uint8_t writeBuffer[20];
static uint8_t kbd_mode = 0;

// Initializes the PS/2 keyboard
int keyboardInit(struct FADT* FADTstruct, uint8_t mode){
    kbd_mode = mode;

    // Check if PS/2 controller exists
    // if (FADTstruct->BootArchitectureFlags & 0x02)
    //     ;
    // else 
    //     return -1;

    // Disable the ps/2 controller
    outb(KEYBOARD_STATUS_PORT, 0xAD);      
    outb(KEYBOARD_STATUS_PORT, 0xA7);

    inb(KEYBOARD_DATA_PORT); // Flush the input buffer

    // Send Keyboard Command to do a self-test(0xAA command)
    outb(KEYBOARD_STATUS_PORT, 0xAA);
    while(!(inb(KEYBOARD_DATA_PORT) == 0x55));

    // Check if PS/2 controller is dual channel
    outb(KEYBOARD_STATUS_PORT, 0xA8);                           // Enable 2nd channel
    printf("\nConfig byte - %x\n", inb(KEYBOARD_STATUS_PORT));  // NEED TO READ BYTE AND CHECK bit 5
    outb(KEYBOARD_STATUS_PORT, 0xA7);                           // Disable second PS/2 port

    // Enable first PS/2 port
    outb(KEYBOARD_STATUS_PORT, 0xAB);

    return 0;
    
}

// Keyboard standard mode interpretation function
// Standard mode - arrows keys are straight passed to the display, all shortcuts are passed to system as interrupts(not made yet), normal keys are passed to the read buffer
static void keyboardStdMode(uint32_t scancode, uint8_t capsStatus){
    if (capsStatus == 0){
        if (scanset1[scancode] >= 32 && scanset1[scancode] <= 127)
            readBuffer[0] = scanset1[scancode]; // change to appropriote circle buffer function
    }
    else 
        if (shiftmap[scancode] >= 32 && shiftmap[scancode] <= 127)
            readBuffer[0] = shiftmap[scancode]; // change to appropriote circle buffer function 
}

// Keyboard display mode interpretation function
// Display mode - nothing is sent to the buffer and everything gets automatically displayed on the screen, shortcuts are as interrupts
static void keyboardDisplayMode(uint32_t scancode, uint8_t capsStatus){
    if (scancode & 0x80)
        return;
    if (capsStatus == 0){
        if (scanset1[scancode] >= 32 && scanset1[scancode] <= 127)
            printScreen(scanset1[scancode]);
    }
    else 
        if (shiftmap[scancode] >= 32 && shiftmap[scancode] <= 127)
            printScreen(scanset1[scancode]); 

    // Handle the arrows
    if (scancode == LARROW_SCAN)
        printScreen(LARROW);
    else if (scancode == RARROW_SCAN)
        printScreen(RARROW);
}

// Keyboard key packet mode interpretation function
// Packet mode - all keys(including shortcuts) are sent in a special keypress packet to a extra buffer
static void keyboardPacketMode(uint32_t scancode, uint8_t capslock){
    keypacket_t scancodeStruct;

    scancodeStruct.scancode = scancode;
    scancodeStruct.capslockStatus = capslock;
    specalReadBuffer[0] = scancodeStruct;   // change to appropriote circle buffer function
}

void keyboard_handler(){
    static uint8_t ctrlStatus = 0, altStatus = 0, shiftStatus = 0;              // Hold key statuses
    static uint8_t capslockStatus = 0, insertStatus = 0;                        // Press key statuses
    static uint8_t caps = 0;                                                    // Status of keys being lower case or higher case
    uint32_t keyboardStatus, scancode;                                          // Data from keyboard ports

    if ((keyboardStatus = inb(KEYBOARD_STATUS_PORT)) & KBS_DIB)
        return;

    scancode = inb(KEYBOARD_DATA_PORT);


    // Hold Key Statuses //
    // Check alt 
    if (scancode == 0x38)
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

    
    // Press Key Statuses //
    // Check capslock
    if (!capslockStatus && scancode == 0x3A)
        capslockStatus = 1;
    else if (capslockStatus && scancode == 0x3A)
        capslockStatus = 0;

    // Check Insert


    // Combine to get either low or high
    caps = shiftStatus ^ capslockStatus;
    

    // Keyboard mode handling
    if (kbd_mode == 0)
        keyboardStdMode(scancode, caps);
    else if (kbd_mode == 1)
        keyboardDisplayMode(scancode, caps);
    else if (kbd_mode == 2)
        keyboardPacketMode(scancode, caps);
    else if (kbd_mode == 3) // Off mode
        return;
}

