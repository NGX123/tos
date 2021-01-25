/*
    @author = ngx123
    @breif = a physical memory page frame allocator
*/


// #include "include/page_frame_alloc.h"


// static void* start_frame = &kernel_end_addr;
// static volatile void* free_space = (volatile void*)&kernel_end_addr;
// static uint32_t page_counter;
// static uint8_t frame_bitmap[PAGE_AMOUNT];

// void* initPFA()
// {
//     struct kernInfo* kernInfo_struct = getBootInfo();

//     return 0;
// }

// static void* kallocFrameInt()
// {
//     uint32_t i = 0;
//     while(frame_bitmap[i] != PAGE_STATUS_FREE)
//     {
//         i++;
//         if (i == npages)
//             return -1;
//     }
//     frame_bitmap[i] = PAGE_STATUS_USED;
//     return (start_frame + (i*PAGE_SIZE));
// }

// void* kallocFrame()
// {
//     int i;
//     static uint8_t allocate = 1;
//     static uint8_t pframe = 0;
//     void* ret;

//     if (pframe == 20)
//         allocate = 1;

//     if (allocate == 1)
//     {
//         for (i = 0; i < 20; i++)
//             free_space[i] = kalloc_frame_int();
//         pframe = 0;
//         allocate = 0;
//     }
//     ret = free_space[pframe];
//     pframe++;
//     return ret;
// }

// void kfreeFrame(void* addr)
// {
//     addr = addr - start_frame;
//     if(addr == 0)
//     {
//             uint32_t index = (uint32_t)addr;
//             frame_bitmap[index] = PAGE_STATUS_FREE;
//     }
//     else
//     {
//         uint32_t index = ((uint32_t)addr) / PAGE_SIZE;
//         frame_bitmap[index] = PAGE_STATUS_FREE;
//     }
// }