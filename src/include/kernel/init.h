/*
    @author = ngx123
    @brief = global defines for hardware platform initialization code
*/


#ifndef INIT_H
#define INIT_H


#include <types.h>


#define PROTOCOL_NONE       0x0
#define PROTOCOL_MULTIBOOT  0x01

#define MEMINFO_ERROR       0x1
#define MEMINFO_RAM_AMOUNT  0x2
#define MEMINFO_MEM_MAP     0x3


struct bootInfo
{
    uint16_t protocol;
    void* boot_protocol_struct_ptr;
    void* boot_protocol_struct_end_ptr;
};

struct kernelMemMap
{
    uint64_t mem_area_start_addr;
    uint64_t mem_area_end_addr;
    size_t mem_area_size;
    uint8_t mem_area_type;
};

struct memInfo
{
    uint8_t flags;
    uint64_t lower_ram_size;
    uint64_t upper_ram_size;
    struct kernelMemMap memory_map;
};
#endif