// @author = ngx123
// @brief = includes code needed to get data and setup everything using multiboot2


#include "multiboot-include.h"
#include "../../drivers/include/serial.h"

static void *multiboot_struct_start_ptr, *multiboot_struct_end_ptr;
static uint16_t avilability_flags;


void interpretMultiboot2(uint32_t magic, uint32_t infoStruct_addr)
{
    initSerial();
    printSerial("\n\n---------------------------------------------------------------------------------------\n");


    uint32_t all_tags_size;
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

    all_tags_size = *(uint32_t*)infoStruct_addr;
    tag_start = (struct multiboot_tag *) (infoStruct_addr + 8);

    multiboot_struct_start_ptr = (void*)infoStruct_addr;
    multiboot_struct_end_ptr = (void*)(infoStruct_addr + all_tags_size - 1);

    for (tag_current = tag_start; tag_current->type != MULTIBOOT_TAG_TYPE_END; tag_current = (struct multiboot_tag *)((multiboot_uint8_t *)tag_current + ((tag_current->size + 7) & ~7)))
    {
        switch (tag_current->type)
        {
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
                toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_MEMSIZE, TOGGLE_BIT_ON);
                break;
            case MULTIBOOT_TAG_TYPE_MMAP:
                toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_MEMMAP, TOGGLE_BIT_ON);
                break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                toggleBit((size_t*)&avilability_flags, AVAILABLE_FLAG_FRAMEBUFFER, TOGGLE_BIT_ON);
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