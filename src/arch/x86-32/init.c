/*
    @author = ngx123
    @brief = code for setting up GDT
*/


#include "include/init.h"


static struct gdt_entry gdt[3]; // Array is used to hold all of the GDT entries as they go one after another
static struct gdt_ptr gp;
static struct kernInfo kernInfo_struct;


void* getBootInfo()
{
    return &kernInfo_struct;
}

void setBootInfo(int var_num, ...)
{
    va_list valist;
    va_start(valist, var_num);

    void* headerPtr;

    if (va_arg(valist, uint32_t) == MULTIBOOT_HEADER_MAGIC){
        kernInfo_struct.protocol = PROTOCOL_MULTIBOOT;
        kernInfo_struct.boot_protocol_struct_ptr = va_arg(valist, void*);
        kernInfo_struct.boot_protocol_struct_end_ptr = headerPtr + sizeof(multiboot_info_t);
    }

    va_end(valist);
}

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
    gdtLoadAsm((void*)&gp);
}
