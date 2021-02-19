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
    1. Load the paging directory table start address into CR3
    2. Set 32 bit of CR0
3. Virtual Memory Manager - gets a physical page from PFA and maps it to a virtual page
4. Memory allocator - allocates memory by carving needed amount out of virtual page
+ Memmory allocation
    - [Port an existing memory manager to the OS](https://wiki.osdev.org/Memory_Allocation#Porting_an_existing_Memory_Allocator)
    - [Pancakes memory allocator implementations](https://wiki.osdev.org/User:Pancakes/SimpleHeapImplementation)


## Information
- [Higher Half Kernel](https://wiki.osdev.org/Higher_Half_Kernel) - it is a way to make the memory space cleaner and let the programms start at 0x0, while kernel is right at the end of memory
    - **Motivation:** Kernel could be located at any physical address in the memory space(where it was loaded) at the start, but when paging is enabled the kernel can be remapped to any address in the address space becuase addresses become virtual. When paging is enabled it could be made that each program has it's own memory space, if the kernel is mapped at the end of memory then the programs recieve all the other memory space for them and can be loaded at 0x0 and use everything till the start of kernel which makes linking the application very easy(no need to take into account that first mb+ is taken up)
    - **Implementation:** The kernel and the space belowow 1mb(on x86) is remapped using paging to be right at the end of memory(so from their start to end of memory there is left only enough space to fit them)
- [Identity Paging](https://wiki.osdev.org/Identity_Paging) - when the pages are mapping virtual addresses 1 to 1 whith physical(e.g. 0xb8000 is 0xb8000)
- [x86 Paging](https://wiki.osdev.org/Paging)


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
    * [TLB](https://wiki.osdev.org/TLB)


## Questions
* Higher Half Kernel
    - Why is 0xC0000000 used - which is below 4GB which means that even in 64 bit mode programm can only access stuff below 0xc0000000 or how does it work, is it remapped beyond end of kernel as soon as programm alocates all the space below?
        * The 0xC0000000 is used only in 32bit mode because it leaves 3GB free for programms and a little space at the end of memory for the kernel
    - Is there any use in having the kernel physically there(which is even impossible in some cases like if computer has 2GB RAM)?
        * No, it is even impossible to have the kernel located there in most configurations and in 64 bit mode it is beyond what motherboards support
    - What if in 32 bit mode kernel would need more RAM
        * Kernel could just be remapped to a lower address then 0xc0000000 or any other that is used by the kernel