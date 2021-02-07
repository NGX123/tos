/*
    @author = ngx123
    @brief = global defines for hardware platform initialization code
*/


#ifndef INIT_H
#define INIT_H


#include <types.h>


#define MEMINFO_FLAG_ERROR       0x1
#define MEMINFO_FLAG_RAM_AMOUNT  0x2
#define MEMINFO_FLAG_MEM_MAP     0x3


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

/*
    @brief = fills memInfo structure based on "count". Sets error flag if "count" is bigger then amount of entries in the memory map. Example use is - calling memInfo with count 0, reading data from memInfo, incrementing count by one, doing previous steps until error flag is set
    @param count = specifies the entry in the memory map
    @return = filled in memory structure
*/
extern struct memInfo getMemInfo(int count);

/*
    @breif = reads needed information from the bootloader based on command and responds with it
*/
extern void bootloaderInterface(void);
#endif