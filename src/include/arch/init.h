/*
    @author = ngx123
    @brief = global defines for hardware platform initialization code
*/


#ifndef INIT_H
#define INIT_H


#include <kernel_types.h>


#define MEMINFO_FLAG_ERROR       	0x1

#define MEMMAP_AREA_TYPE_USABLE     0x1
#define MEMMAP_AREA_TYPE_RESERVED   0x2
#define MEMMAP_AREA_TYPE_SPECIAL    0x3 // Area that was reserved by kernel
#define MEMMAP_AREA_TYPE_OTHER      0x4 // Bootloader should be asked about this memory using bootloaderInterface()

#define BOOTLOADER_FUNCTION_INIT 	0x1


struct memInfo
{
    uint8_t flags;
    union       /* Unnamed union of full address and it's parts so two 32 bit variables can be used instead of one 64bit or other way round */
    {
        uint64_t addr_full;
        struct
        {
            uint32_t addr_part1;
            uint32_t addr_part2;
        };
    };
    union
    {
        uint64_t size_full;
        struct
        {
            uint32_t size_part1;
            uint32_t size_part2;
        };
    };
    uint32_t type;
    /* Fix: if problems occur split the 64bit fields into 32bit fields */
    // uint32_t addr1;
    // uint32_t addr2;
    // uint32_t size1;
    // uint32_t size2;
};

/*
    @brief = fills memInfo structure based on "count". Sets error flag if "count" is bigger then amount of entries in the memory map. Example use is - calling memInfo with count 0, reading data from memInfo, incrementing count by one, doing previous steps until error flag is set
    @param count = specifies the entry in the memory map
    @return = filled in memory structure
*/
extern struct memInfo arch_getMemInfo(int count);

/*
    @breif = reads needed information from the bootloader based on command and responds with it
*/
extern void arch_bootloaderInterface(uint32_t function);
#endif