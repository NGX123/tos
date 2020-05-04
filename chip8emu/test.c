#include <stdlib.h>
#include <stdio.h>

__uint8_t memory[4096];
__uint16_t pc;
__uint16_t opcode;
__uint16_t I;
__uint16_t sp;


int load(){
    // Open ROM file
    FILE* rom = fopen("asm", "rb");
    if (rom == NULL) {
        printf("Failed to open ROM");
        return 1;
    }

    // Get file size
    fseek(rom, 0, SEEK_END);
    long rom_size = ftell(rom);
    rewind(rom);

    // Allocate memory to store rom
    char* rom_buffer = (char*) malloc(sizeof(char) * rom_size);
    if (rom_buffer == NULL) {
        printf("Failed to allocate memory for ROM");
        return 1;
    }

    // Copy ROM into buffer
    size_t result = fread(rom_buffer, sizeof(char), (size_t)rom_size, rom);
    if (result != rom_size) {
        printf("Failed to read ROM");
        return 1;
    }

    // Copy buffer to memory
    if ((4096-512) > rom_size){
        for (int i = 0; i < rom_size; ++i) {
            memory[i + 512] = (__uint8_t)rom_buffer[i];   // Load into memory starting
                                                        // at 0x200 (=512)
        }
    }
    else {
        printf("ROM too large to fit in memory");
        return 1;
    }

    // Clean up
    fclose(rom);
    free(rom_buffer);

    return 1;
}

void init(){
    pc      = 0x200;    // Set program counter to 0x200
    opcode  = 0;        // Reset op code
    I     = 0;          // Reset I
    sp      = 0;        // Reset stack pointer

    // Clear memory
    for (int i = 0; i < 4096; ++i) {
        memory[i] = 0;
    }
}




int main(){
    init();
    load();

    opcode = memory[pc] << 8 | memory[pc + 1];
    switch(opcode & 0xF000){
        case 0x0000:
            printf("case 0");
        
        case 0x1000:
            printf("case 1");
    }
}