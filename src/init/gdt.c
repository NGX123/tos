// File: gdt.c
// Description: includes defenitions for loading the global descriptor table


#include <stdint.h>
extern void load_gdt(uint64_t);

// A gdt entry to be loaded using pointer
struct gdt_entry{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

// Pointer to the GDT, The max bytes taken up by the GDT, minus 1
struct gdt_ptr{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

// Our GDT, with 3 entries, and finally our special GDT pointer
struct gdt_entry gdt[3];
struct gdt_ptr gp;


void setFlat(){
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
    gp.base = (uint64_t)gdt;
    load_gdt((uint64_t)&gp);
}

