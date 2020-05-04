#include "cpu.c"

int load_rom(const char *file_path){
    FILE* rom = fopen(file_path, "rb");
    if (rom == NULL){
        printf("Loading failed");
        return 1;
    }

    //Size of file
    fseek(rom, 0, SEEK_END); //go to the end of the file
    long rom_size = ftell(rom); //tell the size
    rewind(rom); //go to the start of the file

    //Allocate memory
    char *buffer = (char*)malloc(sizeof(char) * rom_size);
    if (buffer == NULL){
        printf("Allocation failed");
        return 1;
    }

    //Copy rom into memory
    if ((4096 - 512) > rom_size){
        for (int i = 0; i < rom_size; ++i){
            chip.memory[i + 512] = (__uint8_t)buffer[i];
        }
    }
    else{
        printf("ROM too large");
        return 1;
    }

    fclose(rom);
}

int main(){
    
}

