// File: kbd.h
// Description: includes defines for the keyboard driver



#ifndef KBD_H
#define KBD_H
#include <stdint.h>

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

// Initialises the keyboard
extern int keyboardInit(uint8_t mode);

// Reads from the keyboard buffer into buf*
extern int keyboardBufRead(uint8_t* data, int amount);
#endif