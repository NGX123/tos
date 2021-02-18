/*
    @author = ngx123
    @brief = local defines for pmm
*/


#include <arch/init.h>


#define PAGE_TYPE_FREE      0x0
#define PAGE_TYPE_ALLOCATED 0x1
#define PAGE_TYPE_RESERVED  0x2


typedef size_t address_size;


/*
    @brief = returns the total amount of RAM available
    @return = total amount of RAM
*/
address_size getRAMsize();

/*
    @brief = marks pages free/reserved based on the memory map
*/
void fillByteMap();