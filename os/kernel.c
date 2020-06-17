// File: kernel.c
// Description: this is the main file which starts all other files
// Problems:

#include "stdint.h"
#include "x86.h"
#include "vga_print.h"
#include "serial.h"
#include "gdt.h"
#include "input.h"

// Core function of the kernel that is called by bootloader
void kernel_main(){
    
    write_string(red, black, "Faro");
    init_serial();
    write_serial("faro");
    uint8_t scan_code = read_scan_code();
    write_char(red, black, scan_code);
}

