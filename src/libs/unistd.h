// File: unistd.c
// Description: includes defines of the system call handler functions



#include <stdint.h>
#include "ringbuf.h"

// Reads the struct pointer supplied by the first argument and uses function pointers contained in the 
uint32_t read(int fd, void *buf, uint32_t count){
    ring_buffer_t ringBuffer;
    char string[250];
    RingBufferInit(&ringBuffer, 250, string);


}