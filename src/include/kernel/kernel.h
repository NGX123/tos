/*
    @author = ngx123
    @brief = global header file for the kernel
*/


#include <stdint.h>


#define PROTOCOL_MULTIBOOT  0x01


struct bootHeader
{
    uint16_t protocol;
    void* struct_reserved_start_addr;
    void* struct_reserved_end_addr;
    void* memory_map_start_addr;
    void* memory_map_end_addr;
};