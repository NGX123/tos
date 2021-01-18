/*
    @author = ngx123
    @brief = code for setting up GDT
*/


#include "include/init.h"


static struct gdt_entry gdt[3]; // Array is used to hold all of the GDT entries as they go one after another
static struct gdt_ptr gp;


int hardwarePlatformInit()
{
    makeMemFlat();

    return 0;
}

static void makeMemFlat()
{
    for (int i = 0; i < 3; i++)
    {
        gdt[i].limit_low = 0xffff;
        gdt[i].base_low = 0x0000;
        gdt[i].base_middle = 0x00;
        gdt[i].access = 0x9A;
        gdt[i].granularity = 0xCF;
        gdt[i].base_high = 0x00;

        if (i == 2)
            gdt[i].access = 0x92;

        if (i == 0)
        {
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
