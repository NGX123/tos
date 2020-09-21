// File: kbd.h
// Description: includes defines for the keyboard driver



#ifndef KBD_H
#define KBD_H

#define KEYBOARD_MODE_CURRENTMODE -1
#define KEYBOARD_MODE_STANDARD 0
#define KEYBOARD_MODE_DISPLAY 1
#define KEYBOARD_MODE_OFF 2

#define LARROW 0x80
#define RARROW 0x81

#include <stdint.h>
#include <stddef.h>

// Structure of a keyboard packet 
struct keyPacket {
  uint32_t scancode;
  uint32_t characterForm;   // Includes the scancode form as a character from keyboard(available only if it is a character) otherwise 0
  uint32_t pressedStatus;   // The status if the key is pressed(1) or released(0)
  uint8_t shortcutStatus;   // If the keypress was a shortcut(e.g. pressed while the control key is pressed)
  uint8_t shortcutKey;      // The that was pressed together with it(e.g. ctrl) 
  uint8_t capslockStatus;   // Status of capslock - off(0), on(1)
  uint8_t specialkeyStatus; // Tells if the key is spacial and is not used in the typical keyboard function(e.g. home)
};
typedef struct keyPacket keypacket_t;
typedef void (*callroutine_t)(uint8_t, uint8_t);

// Initialises the keyboard
extern int keyboardInit(uint8_t mode);

// Changes or returns current mode
extern uint8_t keyboardMode(int command);

// Sets a function pointer to be called when keyboard interrupt is sent
extern int keyboardCallFunc(callroutine_t callroutine_func);

// Reads amount from keyboard to buf
extern int keyboardRead(void* buf, size_t amount);

// Writes amount from buf to keyboard(always fails)
extern int keyboardWrite(void* buf, size_t amount);
#endif