/*
    @author = ngx123
    @brief = includes code needed to get data and setup everything using multiboot2
*/

#include "multiboot-include.h"
#include "../../drivers/include/serial.h"
#include "stdio.h"


extern uint64_t multiboot_magic_var;
extern uint64_t multiboot_tags_address_var;

static unsigned long multiboot2_magic_copy;				// Space to store multiboot magic number locally so it is not stored in some random .bss section and can be lost
static address_tt multiboot2_tags_address_copy;			// Same but for tags address

static unsigned long multiboot2_all_tags_size;			// Tags structures toatal size variable, for use in mmap(to mark the space where the tags exist as reserved in the page frame allocator)

static uint16_t avilability_flags;


void arch_bootloaderInterface(uint32_t function)
{
	if (function == BOOTLOADER_FUNCTION_INIT)
		interpretMultiboot2();
}

void interpretMultiboot2(void)
{
	/* REMOVE THIS */
    initSerial();
    printSerial("\n\n---------------------------------------------------------------------------------------\n");
	/*             */



    struct multiboot_tag *multiboot2_tags_start_address_converted, *multiboot2_tag_current;

    if (multiboot_magic_var != MULTIBOOT2_BOOTLOADER_MAGIC)
    {
        printf("Magic number check failed\n");
        return;
    }
    if (multiboot_tags_address_var & 7)																// addr & 7, checks if the address is divisible 7, if yes - it fails becuase it means it is not aligned
    {
        printf("Align check failed\n");
        return;
    }

	multiboot2_magic_copy = multiboot_magic_var;													/* COPY THE MAGIC number and TAGS ptr to local variables */
	multiboot2_tags_address_copy = multiboot_tags_address_var;

    multiboot2_all_tags_size = *(uint32_t*)multiboot_tags_address_var;								// First in tags structure the size is located in uint_32 format so to get it the address is just converted to int32 pointer
    multiboot2_tags_start_address_converted = (struct multiboot_tag*)(multiboot_tags_address_var + 8); 	// The multiboot2 tags pointer points not directly to flags but to a structure where first field is size, so the address has to be converted to point to tags

    for (multiboot2_tag_current = multiboot2_tags_start_address_converted; multiboot2_tag_current->type != MULTIBOOT_TAG_TYPE_END; multiboot2_tag_current = (struct multiboot_tag*)((multiboot_uint8_t*)multiboot2_tag_current + ((multiboot2_tag_current->size + 7) & ~7)))	// Loop through all tags and do the needed thing for each tag
    {
        switch (multiboot2_tag_current->type)
        {
            case MULTIBOOT_TAG_TYPE_MMAP:
                toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_MEMMAP, TOGGLE_BIT_ON);
                break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_FRAMEBUFFER, TOGGLE_BIT_ON);
                break;
        }
    }


    /* HAS TO BE TRANSFERED TO THE KERNEL CODE */
    struct memInfo memory_map;
    int i;

    for (i = 0; ((memory_map = arch_getMemInfo(i)).flags & MEMINFO_FLAG_ERROR) == 0; i++)
    {
        printf("addr = 0x%lx, length = 0x%lx, type = 0x%x\n", memory_map.start_address, memory_map.area_size, memory_map.area_type);
    }
}

struct memInfo arch_getMemInfo(int count)
{
    struct memInfo returnStruct = {0};
    struct multiboot_tag *tag_current, *tag_start = (struct multiboot_tag *) (multiboot_tags_address_var + 8);
    multiboot_memory_map_t *mmap;

    if (!(avilability_flags & AVAILABLE_FLAG_MEMMAP))	// Return error if the bootloader hasn't given the memory map
    {
        toggleBit((size_t*)&returnStruct.flags, MEMINFO_FLAG_ERROR, TOGGLE_BIT_ON);
        return returnStruct;
    }

    for (tag_current = tag_start; tag_current->type != MULTIBOOT_TAG_TYPE_END; tag_current = (struct multiboot_tag *)((multiboot_uint8_t *)tag_current + ((tag_current->size + 7) & ~7)))	// Parse the memory map and put it in the kernels format
        if (tag_current->type == MULTIBOOT_TAG_TYPE_MMAP)
        {
            mmap = ((struct multiboot_tag_mmap *) tag_current)->entries;
            mmap = (multiboot_memory_map_t *) ((uint64_t)mmap + (((struct multiboot_tag_mmap *)tag_current)->entry_size * count));

            if ((multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag_current + tag_current->size)
            {
                returnStruct.start_address = mmap->addr;
                returnStruct.area_size = mmap->len;
                returnStruct.area_type = mmap->type;
            }
            else
            {
                toggleBit((size_t*)&returnStruct.flags, MEMINFO_FLAG_ERROR, TOGGLE_BIT_ON);
                return returnStruct;
            }

            /* DEBUG */
            // uint32_t *mmap_addr32, *mmap_len32;
            // for (mmap = ((struct multiboot_tag_mmap *) tag_current)->entries; (multiboot_uint8_t *) mmap < (multiboot_uint8_t *) tag_current + tag_current->size; mmap = (multiboot_memory_map_t *) ((unsigned long)mmap + ((struct multiboot_tag_mmap *)tag_current)->entry_size))
            // {
            //     mmap_addr32 = (uint32_t*)&mmap->addr;
            //     mmap_len32 = (uint32_t*)&mmap->len;
            //     printf ("---base_addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n", *(mmap_addr32+1), *mmap_addr32, *(mmap_len32+1), *mmap_len32, mmap->type); //printf ("--- base_addr = 0x%x%x, length = 0x%x%x, type = 0x%x\n", (unsigned) (mmap->addr >> 32), (unsigned) (mmap->addr & 0xffffffff), (unsigned) (mmap->len >> 32), (unsigned) (mmap->len & 0xffffffff), (unsigned) mmap->type);
            // }
        }

    return returnStruct;
}

static void toggleBit(size_t* var, size_t bitmask, uint8_t bit_status)
{
    if ((bit_status == TOGGLE_BIT_ON && (*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && !(*var & bitmask)))
        ;
    else if ((bit_status == TOGGLE_BIT_ON && !(*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && (*var & bitmask)))
        *var ^= bitmask;
}




/* TOMPORARY IMPLEMENTATION FOR DEBUGGING */
int putchar(int chara)
{
    writeSerial(chara);
    return 0;
}