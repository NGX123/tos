/*
    @author = ngx123
    @breif = physicaly memory manager(page frame allocator)
*/


#include "include/pmm.h"
#include <stdio.h>


size_t RAMsize, freeRAM, reservedRAM, allocatedRAM;
uint8_t page_status_bytemap[3]; /* THIS IS INCORRECT AND IS JUST TO ELIMINATE WARNINGS IN IDE BECUASE THE SIZE OF THE BYTEMAP SHOULD BE DETERMINED AT RUNTIME DEPENDING ON THE RAM SIZE */


int initPMM()
{
	if ((RAMsize = getRAMsize()) == 0)
		return -1;

	return 0;
}

static size_t getRAMsize()
{
    size_t ram_size = 0;
	int kernel_area_presence_status = 0;
    struct memInfo memory_map;

    for (size_t i = 0; ((memory_map = arch_getMemInfo(i, MEMMAP_TYPE_PROTOCOL)).flags & MEMINFO_FLAG_ERROR) == 0; i++)
    {
        ram_size += memory_map.area_size;
		if (memory_map.area_type == MEMMAP_AREA_TYPE_KERNEL)
			kernel_area_presence_status = 1;

		printf("addr = 0x%lx, length = 0x%lx, type = 0x%x\n", memory_map.start_address, memory_map.area_size, memory_map.area_type);
	}

	if (kernel_area_presence_status == 0)
		return 0;				// FAIL

    return ram_size;
}