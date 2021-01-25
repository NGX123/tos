/*
    @author = ngx123
    @brief = code for setting up GDT
*/


#include "include/init.h"


static struct gdt_entry gdt[3]; // Array is used to hold all of the GDT entries as they go one after another
static struct gdt_ptr gp;
static struct bootInfo bootInfo_struct;


static void toggleBit(size_t* var, size_t bitmask, uint8_t bit_status)
{
    if ((bit_status == TOGGLE_BIT_ON && (*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && !(*var & bitmask)))
        ;
    else if ((bit_status == TOGGLE_BIT_ON && !(*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && (*var & bitmask)))
        *var ^= bitmask;
}

static struct memInfo getMultibootMemInfo(int* count){
    multiboot_info_t* mboot_header = ((multiboot_info_t*)bootInfo_struct.boot_protocol_struct_ptr);
    struct memInfo memInfo_struct = {0};

    if ((mboot_header->flags & MULTIBOOT_INFO_MEMORY) != 0)
    {
        toggleBit((size_t*)&memInfo_struct.flags, MEMINFO_RAM_AMOUNT, TOGGLE_BIT_ON);
        memInfo_struct.lower_ram_size = mboot_header->mem_lower;
        memInfo_struct.upper_ram_size = mboot_header->mem_upper;
    }
    else
        toggleBit((size_t*)&memInfo_struct.flags, MEMINFO_RAM_AMOUNT, TOGGLE_BIT_OFF);

    if ((mboot_header->flags & MULTIBOOT_INFO_MEM_MAP) != 0)
    {
        multiboot_memory_map_64split_t* mboot_mmap_start = (multiboot_memory_map_64split_t*)mboot_header->mmap_addr;
        multiboot_memory_map_64split_t* mboot_mmap_end = (multiboot_memory_map_64split_t*)(mboot_header->mmap_addr + mboot_header->mmap_length);
        multiboot_memory_map_64split_t* mboot_mmap_current_entry = (multiboot_memory_map_64split_t*)((unsigned int)mboot_mmap_start + ((mboot_mmap_start->size + sizeof(mboot_mmap_start->size)) * *count)); // Size is also added because - size variable holds size of struct but does not take itself into account

        if (!(mboot_mmap_current_entry < mboot_mmap_end))
        {
            *count = -1;
            toggleBit((size_t*)&memInfo_struct.flags, MEMINFO_MEM_MAP, TOGGLE_BIT_OFF);
            return memInfo_struct;
        }
        //printf("Start: %p, Length: %p, Type: %p\n", (uint32_t)((multiboot_memory_map_t*)mboot_mmap_current_entry)->addr, (uint32_t)((multiboot_memory_map_t*)mboot_mmap_current_entry)->len, mboot_mmap_current_entry->type);
        *count += 1;
    }
    else
    {
        *count = -1;
        toggleBit((size_t*)&memInfo_struct.flags, MEMINFO_MEM_MAP, TOGGLE_BIT_OFF);
    }

    return memInfo_struct;
}

void* scanMemory()
{
    if (bootInfo_struct.protocol == PROTOCOL_MULTIBOOT){
        int count = 0;
        while(count != -1){
            getMultibootMemInfo(&count);
        }
    }

    return NULL;
}

void* getBootInfo()
{
    return &bootInfo_struct;
}

void setBootInfo(int var_num, ...)
{
    va_list valist;
    va_start(valist, var_num);

    if (va_arg(valist, uint32_t) == MULTIBOOT_BOOTLOADER_MAGIC){
        bootInfo_struct.protocol = PROTOCOL_MULTIBOOT;
        bootInfo_struct.boot_protocol_struct_ptr = va_arg(valist, void*);
        bootInfo_struct.boot_protocol_struct_end_ptr = bootInfo_struct.boot_protocol_struct_ptr + sizeof(multiboot_info_t);
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
