/*
  @author = ngx123
  @brief = PS/2 keyboard driver internal header
*/


#ifndef KBD_DEFINES_H
#define KBD_DEFINES_H


#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <ringbuf.h>
#include <types.h>

#include <drivers/x86.h>
#include <drivers/vga.h>
#include <drivers/kbd.h>
#include <interrupts.h>


#define KEYBOARD_STATUS_PORT  0x64
#define KEYBOARD_COMMAND_PORT 0x64
#define KEYBOARD_DATA_PORT    0x60
#define KBS_DIB               0x01

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


static uint8_t scanset1[] =
{
  NO,   0x1B, '1',  '2',  '3',  '4',  '5',  '6',
  '7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',
  'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',
  'o',  'p',  '[',  ']',  '\n', CTRL, 'a',  's',
  'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',
  '\'', '`',  SHIFT,'\\', 'z',  'x',  'c',  'v',
  'b',  'n',  'm',  ',',  '.',  '/',  SHIFT,'*',
  ALT,   ' ',  CAPS,NO,   NO,   NO,   NO,   NO,
  NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',
  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO,
  [0x9C] = '\n',
  [0xB5] = '/'
};

static uint8_t shiftmap[] =
{
  NO,   033,  '!',  '@',  '#',  '$',  '%',  '^',
  '&',  '*',  '(',  ')',  '_',  '+',  '\b', '\t',
  'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',
  'O',  'P',  '{',  '}',  '\n', CTRL,   'A','S',
  'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',
  '"',  '~',  SHIFT,'|',  'Z',  'X',  'C',  'V',
  'B',  'N',  'M',  '<',  '>',  '?',  SHIFT,'*',
  ALT,   ' ',  CAPS, NO,   NO,   NO,   NO,   NO,
  NO,   NO,   NO,   NO,   NO,   NO,   NO,   '7',
  '8',  '9',  '-',  '4',  '5',  '6',  '+',  '1',
  '2',  '3',  '0',  '.',  NO,   NO,   NO,   NO,
  [0x9C] = '\n',
  [0xB5] = '/'
};

// static uint8_t ctlmap[] =
// {
//   NO,      NO,      NO,      NO,      NO,      NO,      NO,      NO,
//   NO,      NO,      NO,      NO,      NO,      NO,      NO,      NO,
//   C('Q'),  C('W'),  C('E'),  C('R'),  C('T'),  C('Y'),  C('U'),  C('I'),
//   C('O'),  C('P'),  NO,      NO,      '\r',    NO,      C('A'),  C('S'),
//   C('D'),  C('F'),  C('G'),  C('H'),  C('J'),  C('K'),  C('L'),  NO,
//   NO,      NO,      NO,      C('\\'), C('Z'),  C('X'),  C('C'),  C('V'),
//   C('B'),  C('N'),  C('M'),  NO,      NO,      C('/'),  NO,      NO,
//   [0x9C] = '\r',
//   [0xB5] = C('/')
// };


/*
  @brief = returns the status(held/released) of held keys but before changes the state depending on scancode argument
  @param scancode = the key whos status should be changed to opposite
  @param buttonStatuses = the buffer to hold the status byte where key states are changed
  @return = 0 on success
*/
static int getButtonStatuses(uint32_t scancode, uint8_t* buttonStatuses);

/*
  @brief = interprets scancode and button statuses to produce an ASCII character instead of a scancode
  @param scancode = the scancode that should be converted to character
  @param buttonStatuses = current hold button statuses
  @return = the scancode changed to character
*/
static uint8_t getCharacter(uint32_t scancode, uint8_t buttonStatuses);

/*
  @brief = handles the keyboard standard mode - arrows keys are straight passed to the display, all shortcuts are passed to system as interrupts(not made yet), normal keys are passed to the read buffer
  @param scancode = scancode to perform checks and interpret before doing actions with character
  @param character = the character to be used to output
*/
static void keyboardStdMode(uint32_t scancode, uint8_t character);

/*
  @brief = handles the keyboard display mode - nothing is sent to the buffer and everything gets automatically displayed on the screen, shortcuts are as interrupts
  @param scancode = scancode to perform checks and interpret before doing actions with character
  @param character = the character to be used to output
*/
static void keyboardDisplayMode(uint32_t scancode, uint8_t character);
#endif