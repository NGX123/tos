/*
It is easy to work with incstructions in chip 8 becuase each is exactly 16 bit long and 
    everything needed for the instruction is stored there, first number is instruction
    code and other 3 are the opernands 



// USING INSTRUCTIONS // 
To recognize the instructions and opernands bit masks(check or change bit status) and 
bit shifting(when bits are moved in a certain size) is used

Shifting and Masking
If we have a number 0x23 and we want to get the hex value of the first one, we first AND it
with 0xF0(1111 0000) to get the value of all bits, result is 0x20(0010 0000). Next we want to 
move all bits 4 places to the left using >> and we get 0x02(0000 0010)

Example
If we have an insturction 8st0(moves value of register s into register t). The instruction 
would be 0x8620. 
1 we get the value of "s" - $08620 & $0F00 = $0600.     BIN = 1000 0110 0010 0000 & 0000 1111 0000 0000 = 0000 0110 0000 0000
2 we need to shift 8 places to left - $0600 >> 8 = $0006    BIN = 0000 0110 0000 0000 >> 8 = 0000 0000 0000 0110
3 4 do the same operations with other register 

// EXAMPLE INSTRUCTIONS //
JUMP - moves programm pointer to an address, during next cpu cycle the instruction will be
    fetched from there
Encoding - 1nnn, 1 is instruction code and n is a 4 bit number, together the 4 bit
    numbers form a 12 bit adress for the instruction to go to
Example - 0x1234 - next cpu cycle instruction will be loaded from 0x234


LOAD - tells the cpu to load a number into a register
Encoding - 6snn. 6 is instruction code, s is 4bit value that specifies the register, 
    n are 4bit characters that together make an 8bit value for loading into register
Example - 0x6248 - 0x48 will be loaded into register 2



// FULL INSTRUCTION SET //
00E0 - Clear the screen
00EE - Return from subroutine
1nnn - Jump to address nnn
2nnn - Call routine at address nnn
3snn - Skip next instruction if register s value equals nn
4snn - Do not skip next instruction if register s value equals nn
5st0 - Skip if register s value equals register t value
6snn - Load register s with value nn
7snn - Add value nn to register s
8st0 - Move value from register s to register t
8st1 - Perform logical OR on register s and t and store in t
8st2 - Perform logical AND on register s and t and store in t
8st3 - Perform logical XOR on register s and t and store in t
8st4 - Add s to t and store in s - register F set on carry
8st5 - Subtract s from t and store in s - register F set on !borrow
8s06 - Shift bits in register s 1 bit to the right - bit 0 shifts to register F
8s0E - Shift bits in register s 1 bit to the left - bit 7 shifts to register F
9st0 - Skip next instruction if register s not equal register t
Annn - Load index with value nnn
Bnnn - Jump to address nnn + index
Ctnn - Generate random number between 0 and nn and store in t
Dstn - Draw n byte sprite at x location reg s, y location reg t
Ft07 - Move delay timer value into register t
Ft0A - Wait for keypress and store in register t
Fs15 - Load delay timer with value in register s
Fs18 - Load sound timer with value in register s
Fs1E - Add value in register s to index
Fs29 - Load index with sprite from register s
Fs33 - Store the binary coded decimal value of register s at index
Fs55 - Store the values of register s registers at index
Fs65 - Read back the stored values at index into registers
*/