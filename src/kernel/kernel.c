// File: kernel.c
// Description: this is the main file which starts all other files
// Problems:



#include "kernel.h"
#include "x86.h"

// Core function of the kernel that is called by bootloader
void kernel_main(){
    clear();
    enable_cursor(0, 15);
    printk("Initialised: Screen\n");
    
    init_serial();
    write_serial("Serial test run");
    printk("Initialised: Serial\n");

    setFlat();
    
    idt_init();

    
}

