# [Memory Management](https://wiki.osdev.org/Program_Memory_Allocation_Types)
## General Information
+ [Address spaces](https://wiki.osdev.org/Memory_management#Address_Spaces)
    - Physical Address Space - when address is accessed, it will be sent to CPU to work on without any change or translation and will be the real address in the RAM
    - Virtual Address Space - when each program has it's own memory space starting on 0x0 and going up to size of RAM, so each accessed address here is virtual(not real) and is changed to physical(how - depends on CPU) and only then sent to CPU to work on

+ Virtual->Physical Memory Transaltion Systems
    - [Segmentation](https://wiki.osdev.org/Segmentation) - separating each memory area(virtual) into units using CS,DS,SS,ES,FS,GS(only on x86)
    - [Paging](https://wiki.osdev.org/Paging) - splitting physical memory into chunks(pages) and mapping(virt-phys) page-wise

+ [Program memory types](https://wiki.osdev.org/Program_Memory_Allocation_Types)
    - Read-only - constants and code is stored here, .text(code) and .rodata(data) sections
    - Global - stores global variables available to entire program
    - Lexically-allocated - space allocated for vars, arrays... local to the function block, allocated at the start of function block and freed at the end of it
    - Dynamically allocated - used for data with unknown size and lifetime, allocated at runtime

## Resources
- AMD Manual - VOL 2, Chapters 3 & 4
- Intel Manual - VOL 3A, Chapters 3 & 4
- [Wiki on how MM works](https://linux-mm.org/)
- Tutorials
    * [Bona Fide OSdev](http://www.osdever.net/tutorials/index#Memory-Management)
    * [Brendan's MM Guide](https://wiki.osdev.org/Brendan%27s_Memory_Management_Guide)
    * [MM Guides from Forum](https://wiki.osdev.org/Page_Frame_Allocation#Threads)
    * [Writing MM](https://wiki.osdev.org/Writing_a_memory_manager)


## [Notes](https://wiki.osdev.org/Paging)
* What does it mean that the address needs to be 4KiB aligned?