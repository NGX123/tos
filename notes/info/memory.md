# [Memory Management](https://wiki.osdev.org/Program_Memory_Allocation_Types)
## Vocabulary
+ [Address spaces](https://wiki.osdev.org/Memory_management#Address_Spaces)
    - Physical Address Space - when address is accessed - it is the real address in RAM that is accessed(e.g. if 0xb8000 is read, the info will be read from 0xb8000s byte in RAM)
    - Virtual Address Space - when address is accessed - it is virtual address that is accessed and it will be translated into physical address that could(but not always) be a completely different address form the one accessed(e.g. if 0xb8000 is read, it could really be absoultely any other byte in RAM that is read, like byte 0x98 in RAM instead of 0xb8000 for example)

+ Virtual->Physical Memory Transaltion Systems
    - [Segmentation](https://wiki.osdev.org/Segmentation) - separating each memory area(virtual) into units using CS,DS,SS,ES,FS,GS(legacy way available only on x86)
    - [Paging](https://wiki.osdev.org/Paging) - splitting physical memory into chunks(pages) and mapping(virt-phys) page-wise

## Implementation
+ Memory management with paging
    1. malloc() is called to allocate memory
    2. malloc() requests a page(4KiB) from VMM(Virtual memory manager)
    3. VMM then asks for page in physical memory from PFA(Page Frame Allocator)
    4. PFA finds free memory page(free 4KiB of RAM) and gives it's start address to VMM,
    5. VMM mapes virtual(in virtual address space) page to the physical page provided by the PFA and returns page to the malloc()
    6. Finally malloc() carves the amount of needed memory from the page and returns it's address to it's caller
1. [Physical frame allocation(PFA)](https://wiki.osdev.org/Page_Frame_Allocation) - PFA does not really allocate anything and just returns address of free 4KiB(page) in physical RAM
    - Bitmap method
        1. A list of `RAM size / 4096` entries is made(called bitmap/bytemap), each entry here corresponds to a page(4KiB) in RAM and represents it's status(e.g. if 0 - free page, 1 - allocated page)
        2. Pointer to the start of free memory is made(e.g. right after the end of the kernel executable)
        3. Entries in bytemap are filled with 2(or any other number, to mean that it belongs to device) in places which are not free to use(e.g. needed for devices) based on the obtained memory map - entire page is skipped if some address in it is reserved
        4. Usage
            * kalloc_frame()
                1. First it loops through the bytemap until it finds an entry for a free page(0)
                2. When entry for free page is found, the start address of the corresponding page is found by `free_mem_start_ptr + bytemap_found_entry_num * 4096`
                3. The start address of page is returned to the caller, and entry corresponding to the allocated page is marked as allocated(1)
            * kfree_frame(address)
                1. Set the entry number `(address - start_address) / 4096` as free(0) in bitmap
            * A seperate function is needed to map specific addresses - e.g. 0xb8000-... to the same address in physical RAM, becuase some devices like VGA require it
2. [Paging](https://wiki.osdev.org/Paging)
    - [Identity Paging](https://wiki.osdev.org/Identity_Paging)
    1. Load the paging directory table start address into CR3
    2. Set 32 bit of CR0
3. Virtual Memory Manager - gets a physical page from PFA and maps it to a virtual page
4. Memory allocator - allocates memory by carving needed amount out of virtual page
+ Memmory allocation
    - [Port an existing memory manager to the OS](https://wiki.osdev.org/Memory_Allocation#Porting_an_existing_Memory_Allocator)
    - [Pancakes memory allocator implementations](https://wiki.osdev.org/User:Pancakes/SimpleHeapImplementation)

## Resources
- AMD Manual - VOL 2, Chapters 3 & 4
- Intel Manual - VOL 3A, Chapters 3 & 4
- [Wiki on how MM works](https://linux-mm.org/)
- Tutorials
    * [Bona Fide OSdev](http://www.osdever.net/tutorials/index#Memory-Management)
    * [Brendan's MM Guide](https://wiki.osdev.org/Brendan%27s_Memory_Management_Guide)
    * [MM Guides from Forum](https://wiki.osdev.org/Page_Frame_Allocation#Threads)
    * [Writing MM](https://wiki.osdev.org/Writing_a_memory_manager)
    * [Make Page frame alloctor](https://wiki.osdev.org/Writing_A_Page_Frame_Allocator)
    * [Setup paging](https://wiki.osdev.org/Setting_Up_Paging)
    * [Anastasion Page Frame Allocator Tutorial](https://anastas.io/osdev/memory/2016/08/08/page-frame-allocator.html)
    * [James Molloy Paging](http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html)
    * [Bona Fide Paging](http://www.osdever.net/tutorials/view/implementing-basic-paging)
- Other
    * [Small malloc implementation](https://github.com/CCareaga/heap_allocator)

## [Notes](https://wiki.osdev.org/Paging)
* Releated to paging in x86 https://wiki.osdev.org/TLB
* Change the mapping of the kernel after enabling paging - [Higher Half Kernel](https://wiki.osdev.org/Higher_Half_Kernel)

## Ideas
* To have memory allocator be independant from the architecture and future proof(for 128bits for example:))))  use size_t in declarations

## Questions
* Higher Half Kernel
    * Why is 0xC0000000 used - which is below 4GB which means that even in 64 bit mode programm can only access stuff below 0xc0000000 or how does it work, is it remapped beyond end of kernel as soon as programm alocates all the space below?
    * Is there any use in having the kernel physically there(which is even impossible in some cases like if computer has 2GB RAM)?
    * What if in 32 bit mode kernel would need more RAM
then from 0xC000,0000 to 0xFFFF,FFFF, this is okay in 64 bit mode becuase there could be memory beyond that but what to do in 32 bit mode?