// @author = ngx123
// @brief = includes code needed to get data and setup everything using multiboot2


#include "multiboot-include.h"
#include "../../drivers/include/serial.h"

void *multiboot_struct_start_ptr, *multiboot_struct_end_ptr;
uint16_t avilability_flags;


void interpretMultiboot2(uint32_t magic, uint32_t infoStruct_addr)
{
    initSerial();
    printSerial("---------------------------------------------------------------------------------------\n");


    unsigned int size;
    struct multiboot_tag *tag_start, *tag_current;

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)   // addr & 7, checks if the address is divisible 7, if yes - it fails becuase it means it is not aligned
    {
        printf("Magic number check failed\n");
        return;
    }
    if (infoStruct_addr & 7)
    {
        printf("Align check failed\n");
        return;
    }

    size = *(unsigned int*)infoStruct_addr;
    tag_start = (struct multiboot_tag *) (infoStruct_addr + 8);

    for (tag_current = tag_start; tag_current->type != MULTIBOOT_TAG_TYPE_END; tag_current = (struct multiboot_tag *)((multiboot_uint8_t *)tag_current + ((tag_current->size + 7) & ~7)))
    {
        switch (tag_current->type)
        {
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_MEMSIZE, TOGGLE_BIT_ON);
                break;
            case MULTIBOOT_TAG_TYPE_MMAP:           /* DO NOT INTERPRET, INSTEAD MARK A FLAG OR A VARIABLE TO SHOW THAT MEMORY MAP IS AVIALABLE AND THEN IF getMemInfo() will see that there is no memory map it will fail, otherwise interpret it */
                {
                    multiboot_memory_map_t *mmap;

                    for (mmap = ((struct multiboot_tag_mmap *)tag_current)->entries; (multiboot_uint8_t *) mmap < (multiboot_uint8_t *)tag_current + tag_current->size; mmap = (multiboot_memory_map_t *)((unsigned long) mmap + ((struct multiboot_tag_mmap *) tag_current)->entry_size))
                        printf (" base_addr = 0x%x%x," " length = 0x%x%x, type = 0x%x\n", (unsigned) (mmap->addr >> 32), (unsigned) (mmap->addr & 0xffffffff), (unsigned) (mmap->len >> 32), (unsigned) (mmap->len & 0xffffffff), (unsigned) mmap->type);
                }
                break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: /* MAKE IT DO THE NEEDED SETUP AND SET A BIT/VARIABLE TO INDICATE THAT FRAMEBUFFER WAS SETUP AND THE DRIVER WILL NOT WASTE TIME SEARCHING FOR IT */
                break;
        }
    }
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

static void printint(int xx, int base, int sgn)
{
    static char digits[] = "0123456789ABCDEF";
    char buf[16];
    int i, neg;
    uint32_t x;

    neg = 0;
    if(sgn && xx < 0){
        neg = 1;
        x = -xx;
    }
    else
        x = xx;

    i = 0;
    do
    {
        buf[i++] = digits[x % base];
    } while((x /= base) != 0);

    if (neg)
        buf[i++] = '-';

    while(--i >= 0)
        putchar(buf[i]);
}

void printf(const char *fmt, ...) // only understands %d, %x, %p, %s, %c
{
  char *s;
  int c, i, state;
  uint32_t *ap;

  state = 0;
  ap = (uint32_t*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++)
  {
    c = fmt[i] & 0xff;
    if(state == 0)
    {
        if(c == '%')
            state = '%';
        else
            putchar(c);
    }
    else if(state == '%')
    {
        if(c == 'd')
        {
            printint((int)*ap, 10, 1);
            ap++;
        }
        else if(c == 'x' || c == 'p')
        {
            printint((int)*ap, 16, 0);
            ap++;
        }
        else if(c == 's')
        {
            s = (char*)*ap;
            ap++;
            if(s == 0)
                s = "(null)";
            while(*s != 0)
            {
                putchar(*s);
                s++;
            }
        }
        else if(c == 'c')
        {
            putchar((int)*ap);
            ap++;
        }
        else if(c == '%')
        {
            putchar(c);
        }
        else
        {
            // Unknown % sequence.  Print it to draw attention.
            putchar('%');
            putchar(c);
        }
        state = 0;
    }
  }
}