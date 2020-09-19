// File: kbd-defines.h
// Description: internal to the keyboard driver header





#ifndef KBD_DEFINES_H
#define KBD_DEFINES_H
#include <stdint.h>

// Defines
#define KEYBOARD_STATUS_PORT  0x64
#define KEYBOARD_COMMAND_PORT 0x64
#define KEYBOARD_DATA_PORT    0x60
#define KBS_DIB               0x01    // kbd data in buffer

#define NO 0x0
#define ESC 0x01
#define LARROW_SCAN 0xE4B
#define RARROW_SCAN 0xE4D

#define LARROW    0x80
#define RARROW    0x81
#define CTRL      0x82
#define SHIFT     0x83
#define ALT       0x84
#define CAPS      0x85

#define SHORTCUT_CTRL      0x1
#define SHORTCUT_ALT       0x2
#define SHORTCUT_SHIFT     0x4
#define SHORTCUT_CAPSLOCK  0x8
#define KEYBYTE_CAPS       0x10

#define BUFSIZE 255

#define C(x) (x - '@')

// Scanset 1 keymap
static uint8_t scanset1[] =
{
  NO,   0x1B, '1',  '2',  '3',  '4',  '5',  '6',  // 0x00
  '7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',
  'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  // 0x10
  'o',  'p',  '[',  ']',  '\n', CTRL, 'a',  's',
  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  // 0x20
  '\'', '`',  SHIFT,'\\', 'z',  'x',  'c',  'v',
  'b',  'n',  'm',  ',',  '.',  '/',  SHIFT,'*',  // 0x30
  ALT,   ' ',  CAPS,NO,   NO,   NO,   NO,   NO,
  NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',  // 0x40
  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO,   // 0x50
  [0x9C] '\n',      // KP_Enter
  [0xB5] '/'        // KP_Div
};

// Character map when shift is pressed 
static uint8_t shiftmap[] =
{
  NO,   033,  '!',  '@',  '#',  '$',  '%',  '^',  // 0x00
  '&',  '*',  '(',  ')',  '_',  '+',  '\b', '\t',
  'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  // 0x10
  'O',  'P',  '{',  '}',  '\n', CTRL,   'A','S',
  'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  // 0x20
  '"',  '~',  SHIFT,'|',  'Z',  'X',  'C',  'V',
  'B',  'N',  'M',  '<',  '>',  '?',  SHIFT,'*',  // 0x30
  ALT,   ' ',  CAPS, NO,   NO,   NO,   NO,   NO,
  NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',  // 0x40
  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO,   // 0x50
  [0x9C] '\n',      // KP_Enter
  [0xB5] '/'        // KP_Div
};

// Character map when control is pressed
static uint8_t ctlmap[] =
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
#endif