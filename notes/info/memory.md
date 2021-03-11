# [Vocabulary](https://wiki.osdev.org/Program_Memory_Allocation_Types)
- [Address spaces](https://wiki.osdev.org/Memory_management#Address_Spaces)
    * Physical Address Space - when address is accessed - it is the real address in RAM that is accessed(e.g. if 0xb8000 is read, the info will be read from 0xb8000s byte in RAM)
    * Virtual Address Space - when address is accessed - it is virtual address that is accessed and it will be translated into physical address that could(but not always) be a completely different address form the one accessed(e.g. if 0xb8000 is read, it could really be absoultely any other byte in RAM that is read, like byte 0x98 in RAM instead of 0xb8000 for example)
- Virtual->Physical Memory Transaltion Systems
    * [Segmentation](https://wiki.osdev.org/Segmentation) - separating each memory area(virtual) into units using CS,DS,SS,ES,FS,GS(legacy way available only on x86)
    * [Paging](https://wiki.osdev.org/Paging) - splitting physical memory into chunks(pages) and mapping(virt-phys) page-wise
- [Higher Half Kernel](https://wiki.osdev.org/Higher_Half_Kernel) - it is a way to make the memory space cleaner and let the programms start at 0x0, while kernel is right at the end of memory
    * **Motivation:** Kernel could be located at any physical address in the memory space(where it was loaded) at the start, but when paging is enabled the kernel can be remapped to any address in the address space becuase addresses become virtual. When paging is enabled it could be made that each program has it's own memory space, if the kernel is mapped at the end of memory then the programs recieve all the other memory space for them and can be loaded at 0x0 and use everything till the start of kernel which makes linking the application very easy(no need to take into account that first mb+ is taken up)
    * **Implementation:** The kernel and the space belowow 1mb(on x86) is remapped using paging to be right at the end of memory(so from their start to end of memory there is left only enough space to fit them)
		* There would be variables placed in the linker_script before the kernel and after the kernel to calculate it's size `kernel_size = kernel_end-kernel_start`, then the address where kernel should be reallocated would be counted with - if kernel_size < 512MB then: `kernel_vma = max_address(available in architecture) - 512MB`; if kernel_size > 512MB then - `kernel_vma = max_address(available in arch) - (kernel_size + 512MB)`
- [Identity Paging](https://wiki.osdev.org/Identity_Paging) - when the pages are mapping virtual addresses 1 to 1 whith physical(e.g. 0xb8000 is 0xb8000)



# [Paging](https://wiki.osdev.org/Paging)
## Information
- Bullet points
	* In x86 paging first 12 bits of addresses used in paging data structs(PML4 address in CR3, PDP address in PML4...) are used for flags. It is possible to use first 12 bits for flags because all addresses of pages and tables used for paging should be 4096 aligned , and all numbers that are divisible/aligned by 4096 have first 12 bits as zeros so they could just be discareded by CPU when using as an address and be used for flags
	* Paging data structs could be allocated dynamically by the PMM because the size of each page is 4096 like the size of page frame allocated by the PMM
	* All the page tables should be cleared(all bits set to 0) before they are used. The .bss section is nullified(all bits are set 0) before the program is loaded so the bootstrap tables are OK and can be used without cleaning



# [Page Frame Allocation(Also called Physical Memory Manager and Physical Memory Allocator(for easier search in google))](https://wiki.osdev.org/Page_Frame_Allocation)
## Information
- Description - gives out 4096 portions of physical memory to be mapped to virtual memory
- **Allocation method** - frames pointer and bytemap pointer. Everything not marked as free in the memory map would be marked reserved(2) in the bytemap, everything that is free would be marked as free(0) and all the frames that were allocated by PMM would be marked as allocated(1)
- **Data structure location** - PMM would sum up sizes of all entries in mmap to get the amount of RAM, then the amount of ram would be devided by 4096(page size) to get size of byte map in bytes - `bytemap_size = RAM_amount / 4096`, then the bytemap_size amount of RAM would be reserved below the kernel and bye map pointer would point there
	* Frames pointer - would be a `void*` pointer to the 0x0(start of memory that would be used for frame allocation as it is the start of ram) and when finding an address for frame start to return use formula `frame_addr = frames_pointer + (page_number * 4096)`
	* Bytemap pointer - would be a `uint8_t` list which would contain bytes each of which would correspond to a frame(4096 bytes) in frame list(first byte in bytemap = first 4096 bytes in frame list). The bytes would have information on frame status(e.g. reserved/free...)


## Implementation
### Bit/Bytemap algorithm
+ Defenitions/Info
	- Bytemap - an array of bytes where each byte represents the status(reserved/allocated/free) of a page-size(4096 bytes) block of physical RAM
	- When determining size(how many frames will fit) in frames(4096) of some memory range
		* Add +1, so there is one more page reserved as diving by 4096 may give a float which is slightly bigger but requires an extra page
			* DO THIS IN CASES LIKE RESERVED MEMORY WHERE IT IS REALLY IMPORTANT NOT TO TOUCH IT
			* Don't do it with areas like free becase there could be half a page lost and it will not harm
+ Initialization
	* Declare `size_t reserved_ram_size, free_ram_size, allocated_ram_size` to calculate amount of different types of memory
	1. Get the the start of memory from the memory map
		* Start of the first entry in memmap
	2. Get the amount/end of RAM
		* Sum of sizes of all zones in memmap

	3. Find the largest free zone in the memory map(loop until finding largest zone by size out of all) and make the bitmap be a pointer to it (`bitmap_ptr = largest_zone_addr`) <!--- A bad descision, find another way to find the location for stroing bitmap -->
		* There should be a check, so the area where the bitmap is does not collide with where the kernel is
			* If it does use the byte right after the kernel finish
	4. Record Address and size of bytemap
		* Bitmap size - `(int/uint)RAM_size / 4096 + 1`
			* Conversion to int/uint is needed because the number should not be a float, +1 is needed to prevent any overflows

	4. Change all statuses in the bitmap to **RESERVED**, because memory map has gaps(so memory should not be free and then some pieces reserved. But memory should be reserved and some pieces should be freed)
	6. Change statuses of pages that are used by the kernel to **ALLOCATED**
		* The start and size of kernel should be specified in a memory map entry with a special type(KERNEL)
			* If memory does not specify this, the OS should fail as it is very important information

	7. Change statuses of all pages in the bitmap to **RESERVED**. This is done because memory map does not specify all of the reserved areas(it has gapps)
	8. Change statuses of pages located in free areas to **FREE**

+ Functionality
	- `void* palloc(size_t frame_count)` - allocates amount of frames specified starting from frame which includes frame_address
		1. Search for the first frame status that is **FREE** in the bytemap(by looping through the whole bytemap till `i < bytemap_size`)
			* Error if a free frame is not found - `return NULL`
		2. Mark frame as **ALLOCATED**
		3. Return address of the allocated frame
			* `return_address = found_index * 4096`

	- `int pfree(address_tt frame_address, size_t frame_count)` - frees amount of frames specified starting from frame which includes frame_address
		1. Get the index in the bytemap with `index = (uint)frame_address / 4096`
		2. Check if the index is below bytemap_size - `i < bytemap_size`
		3. If the page is allocated then continue, otherwise(free or reserved) `return -1`
		4. Change the entry status to **FREE**
		5. Add 4096(page size) to `free_ram_size` as there is now more free RAM
		6. Remove 4096(page size) from `allocated_ram_size` as RAM was freed
		7. `return 0`

	- `int reserveRAM(address_tt frame_address, size_t frame_count)` - reserves physical memory(so it can't be allocated)
		1. Get the index in the bytemap with `index = (uint)frame_address / 4096`
		2. Check if the index is below bytemap_size - `i < bytemap_size`
		3. If the page is free continue, otherwise(resereved or allocated) `return -1`
		4. Change the bytemap entry status to **RESERVED**
		5. Add 4096(page size) to `reserved_ram_size` as there is now more reserved RAM
		6. Remove 4096(page size) from `free_ram_size` as RAM was reserved
		7. `return 0`

	- `int unreserveRAM(address_tt frame_address, size_t frame_count)` - unreserves physical memory so it can be allocated
		1. Get the index in the bytemap with `index = (uint)frame_address / 4096`
		2. Check if the index is below bytemap_size - `i < bytemap_size`
		3. If the page is reserved continue, otherwise(free or allocated) `return -1`
		4. Change the entry status to **FREE**
		5. Add 4096(page size) to `free_ram_size` as there is now more free RAM
		6. Remove 4096(page size) from `reserved_ram_size` as RAM was freed
		7. `return 0`

	- `size_t getRAMsize()` - get's the total amount of RAM
		* Declare a static variable `RAMsize = 0`
		1. If RAM size is 0, then the RAM size has not been calculated yet and we can proccedd to step 2, otherwise it is already calculated so we return the `RAMsize` variable
		2. Add up the sizes of all memory map areas - each time add size to `RAMsize`
+ Problems - how would multiple frames be allocated one after another (e.g. for HUGE(1GB) pages which are sometimes usefull)

### Linked List + Stack algorithm
## Information
- Here stack = linked list
- Description - in this algorithm a linked list that acts like a stack is used to manage the frames. Here the first `address_length` bytes in the frame are used to store the address of the next free frame, and when the frame is taken or put back on to the linked list it is zerod. The address of the first free frame in the system is a pointer to a pointer to the next free frame which includes pointer to the next and so on. First free frame is the head of the linked list pointing to the next free frame(as all frames are zerod , the last frame will point to 0, so NULL pointer is end of the linked list). When a frame is allocated - frame at head address is removed from the linked list, it is zerod and address is returned to the caller. When frame is freed - it is put at the head of the list(so into it there is a pointer inserted to the current head making it the head), and can be allocated again;
	* e.g. `frame1.pointer -> frame2.pointer -> frame3.pointer`. Then one frame is allocated and we are left with `frame2.pointer -> frame3.pointer` and so on. When a frame is freed we have `freed_frame.pointer -> frame2.pointer -> frame3.pointer`.
	* ALL ADDRESSES MUST BE 4096 aligned
	* FRAMES SHOULD BE ZEROED BEFORE PASSING SO THE POINTER TO NEXT ONE IS NOT EXPOSED.
	+ Problems
		* How would multiple frames be allocated one after another



# [VMM(Virutal Memory Manager)](https://wiki.osdev.org/Memory_Allocation)
## Information
- Description - manages page tables and mapps pages to physical addresses(mapping virtual addresses)



# Memory Allocator
## Information
- Description - gives out portions from the mapped virtual memory



# Segmentation
## Information
- Description - seperates the memory in to several segments giving them different access rights

## Implementation
* The GDT should be loaded with code and data segments with different access rights, but that just span over the whole memory range making physical addresses directly mapped to virtual
- [Global Descriptor Table](https://wiki.osdev.org/Global_Descriptor_Table) - it was used for segmentation in protected mode when it was only invented, but now is always set to flat memory model(all virtual addresses map directly to physical)
    * **Flat memory mode:** in flat memory model all the virtual addresses are mapped direcotry to physical, this is done because both the data and code selectors span through all of the 4 GB space meaning that any access could be done to any address
    * **Limit** - spans through two fields so the first field is set to 0xffff and second(located later) is set to 0xf and together they make up a number that when multiplied by 4096(size of unit if granularity bit is set) is 4 Billion meaning that selector describes all 4 GB
    * **Base** - base also spans throgh two fields which both should be 0 on both selectors because the selectors start at 0x0 and span to the end of the memory(4 GB, as described in previous sentance)
    * **Access** - Access is the only field that differs because the executable bit is set on code selector and off on data other bits are just what they should be for descrptor to work
    * **Flags** - in the flags the granularity bit should be set so the limit is measured in 4KiB units and not in bytes and bits that depend on it(the one near) should also be set/off



# Resources
- AMD Manual - VOL 2, Chapters 3 & 4
- Intel Manual - VOL 3A, Chapters 3 & 4
- [Wiki on how MM works](https://linux-mm.org/)
- Tutorials
	* [Make Page frame alloctor](https://wiki.osdev.org/Writing_A_Page_Frame_Allocator)
	* [Setup paging](https://wiki.osdev.org/Setting_Up_Paging)
	* [Paging][https://wiki.osdev.org/Paging]

    * [Bona Fide OSdev](http://www.osdever.net/tutorials/index#Memory-Management)
    * [Brendan's MM Guide](https://wiki.osdev.org/Brendan%27s_Memory_Management_Guide)
    * [MM Guides from Forum](https://wiki.osdev.org/Page_Frame_Allocation#Threads)
    * [Writing MM](https://wiki.osdev.org/Writing_a_memory_manager)
    * [Anastasion Page Frame Allocator Tutorial](https://anastas.io/osdev/memory/2016/08/08/page-frame-allocator.html)
    * [James Molloy Paging](http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html)
    * [Bona Fide Paging](http://www.osdever.net/tutorials/view/implementing-basic-paging)
    * [Port an existing memory manager to the OS](https://wiki.osdev.org/Memory_Allocation#Porting_an_existing_Memory_Allocator)
    * [Pancakes memory allocator implementations](https://wiki.osdev.org/User:Pancakes/SimpleHeapImplementation)
- Other
	* [Higher Half Kernel Guide on i386](https://medium.com/@connorstack/how-does-a-higher-half-kernel-work-107194e46a64)
    * [Small malloc implementation](https://github.com/CCareaga/heap_allocator)
    * [TLB](https://wiki.osdev.org/TLB)



# Questions
## Current
- Can I mark the pages kernel is located on as read only(in page flags) so the kernel can't be accidentaly overwritten by data(which in my case is under the kernel)?
- Can the stack that was setup in protected mode, be used in long mode?

## Solved
- Paging
	+ What if the some data happens to be located between two pages(e.g. several byts on one and others on another) and how to prevent it?
		* This is not a problem ans probably is not even possible
	+ Should the page frame allocator align every page by 4096?
		* Yes because becuase all addressess used for paging structs and pages should be 4096, otherwise first 12 bits would not be zeroes(be free for use)
	+ Where should be the page tables/structs be stored in memory(should some space be allocated for them using PFA)?

- Higher Half Kernel
    + Why is 0xC0000000 used - which is below 4GB which means that even in 64 bit mode programm can only access stuff below 0xc0000000 or how does it work, is it remapped beyond end of kernel as soon as programm alocates all the space below?
        * The 0xC0000000 is used only in 32bit mode because it leaves 3GB free for programms and a little space at the end of memory for the kernel
    + Is there any use in having the kernel physically there(which is even impossible in some cases like if computer has 2GB RAM)?
        * No, it is even impossible to have the kernel located there in most configurations and in 64 bit mode it is beyond what motherboards support
    + What if in 32 bit mode kernel would need more RAM
        * Kernel could just be remapped to a lower address then 0xc0000000 or any other that is used by the kernel