#include "cpu.c"


int load(char *file){
    char filename[strlen(file)];
    for (long i = 0; i < strlen(file); i++){
        filename[i] = file[i];
    }


    // Open ROM file
    FILE* rom = fopen(filename, "rb");
    if (rom == NULL) {
        printf("Failed to open ROM");
        return 1;
    }
    
    // Get file size
    fseek(rom, 0, SEEK_END);
    unsigned long rom_size = ftell(rom);
    rewind(rom);

    // Allocate memory to store rom
    char* rom_buffer = (char*) malloc(sizeof(char) * rom_size);
    if (rom_buffer == NULL) {
        printf("Failed to allocate memory for ROM");
        return 1;
    }

    // Copy ROM into buffer
    size_t result = fread(rom_buffer, sizeof(char), rom_size, rom);
    if (result != rom_size) {
        printf("Failed to load ROM");
        return 1;
    }

    
    // Copy buffer to memory
    if ((4096-512) > rom_size){
        for (int i = 0; i < rom_size; ++i) {
            chip.memory[i + 512] = (__uint8_t)rom_buffer[i];                                           
        }
    }
    else {
        printf("ROM too large to fit in memory");
        return 1;
    }

    printf("Success\n");
    // Clean up
    fclose(rom);
    free(rom_buffer);

}

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




int main(int c, char *v[]){
    // init();

    // if (c != 2){
    //     printf("No rom supplied");
    //     return 1;
    // }

    // int load_rom = load(v[1]);

    // if (load_rom == 1){
    //     return 1;
    // }
    // chip.opcode = chip.memory[chip.pc];
    // printf("%x\n", chip.opcode);

    // chip.opcode = chip.memory[chip.pc] << 8 | chip.memory[chip.pc + 1];
    // printf("%x\n", chip.opcode);

    printf("%d", sizeof(unsigned short));
}