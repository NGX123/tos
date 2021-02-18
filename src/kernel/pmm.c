/*
    @author = ngx123
    @breif = physicaly memory manager(page frame allocator)
*/


#include "include/pmm.h"
#include <stdio.h>

uint8_t page_status_bytemap[3]; /* THIS IS INCORRECT AND IS JUST TO ELIMINATE WARNINGS IN IDE BECUASE THE SIZE OF THE BYTEMAP SHOULD BE DETERMINED AT RUNTIME DEPENDING ON THE RAM SIZE */


address_size getRAMsize()
{
    int i;
    size_t ram_size = 0;
    struct memInfo memory_map;

    for (i = 0; ((memory_map = arch_getMemInfo(i)).flags & MEMINFO_FLAG_ERROR) == 0; i++)
    {
        printf("addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n", memory_map.addr_part2, memory_map.addr_part1, memory_map.size_part2, memory_map.size_part1, memory_map.type);
        ram_size += memory_map.size_full;
    }

    return ram_size;
}

void fillByteMap()
{

}