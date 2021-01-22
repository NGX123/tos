/*
    @author = ngx123
    @brief = global defines for hardware platform initialization code
*/


#ifndef INIT_H
#define INIT_H


#include <types.h>


#define PROTOCOL_NONE       0x0
#define PROTOCOL_MULTIBOOT  0x01


struct kernInfo
{
    void* boot_protocol_struct_ptr;
    void* struct_reserved_end_addr;
    void* memory_map_start_addr;
    void* memory_map_end_addr;

    uint16_t protocol;
    size_t high_ram_amount;
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
extern void setBootInfo(uint16_t protocol, int var_num, ...);
#endif