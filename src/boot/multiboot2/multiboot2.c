// @author = ngx123
// @brief = includes code needed to get data and setup everything using multiboot2


#include "multiboot-include.h"


static struct bootInfo bootInfo_struct;


void setBootInfo(int var_num, ...)  /* IS FOR MULTIBOOT 1*/
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

static struct memInfo getMultibootMemInfo(int count) /*DOES NOT WORK*/
{
}

static void toggleBit(size_t* var, size_t bitmask, uint8_t bit_status)
{
    if ((bit_status == TOGGLE_BIT_ON && (*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && !(*var & bitmask)))
        ;
    else if ((bit_status == TOGGLE_BIT_ON && !(*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && (*var & bitmask)))
        *var ^= bitmask;
}