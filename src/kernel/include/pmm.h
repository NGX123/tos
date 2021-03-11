/*
    @author = ngx123
    @brief = local defines for pmm
*/


#include <arch/init.h>

#include <kernel/pmm.h>


#define FRAME_TYPE_FREE      			0x0		// Can be given on allocation request
#define FRAME_TYPE_ALLOCATED 			0x1		// Can not be given on allocation request
#define FRAME_TYPE_RESERVED				0x2		// Indicates the frame can not be used as it is reserved by firmware(frames can be unreserved)

#define FRAME_SIZE						4096


/*
    @brief = returns the total amount of RAM available
    @return = total amount of RAM, 0 on error
*/
static size_t getRAMsize();

/*
	@brief = initialises the bitmap by finding it's location reserving everything...
	@return = 0 on success, -1 on error
*/
static int initBitmap();

/*
	@brief = marks all the areas in bitmap according to the memory map
	@return = 0 on success, -1 on error
*/
static int interpretMemoryMap();
