#include "cpu.c"


int loadRom(char *file){
    
    // ROM filename
    char filename[strlen(file)];
    for (long i = 0; i < strlen(file); i++){
        filename[i] = file[i];
    }
    
    // Read rom
    FILE* rom = fopen(filename, "rb");
    if (rom == NULL){
        printf("Failed to read rom");
        return 1;
    }

    // Get file size
    fseek(rom, 0, SEEK_END);
    unsigned long rom_size = ftell(rom);
    rewind(rom);

    // Alocate memory for rom
    unsigned char* rom_buffer = (unsigned char *)malloc(sizeof(char) * rom_size);
    if (rom_buffer == NULL){
        printf("Failed to allocate memory");
        return 1;
    }

    // Load rom into the buffer
    size_t result = fread(rom_buffer, sizeof(char), rom_size, rom);
    if (result != rom_size){
        printf("Failed to load rom");
        return 1;
    }
    
    // Copy rom into rom
    if ((4096-512) > rom_size){
        for (int i = 0; i < rom_size; ++i) {
            chip.memory[i + 512] = (__uint8_t)rom_buffer[i];                                              
        }
    }
    else {
        printf("ROM too large to fit in memory");
        return 1;
    }

    fclose(rom);
    free(rom_buffer);

    printf("Success: loaded ROM\n");
}


// Clears everything and loads fontset
void init(void){
    // Set the initial pointer
    chip.pc = 0x200;

    // Clear display
    for (int i = 0; i < 2048; ++i)
        chip.gfx[i] = 0;

    // Clear key, register and stack
    for (int i = 0; i < 16; ++i) {
        chip.stack[i] = 0;
        chip.key[i] = 0;
        chip.v[i] = 0;
    }

    // Clear memory
    for (int i = 0; i <= 4096; ++i)
        chip.memory[i] = 0;

    //Reset timers
    chip.sound_timer = 0;
    chip.delay_timer = 0;

    // Load fontset
    for (int i = 0; i < 80; i++)
        chip.memory[i] = chip8_fontset[i];
}



int main(int argc, char *argv[]){
    // Check if ROM is supplied
    if (argc != 2){
        printf("Error: No ROM supplied");
        return 1;
    }

    init();

    // Load rom and check for if success
    int rom_status = loadRom(argv[1]);
    if (rom_status == 1){
        printf("Error: ROM can not be loaded");
        return 1;
    }

    
    cpu_cycle();
}

