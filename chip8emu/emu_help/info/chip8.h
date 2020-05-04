#include <stdlib.h>
/*Emulator - emulates a hardware of a certain computer system to run the software that was 
designed for it. Emulator need to have the specifications for programms to thinck it is a cpu
of the computer system emulated. 
To write an emulator full technical detail is needed - amount of cpu opcodes(codes of operations
that are performed using the cpu) 


Specifications:
Opecodes - chip-8 has 35 opcodes with each having a size of 2 bytes
Memory - 4K memory
Registers - it has 15 8-bit registers name V0, V1... VF, 16 register is used asa carry flag
Special - it has index register I and a programm counter(PC) which have values from 0x000 to 0xFFF
Graphics - it has one instruction for drawing - XOR, pixels are turned off as result of drawing, VF is set
    the graphics of chip 8 is 2048 pixel screen (64 x 32)
Timers - chip 8 has two timer registers delay_timer and sound_timer that cound at 60Hz, when
    above 0, will count down to 0
Stack - you need to implement a stack yourself, to store the address before jump to new
Keypad - -0x0 - 0xF values to store the current state of the key


Memory Map
0x000 - 0x1FF - Chip 8 interpreter(contains font set in emu)
0x050-0x0A0 - Used for the built in 4x5 pixel font set(0-F)
0x200-0xFFF - program ROM and work ROM


WORD
To make the code compile to big endian use CFLAGS=-DWORDS_BIGENDIAN
Big endian - if there is a value FFEE, FF is bigger and on bigendian machines it would
be stored as FF - 54 location in memory, EE - 55 location in memory FFEE
Little endian - FFEE, EE - 54 memory location, FF - 55 memory location. EEFF
When storing a word it needs to be devided in two ends(1 bytes each)

*/




///*** TYPES ***///
typedef unsigned char byte;

typedef union{
    unsigned short int WORD;
    struct {
        #ifdef WORDS_BIGENDIAN
            byte high, low;
        #else
            byte low, high;
        #endif
    } BYTE;
} word;



// Memory //
int memory_init(int memorysize){
    byte *memory = (byte *)malloc(sizeof(byte) * memorysize);
    return memory != NULL;
}

inline byte memory_read(byte *memory, int address){
    return memory[address];
}

inline void memory_write(word address, byte value, byte *memory){
    memory[address.WORD] = value;
}



// CPU //
//CPU structure
typedef struct {
    byte v[0x10];      // V registers                                       
    word i;            // Index register                                   
    word pc;           // Program Counter register                          
    word oldpc;        // The last PC address                              
    word sp;           // Stack Pointer register                            
    byte dt;           // Delay Timer register                              
    byte st;           // Sound Timer register                             
    word operand;      // The current operand                               
    char *opdesc;      // A string representation of the current opcode     
    int state;         // The current state of the CPU                      
    byte rpl[0x10];    // RPL register storage                              
} chip8regset;

