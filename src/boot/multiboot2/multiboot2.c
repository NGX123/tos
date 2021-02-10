// @author = ngx123
// @brief = includes code needed to get data and setup everything using multiboot2


#include "multiboot-include.h"
#include "../../drivers/include/serial.h"

void *multiboot_struct_start_ptr, *multiboot_struct_end_ptr;

void printf(char *str);


void interpretMultiboot2(uint32_t magic, uint32_t infoStruct_addr)
{
    initSerial();
    printSerial("---------------------------------------------------------------------------------------");


    unsigned int size;
    struct multiboot_tag *tag_start, *tag_current;

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)   // addr & 7, checks if the address is divisible 7, if yes - it fails becuase it means it is not aligned
    {
        printf("---Magic number check failed---");
        return;
    }
    if (infoStruct_addr & 7)
    {
        printf("---Align check failed---");
        return;
    }

    size = *(unsigned int*)infoStruct_addr;
    tag_start = (struct multiboot_tag *) (infoStruct_addr + 8);
    // printf ("Announced mbi size 0x%x\n", size);

    for (tag_current = tag_start; tag_current->type != MULTIBOOT_TAG_TYPE_END; tag_current = (struct multiboot_tag *)((multiboot_uint8_t *)tag_current + ((tag_current->size + 7) & ~7)))
    {
        //printf ("Tag 0x%x, Size 0x%x\n", tag->type, tag->size);
        switch (tag_current->type)
        {
            case MULTIBOOT_TAG_TYPE_CMDLINE:        /* LOOK AT WHAT THE STRING CONTAINS AND IF IT IS NOTHING USEFULL REMOVE THIS TAG PARSING */
                //printf ("Command line = %s\n", ((struct multiboot_tag_string *)tag_current)->string);
                break;
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:  /* RECORD THE DATA INTO VARIABLES AND THEN getMemInfo WOULD NOT NEED TO SEARCH THE TAG EACH TIME AND COULD JUST READ INFO FROM VARIABLES */
                //printf ("mem_lower = %uKB, mem_upper = %uKB\n", ((struct multiboot_tag_basic_meminfo *) tag)->mem_lower, ((struct multiboot_tag_basic_meminfo *) tag)->mem_upper);
                break;
            // case MULTIBOOT_TAG_TYPE_MMAP:           /* DO NOT INTERPRET, INSTEAD MARK A FLAG OR A VARIABLE TO SHOW THAT MEMORY MAP IS AVIALABLE AND THEN IF getMemInfo() will see that there is no memory map it will fail, otherwise interpret it */
            //     {
            //         multiboot_memory_map_t *mmap;

            //         for (mmap = ((struct multiboot_tag_mmap *)tag_current)->entries; (multiboot_uint8_t *) mmap < (multiboot_uint8_t *)tag_current + tag_current->size; mmap = (multiboot_memory_map_t *)((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag_current)->entry_size))
            //             printf (" base_addr = 0x%x%x," " length = 0x%x%x, type = 0x%x\n", (unsigned) (mmap->addr >> 32), (unsigned) (mmap->addr & 0xffffffff), (unsigned) (mmap->len >> 32), (unsigned) (mmap->len & 0xffffffff), (unsigned) mmap->type);
            //     }
            //     break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: /* MAKE IT DO THE NEEDED SETUP AND SET A BIT/VARIABLE TO INDICATE THAT FRAMEBUFFER WAS SETUP AND THE DRIVER WILL NOT WASTE TIME SEARCHING FOR IT */
                break;
        }
    }
}

void bootloaderInterface()  /* For special reqests for information from the bootloader */
{
}

static void toggleBit(size_t* var, size_t bitmask, uint8_t bit_status)
{
    if ((bit_status == TOGGLE_BIT_ON && (*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && !(*var & bitmask)))
        ;
    else if ((bit_status == TOGGLE_BIT_ON && !(*var & bitmask)) || (bit_status == TOGGLE_BIT_OFF && (*var & bitmask)))
        *var ^= bitmask;
}





void printf(char *str)
{
    printSerial(str);
}