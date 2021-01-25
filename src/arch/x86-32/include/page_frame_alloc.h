/*
    @author = ngx123
    @brief = local declarations for page frame allocator
*/


#include <stdint.h>
#include <stdio.h>
#include <kernel/init.h>


#define PAGE_SIZE           4096
#define PAGE_AMOUNT         1048576 // 4GiB(4 294 967 296B) / 4KiB(4096) - MAX RAM AMOUNT / PAGE SIZE
#define PAGE_STATUS_FREE    0x00
#define PAGE_STATUS_USED    0x01
#define PAGE_RESERVED       0x02


extern uint32_t kernel_end_addr;    // Holds the kernel end address from linkerscript