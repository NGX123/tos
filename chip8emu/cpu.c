#include "global.c"

chip8 chip;

void cpu_cycle(void){
    //adds 8 zeros to the end of the first part of opcode and then changes them to second part using OR
    chip.opcode = chip.memory[chip.pc] << 8 | chip.memory[chip.pc + 1];

    printf("%d\n", chip.opcode);
    //Declarations
    unsigned short x = chip.v[chip.opcode & 0x0F00]; //x coordinate
    unsigned short y = chip.v[chip.opcode & 0x00F0]; //y coordinate
    unsigned short height = chip.opcode & 0x000F; // height 
    unsigned short pixel;

    unsigned char key_pressed = 1;
    //Leaves only the first number to know the instruction code 
    switch(chip.opcode & 0xF000){
        case 0x0000:
            //Check 0xxx codes
            switch (chip.opcode & 0x000F) {
                // 00E0 - Clear screen
                case 0x0000:
                    for (int i = 0; i < 2048; ++i) {
                        chip.gfx[i] = 0;
                    }
                    chip.drawFlag = 0;
                    chip.pc+=2;
                    break;

            // 00EE - Return from subroutine
            case 0x000E:
                --chip.sp;
                chip.pc = chip.stack[chip.sp];
                chip.pc += 2;
                break;
            }
        

        
        case 0x1000:
            chip.pc = chip.opcode & 0x0FFF;
            printf("me");
            break;
            
        case 0x2000:
            chip.stack[chip.sp] = chip.pc;
            ++chip.sp;
            chip.pc = chip.opcode & 0x0FFF;
            break;

        case 0x3000:
            if (chip.v[chip.opcode & 0x0F00] == (chip.opcode & 0x00FF))
                chip.pc += 4;
            else
                chip.pc += 2;
            break;
           
        case 0x4000:
            if (chip.v[chip.opcode & 0x0F00] != (chip.opcode & 0x00FF))
                chip.pc += 4;
            else 
                chip.pc += 2;
            printf("me");
            break;

        case 0x5000:
            if (chip.v[chip.opcode & 0x0F00] == chip.v[chip.opcode & 0x00F0])
                chip.pc += 4;
            else
                chip.pc += 2;
            printf("me");
            break;
        
        case 0x6000:
            chip.v[chip.opcode & 0x0F00] = chip.opcode & 0x00FF;
            chip.pc += 2;
            break;

        case 0x7000:
            chip.v[chip.opcode & 0x0F00] += chip.opcode & 0x00FF;
            chip.pc += 2;
            break;

        case 0x8000:
            switch(chip.opcode & 0x000F){
                case 0x0000:
                    chip.v[chip.opcode & 0x0F00] = chip.v[chip.opcode & 0x00F0];
                    chip.pc += 2;
                    break;
                
                case 0x0001:
                    chip.v[chip.opcode & 0x0F00] |= chip.v[chip.opcode & 0x00F0];
                    chip.pc += 2;
                    break;
                
                case 0x0002:
                    chip.v[chip.opcode & 0x0F00] &= chip.v[chip.opcode & 0x00F0];
                    chip.pc += 2;
                    break;

                case 0x0003:
                    chip.v[chip.opcode & 0x0F00] ^= chip.v[chip.opcode & 0x00F0];
                    chip.pc += 2;
                    break;

                //Check how 0x0004 - 0x000E work
                case 0x0004:
                    chip.v[chip.opcode & 0x0F00] += chip.v[chip.opcode & 0x00F0];
                     if(chip.v[chip.opcode & 0x00F0] > (0xFF - chip.v[chip.opcode & 0x0F0]))
                        chip.v[0xF] = 1; //carry
                    else
                        chip.v[0xF] = 0;
                    chip.pc += 2;
                    break;

                case 0x0005:
                    if(chip.v[chip.opcode & 0x00F0] > (0xFF - chip.v[chip.opcode & 0x0F0]))
                        chip.v[0xF] = 1; //carry
                    else
                        chip.v[0xF] = 0;
                    chip.v[chip.opcode & 0x0F00] += chip.v[chip.opcode & 0x00F0];
                    chip.pc += 2;
                    break;

                case 0x0006:
                    chip.v[0xF] = chip.v[chip.opcode & 0x0F00] & 0x1;
                    chip.v[chip.opcode & 0x0F00] >>= 1;
                    chip.pc += 2;
                    break;

                case 0x0007:
                    if(chip.v[chip.opcode & 0x0F00] > chip.v[chip.opcode & 0x00F0])
                        chip.v[0xF] = 0;
                    else 
                        chip.v[0xF] = 0;
                    
                case 0x000E:
                    chip.v[0xF] = chip.v[chip.opcode & 0x0F00] >> 7;
                    chip.v[chip.opcode & 0x0F00] <<= 1;
                    chip.pc += 2;
                    break;

                default:
                    printf("\nUnknown opcode");
                    exit(3);
        }   

        case 0x9000:
            if (chip.v[chip.opcode & 0x0F00] != chip.v[chip.opcode & 0x00F0])
                chip.pc += 4;
            else 
                chip.pc += 2;
            break;
        
        case 0xA000:
            chip.i = chip.opcode & 0x0FFF;
            chip.pc += 2;
            break;
        
        case 0xB000:
            chip.pc = (chip.opcode & 0x0FFF) + chip.v[0];
            break;

        case 0xC000:
            chip.v[chip.opcode & 0x0F00] = (rand() % (0xFF +1)) & (chip.opcode & 0x00FF);
            chip.pc += 2;
            break;

        case 0xD000:
            chip.v[0xF] = 0;
            for (int yline = 0; yline < height; yline++){
                pixel = chip.memory[chip.i + yline];
                for (int xline = 0; xline < 8; xline++){
                    if ((pixel & (0x80 >> xline)) != 0){
                        if (chip.gfx[(x + xline + ((y + yline) * 64))] == 1)
                            chip.v[0xF] = 1;
                        chip.gfx[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                }
            }

            chip.drawFlag = 0;
            chip.pc += 2;

        case 0xE000:
            switch(chip.opcode & 0x00FF){
                case 0x009E:
                    if (chip.key[chip.v[chip.opcode & 0x0F00]] != 0)
                        chip.pc += 4;
                    else
                        chip.pc += 2;
                    break;

                case 0x00A1:
                    if (chip.key[chip.v[chip.opcode & 0x0F00]] == 0)
                        chip.pc += 4;
                    else 
                        chip.pc += 2;
                    break;
            }

        case 0xF000:
            switch(chip.opcode & 0x00FF){
                case 0x0007:
                    chip.v[chip.opcode & 0x0F00] == chip.delay_timer;
                    chip.pc += 2;
                    break;
                
                case 0x000A:
                    for (int i = 0; i < 16; ++i){
                        if (chip.key[i] != 0){
                            chip.v[chip.opcode & 0x0F00] = i;
                            key_pressed = 0;
                        }
                    }

                    if(key_pressed == 0)
                        return;

                    chip.pc += 2;

                    break;

                case 0x0015:
                    chip.delay_timer = chip.v[chip.opcode & 0x0F00];
                    chip.pc += 2;
                    break;

                case 0x0018:
                    chip.sound_timer = chip.v[chip.opcode & 0x0F00];
                    chip.pc += 2;
                    break;
                
                case 0x001E:
                    if(chip.i + chip.v[chip.opcode & 0x0F00] > 0xFFF)
                        chip.v[0xF] = 1;
                    else
                        chip.v[0xF] = 0;

                    chip.i += chip.v[chip.opcode & 0x0F00];
                    chip.pc += 2;
                    break;

                case 0x0029:
                    chip.i = chip.v[chip.opcode & 0x0F00] * 0x5;
                    chip.pc += 2;
                    break;

                case 0x0033:
                    chip.memory[chip.i] = chip.v[chip.opcode & 0x0F00] / 100;
                    chip.memory[chip.i + 1] = (chip.v[chip.opcode & 0x0F00] / 10) % 10;
                    chip.memory[chip.i + 1] = chip.v[chip.opcode & 0x0F00] % 10;
                    chip.pc += 2;
                    break;

                case 0x0055:
                    for (int i = 0; chip.i <= ((chip.opcode & 0x0F00) >> 8); ++i)
                        chip.memory[chip.i + i] = chip.v[i];

                    chip.i += ((chip.opcode & 0x0F00) >> 8) + 1;
                    chip.pc += 2;
                    break;

                case 0x0065:
                    for (int i = 0; i <= ((chip.opcode & 0x0F00) >> 8); ++i)
                        chip.v[i] = chip.memory[chip.i + i];

                    chip.i += ((chip.opcode & 0x0F00) >> 8) + 1;
                    chip.pc += 2;
                    break;

                default:
                    printf("Unknown opcode\n");
            }
            break;

        default:
            printf("Unimplemented opcode");
            exit(3);
    }       

    if (chip.delay_timer > 0)
        --chip.delay_timer;

    if (chip.sound_timer > 0)
        if (chip.sound_timer == 1);
        --chip.sound_timer;
}  


