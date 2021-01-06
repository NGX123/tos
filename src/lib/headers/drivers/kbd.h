/*
  @author = ngx123
  @breif = PS/2 keyboard driver external header
*/


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
#include "types.h"


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
typedef void (*callroutine_t)(uint8_t, uint32_t);


/*
  @brief = handles the keypress interrupt by reading scancode and calling needed functions to interpret it
*/
extern void keyboard_handler();

/*
  @breif = makes the initialization for the PS/2 keyboard
  @param mode = the mode in which keyboard should work(display/standard)
  @return = 0 on success, -1 on fail
*/
extern int keyboardInit(uint8_t mode);

/*
  @brief = changes or tells current keyboard mode
  @param command = selects the action - change or print current mode
  @return = 0 or current mode on success, -1 on fail
*/
extern int keyboardMode(int command);

/*
  @brief = sets the function pointer to be called after the scancode has been interpreted
  @param callroutine_func = function pointer to be called by the keyboard_handler
  @return = 0 on success
*/
extern int keyboardCallFunc(callroutine_t callroutine_func);

/*
  @brief = reads from keyboard into the buffer
  @param buf = buffer to read to
  @param count = amount of bytes to read
  @return = amount of read bytes on success, -1 on fail
*/
extern ssize_t keyboardRead(void* buf, size_t amount);

/*
  @brief = writes to the keyboard from the buffer
  @param buf = buffer to write from
  @param count = amount of bytes to write from buffer
  @return = -1
*/
extern ssize_t keyboardWrite(void* buf, size_t amount);
#endif