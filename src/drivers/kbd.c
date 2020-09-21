// File: kbd.c
// Descriptions: keyboard driver



// Includes
#include <stdint.h>
#include <stddef.h>
#include "stdio.h"
#include "ringbuf.h"
#include "headers/kbd-defines.h"

#include "drivers/x86.h"
#include "drivers/vga.h"
#include "drivers/kbd.h"

static uint8_t charBuffer[BUFSIZE];
static ring_buffer_t charRingBufferStruct;

static uint8_t kbd_mode = 0;
static callroutine_t callroutine = 0;

/// INTERRUPT HANDLER FUNCTIONS ///
// Returns a status byte for all toggle and hold keys
static uint8_t getButtonStatuses(uint32_t scancode, uint8_t buttonStatuses){
    // Hold Key Statuses //
    // Check ctrl 
    if ((scancode == 0x1D      || scancode == 0x1DE0) && !(buttonStatuses & SHORTCUT_CTRL))
        buttonStatuses ^= SHORTCUT_CTRL;
    else if ((scancode == 0x9D || scancode == 0x9DE0) &&  (buttonStatuses & SHORTCUT_CTRL))
        buttonStatuses ^= SHORTCUT_CTRL;

    // Check alt 
    if ((scancode == 0x38      || scancode == 0x38E0) && !(buttonStatuses & SHORTCUT_ALT))
        buttonStatuses ^= SHORTCUT_ALT;
    else if ((scancode == 0xB8 || scancode == 0xB8E0) &&  (buttonStatuses & SHORTCUT_ALT))
        buttonStatuses ^= SHORTCUT_ALT;

    // Check shift
    if ((scancode == 0x2A      || scancode == 0x36)  && !(buttonStatuses & SHORTCUT_SHIFT))
        buttonStatuses ^= (SHORTCUT_SHIFT | KEYBYTE_CAPS);
    else if ((scancode == 0xAA || scancode == 0xB6)  &&  (buttonStatuses & SHORTCUT_SHIFT))
        buttonStatuses ^= (SHORTCUT_SHIFT | KEYBYTE_CAPS);

    // Check capslock
    if (scancode == 0x3A      && !(buttonStatuses & SHORTCUT_CAPSLOCK))
        buttonStatuses ^= (SHORTCUT_CAPSLOCK | KEYBYTE_CAPS);
    else if (scancode == 0x3A &&  (buttonStatuses & SHORTCUT_CAPSLOCK))
        buttonStatuses ^= (SHORTCUT_CAPSLOCK | KEYBYTE_CAPS);

    return buttonStatuses;
}

// Gets the character from the scancode
static uint8_t getCharacter(uint32_t scancode, uint8_t buttonStatuses){
    // Initialize the character variable
    uint8_t character = 0;

    // Lowercase normal character
    if (!(buttonStatuses & (SHORTCUT_CTRL | SHORTCUT_ALT | KEYBYTE_CAPS)) && !(scancode & 0x80))
        character = scanset1[scancode];
    
    // Uppercase normal character
    else if ((buttonStatuses & (KEYBYTE_CAPS)) && !(buttonStatuses & (SHORTCUT_CTRL | SHORTCUT_ALT)) && !(scancode & 0x80))
        character = shiftmap[scancode];
    
    // Control shortcut
    else if ((buttonStatuses & SHORTCUT_CTRL) && !(buttonStatuses & SHORTCUT_ALT))
        character = ctlmap[scancode];

    return character;
}

/// MODES ///
// Keyboard standard mode interpretation function
// Standard mode - arrows keys are straight passed to the display, all shortcuts are passed to system as interrupts(not made yet), normal keys are passed to the read buffer
static void keyboardStdMode(uint32_t scancode, uint8_t character){
    // Write normal characters to the buffer
    if (character >= 32 && character <= 127 && scancode < 0x80)
        writeBuf(&charRingBufferStruct, character);

    // Escape sequences
    else if (character == '\b' || character == '\t' || character == '\n') 
        writeBuf(&charRingBufferStruct, character);

    // Arrows
    else if (scancode == LARROW_SCAN)
        writeBuf(&charRingBufferStruct, LARROW);
    else if (scancode == RARROW_SCAN)
        writeBuf(&charRingBufferStruct, RARROW);
}

// Keyboard display mode interpretation function
// Display mode - nothing is sent to the buffer and everything gets automatically displayed on the screen, shortcuts are as interrupts
static void keyboardDisplayMode(uint32_t scancode, uint8_t character){
    // Print normal characters
    if (character >= 32 && character <= 127 && scancode < 0x80)
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
    static uint8_t buttonStatuses = 0;                                                           // Toggle/Hold button statuses
    uint32_t keyboardStatus, scancode;                                                           // Data from ports
    uint8_t character, scancode_byte2;                                           

    // Get the keyboard input
    if ((keyboardStatus = inb(KEYBOARD_STATUS_PORT)) & KBS_DIB)
        scancode = inb(KEYBOARD_DATA_PORT);
    else 
        return;

    // Handle E0
    if (scancode == 0xE0){
        scancode_byte2 = inb(KEYBOARD_DATA_PORT);
        scancode = scancode | (scancode_byte2 << 8);
    }

    // Initialize the variables
    buttonStatuses = getButtonStatuses(scancode, buttonStatuses);
    character = getCharacter(scancode, buttonStatuses);


    // Keyboard mode handling //
    if (kbd_mode == 0)
        keyboardStdMode(scancode, character);
    else if (kbd_mode == 1)
        keyboardDisplayMode(scancode, character);
    else if (kbd_mode == 2) // Off mode
        return;
    if (callroutine != 0){
        if (scancode != RARROW_SCAN || scancode != LARROW_SCAN)
            callroutine(character, buttonStatuses);
        else 
            callroutine(scancode == RARROW_SCAN ? RARROW : LARROW, buttonStatuses);
    }
}


/// IOCTL FUNCTIONS ///
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

    // Enable first PS/2 port
    outb(KEYBOARD_COMMAND_PORT, 0xAE);                          // Enable 1st PS/2 port

    return 0;  
}

// Changes or tells the current keyboard mode
uint8_t keyboardMode(int command){
    // Current mode
    if (command == KEYBOARD_MODE_CURRENTMODE)
        return kbd_mode;

    // Change mode
    if (command < 3){
        kbd_mode = command;
        return 0;
    }
    
    return -1;
}

// Sets a function pointer to be called when keyboard interrupt is sent
int keyboardCallFunc(callroutine_t callroutine_func){
    callroutine = callroutine_func;
    return 0;
}


/// FILE OPERATIONS ///
// Reads from the keyboard into the buffer, if there is an error returns -1
int keyboardRead(void* buf, size_t count){
    size_t i;
    int tmpVar;

    for (i = 0; i < count; i++)
        if((tmpVar = readBuf(&charRingBufferStruct)) == -1){
            if (i == 0)
                return -1;
        }
        else 
            ((uint8_t*)buf)[i] = (uint8_t)tmpVar;

    return i;
}

int keyboardWrite(void* buf, size_t count){
    int tempvar;
    tempvar = ((uint8_t*)buf)[count-1];
    
    return (tempvar * 0 - 1);
}