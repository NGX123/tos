/*
    @author = ngx123
    @brief = code for setting up GDT
*/


#include "headers/init.h"


// Our GDT, with 3 entries, and finally our special GDT pointer
static struct gdt_entry gdt[3];
static struct gdt_ptr gp;


int hardwarePlatformInit(){
    setFlat();

    return 0;
}

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
