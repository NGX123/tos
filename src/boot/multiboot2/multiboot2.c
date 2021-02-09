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
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
            {
                multiboot_uint32_t color;
                unsigned i;
                struct multiboot_tag_framebuffer *tagfb = (struct multiboot_tag_framebuffer *) tag_current;
                void *fb = (void *) (unsigned long) tagfb->common.framebuffer_addr;

                switch (tagfb->common.framebuffer_type) // Does setup based on the framebuffer type
                {
                    case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
                        {
                            unsigned best_distance, distance;
                            struct multiboot_color *palette;

                            palette = tagfb->framebuffer_palette;

                            color = 0;
                            best_distance = 4*256*256;

                            for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++)
                            {
                                distance = (0xff - palette[i].blue) * (0xff - palette[i].blue) + palette[i].red * palette[i].red + palette[i].green * palette[i].green;
                                if (distance < best_distance)
                                {
                                    color = i;
                                    best_distance = distance;
                                }
                            }
                        }
                        break;
                    case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                        color = ((1 << tagfb->framebuffer_blue_mask_size) - 1) << tagfb->framebuffer_blue_field_position;
                        break;
                    case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
                        color = '\\' | 0x0100;
                        break;
                    default:
                        color = 0xffffffff;
                        break;
                }

                for (i = 0; i < tagfb->common.framebuffer_width && i < tagfb->common.framebuffer_height; i++)
                {
                    switch (tagfb->common.framebuffer_bpp)
                    {
                        case 8:
                            {
                                multiboot_uint8_t *pixel = fb + tagfb->common.framebuffer_pitch * i + i;
                                *pixel = color;
                            }
                            break;
                        case 15:
                        case 16:
                            {
                                multiboot_uint16_t *pixel = fb + tagfb->common.framebuffer_pitch * i + 2 * i;
                                *pixel = color;
                            }
                            break;
                        case 24:
                            {
                                multiboot_uint32_t *pixel = fb + tagfb->common.framebuffer_pitch * i + 3 * i;
                                *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
                            }
                            break;

                        case 32:
                            {
                                multiboot_uint32_t *pixel = fb + tagfb->common.framebuffer_pitch * i + 4 * i;
                                *pixel = color;
                            }
                            break;
                    }
                }
                break;
            }
        }
        tag_current = (struct multiboot_tag *)((multiboot_uint8_t *)tag_current + ((tag_current->size + 7) & ~7));
        //printf ("Total mbi size 0x%x\n", (unsigned) tag - addr);
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