/*
    @author = ngx123
    @brief = global defines for hardware platform initialization code
*/


#ifndef INIT_H
#define INIT_H


#include <types.h>
#include <kernel/multiboot.h>


#define PROTOCOL_NONE       0x0
#define PROTOCOL_MULTIBOOT  0x01

#define MEMINFO_RAM_AMOUNT  0x1
#define MEMINFO_MEM_MAP     0x2


struct bootInfo
{
    uint16_t protocol;
    void* boot_protocol_struct_ptr;
    void* boot_protocol_struct_end_ptr;
};

struct kernelMemMap
{
    void* mem_area_start_addr;
    void* mem_area_end_addr;
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


/*
    @breif = return the pointer to data collected by the kernel
    @return = pointer to kernInfo struct
*/
extern void* getBootInfo();

/*
    @brief = solves all of the platform specfic stuff for the kernel
    @param protocol = type of boot protocol used
    @param var_num = amount of arguments to be passed
    @param ... = the platform specific arguments
*/
extern void setBootInfo(int var_num, ...);

/*
    @brief = a wrapper around bootloader specific functions to that frees other functions fromhaving to find out which bootloader is used
    @param count = passed to bootloader
    @return = passed on from bootloader
*/
extern struct memInfo scanMemory(int* count);
#endif