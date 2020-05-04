#include "global.c"

chip8 chip;

void init(void){
    chip.pc = 0x200;

    for (int i = 0; i < 2048; ++i)
        chip.gfx[i] = 0;

    for (int i = 0; i < 16; ++i) {
        chip.stack[i] = 0;
        chip.key[i] = 0;
        chip.v[i] = 0;
    }

    for (int i = 0; i <= 4096; ++i)
        chip.memory[i] = 0;
    
       
    for (int i = 0; i < 80; i++)
        chip.memory[i] = chip8_fontset[i];

    chip.sound_timer = 0;
    chip.delay_timer = 0;
}


void cpu_cycle(void){
    chip.opcode = chip.memory[chip.pc];

    switch(chip.opcode & 0x000F)
}   