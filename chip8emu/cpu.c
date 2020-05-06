#include "global.c"

chip8 chip;

void cpu_cycle(void){
    //adds 8 zeros to the end of the first part of opcode and then changes them to second part using OR
    chip.opcode = chip.memory[chip.pc] << 8 | chip.memory[chip.pc + 1];

    //Leaves only the first number to know the instruction code 
    switch(chip.opcode & 0xF000){
        case 0x0000:
                printf("0");
        
        case 0x1000:
            chip.pc = chip.opcode & 0x0FFF;
            break;

        case 0x2000:
            chip.stack[chip.sp] = chip.pc;
            ++chip.sp;
            chip.pc = chip.opcode & 0x0FFF;
            break;

        case 0x3000:
            if (chip.v[(chip.opcode & 0x0F00) >> 8] == (chip.opcode & 0x00FF))
                chip.pc += 4;
            else
                chip.pc += 2;
            break;
           
        // case 0x4000:
        //     if (chip.v[chip.opcode & 0x0F00] != (chip.opcode & 0x00FF))
    }
}   