/*
    @author = ngx123
    @brief = local declarations for page frame allocator
*/


#include <stdint.h>


#define PAGE_SIZE           4096
#define PAGE_STATUS_FREE    0x00
#define PAGE_STATUS_USED    0x01


extern uint32_t kernel_end_addr;    // Holds the kernel end address from linkerscript