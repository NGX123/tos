// File: kbd.h
// Description: includes defines for the keyboard driver



#ifndef KBD_H
#define KBD_H
#include <stdint.h>

// Defines
#define KEYBOARD_STATUS_PORT  0x64
#define KEYBOARD_COMMAND_PORT 0x64
#define KEYBOARD_DATA_PORT    0x60
#define KBS_DIB               0x01    // kbd data in buffer

#define NO 0x0
#define ESC 0x01
#define CONTROL 0x1D
#define ALT 0x38
#define CAPSLOCK 0x3A
#define LARROW_SCAN 0xE4B
#define RARROW_SCAN 0xE4D

#define C(x) (x - '@')
#define LARROW 0x80
#define RARROW 0x81

// Scanset 1 keymap
static uint8_t scanset1[] =
{
  NO,   0x1B, '1',  '2',  '3',  '4',  '5',  '6',  // 0x00
  '7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',
  'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  // 0x10
  'o',  'p',  '[',  ']',  '\n', NO,   'a',  's',
  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  // 0x20
  '\'', '`',  NO,   '\\', 'z',  'x',  'c',  'v',
  'b',  'n',  'm',  ',',  '.',  '/',  NO,   '*',  // 0x30
  NO,   ' ',  NO,   NO,   NO,   NO,   NO,   NO,
  NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',  // 0x40
  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO,   // 0x50
  [0x9C] '\n',      // KP_Enter
  [0xB5] '/'       // KP_Div
};

// Character map when shift is pressed 
static uint8_t shiftmap[256] =
{
  NO,   033,  '!',  '@',  '#',  '$',  '%',  '^',  // 0x00
  '&',  '*',  '(',  ')',  '_',  '+',  '\b', '\t',
  'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  // 0x10
  'O',  'P',  '{',  '}',  '\n', NO,   'A',  'S',
  'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  // 0x20
  '"',  '~',  NO,   '|',  'Z',  'X',  'C',  'V',
  'B',  'N',  'M',  '<',  '>',  '?',  NO,   '*',  // 0x30
  NO,   ' ',  NO,   NO,   NO,   NO,   NO,   NO,
  NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',  // 0x40
  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO,   // 0x50
  [0x9C] '\n',      // KP_Enter
  [0xB5] '/'        // KP_Div
};

// Character map when control is pressed
static uint8_t ctlmap[256] =
{
  NO,      NO,      NO,      NO,      NO,      NO,      NO,      NO,
  NO,      NO,      NO,      NO,      NO,      NO,      NO,      NO,
  C('Q'),  C('W'),  C('E'),  C('R'),  C('T'),  C('Y'),  C('U'),  C('I'),
  C('O'),  C('P'),  NO,      NO,      '\r',    NO,      C('A'),  C('S'),
  C('D'),  C('F'),  C('G'),  C('H'),  C('J'),  C('K'),  C('L'),  NO,
  NO,      NO,      NO,      C('\\'), C('Z'),  C('X'),  C('C'),  C('V'),
  C('B'),  C('N'),  C('M'),  NO,      NO,      C('/'),  NO,      NO,
  [0x9C] '\r',      // KP_Enter
  [0xB5] C('/')     // KP_Div
};

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
#endif