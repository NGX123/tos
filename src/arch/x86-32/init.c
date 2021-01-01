// File: init.c
// Description: includes defenitions for loading the global descriptor table
// WHEN CHANGING TO LONG MODE CHANGE THE "uint32_t" in the adresses to "uint64_t"

#include "headers/init.h"

// Our GDT, with 3 entries, and finally our special GDT pointer
struct gdt_entry gdt[3];
struct gdt_ptr gp;

// Makes platform specific initializations
int hardwarePlatformInit(){
    setFlat();

    return 0;
}

// Initializes the flat memory model
static void setFlat(){
    for (int i = 0; i < 3; i++){
        gdt[i].limit_low = 0xffff;
        gdt[i].base_low = 0x0000;
        gdt[i].base_middle = 0x00;
        gdt[i].access = 0x9A;
        gdt[i].granularity = 0xCF;
        gdt[i].base_high = 0x00;

        if (i == 2)
            gdt[i].access = 0x92;

        if (i == 0){
            gdt[i].limit_low = 0x0000;
            gdt[i].base_low = 0x0000;
            gdt[i].base_middle = 0x00;
            gdt[i].access = 0x00;
            gdt[i].granularity = 0x00;
            gdt[i].base_high = 0x00;
        }
    }

    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (void*)gdt;
    load_gdt((void*)&gp);
}
