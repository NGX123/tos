# [Memory Management](https://wiki.osdev.org/Program_Memory_Allocation_Types)
## Vocabulary
+ [Address spaces](https://wiki.osdev.org/Memory_management#Address_Spaces)
    - Physical Address Space - when address is accessed - it is the real address in RAM that is accessed(e.g. if 0xb8000 is read, the info will be read from 0xb8000s byte in RAM)
    - Virtual Address Space - when address is accessed - it is virtual address that is accessed and it will be translated into physical address that could(but not always) be a completely different address form the one accessed(e.g. if 0xb8000 is read, it could really be absoultely any other byte in RAM that is read, like byte 0x98 in RAM instead of 0xb8000 for example)

+ Virtual->Physical Memory Transaltion Systems
    - [Segmentation](https://wiki.osdev.org/Segmentation) - separating each memory area(virtual) into units using CS,DS,SS,ES,FS,GS(legacy way available only on x86)
    - [Paging](https://wiki.osdev.org/Paging) - splitting physical memory into chunks(pages) and mapping(virt-phys) page-wise


## Information
### General
- [Global Descriptor Table](https://wiki.osdev.org/Global_Descriptor_Table) - it was used for segmentation in protected mode when it was only invented, but now is always set to flat memory model(all virtual addresses map directly to physical)
    * **Flat memory mode:** in flat memory model all the virtual addresses are mapped direcotry to physical, this is done because both the data and code selectors span through all of the 4 GB space meaning that any access could be done to any address
    * **Limit** - spans through two fields so the first field is set to 0xffff and second(located later) is set to 0xf and together they make up a number that when multiplied by 4096(size of unit if granularity bit is set) is 4 Billion meaning that selector describes all 4 GB
    * **Base** - base also spans throgh two fields which both should be 0 on both selectors because the selectors start at 0x0 and span to the end of the memory(4 GB, as described in previous sentance)
    * **Access** - Access is the only field that differs because the executable bit is set on code selector and off on data other bits are just what they should be for descrptor to work
    * **Flags** - in the flags the granularity bit should be set so the limit is measured in 4KiB units and not in bytes and bits that depend on it(the one near) should also be set/off
- [Higher Half Kernel](https://wiki.osdev.org/Higher_Half_Kernel) - it is a way to make the memory space cleaner and let the programms start at 0x0, while kernel is right at the end of memory
    - **Motivation:** Kernel could be located at any physical address in the memory space(where it was loaded) at the start, but when paging is enabled the kernel can be remapped to any address in the address space becuase addresses become virtual. When paging is enabled it could be made that each program has it's own memory space, if the kernel is mapped at the end of memory then the programs recieve all the other memory space for them and can be loaded at 0x0 and use everything till the start of kernel which makes linking the application very easy(no need to take into account that first mb+ is taken up)
    - **Implementation:** The kernel and the space belowow 1mb(on x86) is remapped using paging to be right at the end of memory(so from their start to end of memory there is left only enough space to fit them)
		* There would be variables placed in the linker_script before the kernel and after the kernel to calculate it's size `kernel_size = kernel_end-kernel_start`, then the address where kernel should be reallocated would be counted with - if kernel_size < 512MB then: `kernel_vma = max_address(available in architecture) - 512MB`; if kernel_size > 512MB then - `kernel_vma = max_address(available in arch) - (kernel_size + 512MB)`
- [Page Frame Allocator](https://wiki.osdev.org/Page_Frame_Allocation) - a program which allocates 4096 byte blocks of memory to give to VMM to map the page to
- [Identity Paging](https://wiki.osdev.org/Identity_Paging) - when the pages are mapping virtual addresses 1 to 1 whith physical(e.g. 0xb8000 is 0xb8000)
- [xPaging](https://wiki.osdev.org/Paging)
    + How it works
		1. malloc() is called to allocate memory
		2. malloc() requests a page(4KiB) from VMM(Virtual memory manager)
		3. VMM then asks for page in physical memory from PFA(Page Frame Allocator)
		4. PFA finds free memory page(free 4KiB of RAM) and gives it's start address to VMM,
		5. VMM mapes virtual(in virtual address space) page to the physical page provided by the PFA and returns page to the malloc()
		6. Finally malloc() carves the amount of needed memory from the page and returns it's address to it's caller

### Paging
- Bullet points
	* In x86 paging first 12 bits of addresses used in paging data structs(PML4 address in CR3, PDP address in PML4...) are used for flags. It is possible to use first 12 bits for flags because all addresses of pages and tables used for paging should be 4096 aligned , and all numbers that are divisible/aligned by 4096 have first 12 bits as zeros so they could just be discareded by CPU when using as an address and be used for flags
	* Paging data structs could be allocated dynamically by the PMM because the size of each page is 4096 like the size of page frame allocated by the PMM
	* All the page tables should be cleared(all bits set to 0) before they are used. The .bss section is nullified(all bits are set 0) before the program is loaded so the bootstrap tables are OK and can be used without cleaning

### Page Frame Allocation
- **Allocation method** - frames pointer and bytemap pointer. Everything not marked as free in the memory map would be marked reserved(2) in the bytemap, everything that is free would be marked as free(0) and all the frames that were allocated by PMM would be marked as allocated(1)
- **Data structure location** - PMM would sum up sizes of all entries in mmap to get the amount of RAM, then the amount of ram would be devided by 4096(page size) to get size of byte map in bytes - `bytemap_size = RAM_amount / 4096`, then the bytemap_size amount of RAM would be reserved below the kernel and bye map pointer would point there
	* Frames pointer - would be a `void*` pointer to the 0x0(start of memory that would be used for frame allocation as it is the start of ram) and when finding an address for frame start to return use formula `frame_addr = frames_pointer + (page_number * 4096)`
	* Bytemap pointer - would be a `uint8_t` list which would contain bytes each of which would correspond to a frame(4096 bytes) in frame list(first byte in bytemap = first 4096 bytes in frame list). The bytes would have information on frame status(e.g. reserved/free...)


## Implementation
+ Physical frame allocation(PFA) - PFA does not really allocate anything and just returns address of free 4KiB(page) in physical RAM
	* kalloc_frame()
		1. First it loops through the bytemap until it finds an entry for a free page(0)
		2. When entry for free page is found, the start address of the corresponding page is found by `free_mem_start_ptr + bytemap_found_entry_num * 4096`
		3. The start address of page is returned to the caller, and entry corresponding to the allocated page is marked as allocated(1)
	* kfree_frame(address)
		1. Set the entry number `(address - start_address) / 4096` as free(0) in bitmap
+ Virtual Memory Manager - gets a physical page from PFA and maps it to a virtual page
+ Memory allocator - allocates memory by carving needed amount out of virtual page


## Resources
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
    * [Small malloc implementation](https://github.com/CCareaga/heap_allocator)
    * [TLB](https://wiki.osdev.org/TLB)


## Questions
### Current
- Can I mark the pages kernel is located on as read only(in page flags) so the kernel can't be accidentaly overwritten by data(which in my case is under the kernel)?
- Can the stack that was setup in protected mode, be used in long mode?

### Solved
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