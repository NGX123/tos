/*
    @author = ngx123
    @brief = global header file for the kernel
*/


#include <stdint.h>


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