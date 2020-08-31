// File: ringbuf.c
// Description: defenitions for the ringbuffer library



#ifndef RINGBUF_H
#define RINGBUF_H

struct RBufStruct {
    int capacity;               // Capacity
    int size;                   // Current size(filled in)
    int readIndex;              // Current position to read from the buffer
    int writeIndex;             // Current position to write to the buffer
    char* buffer;               // Pointer to the buffer
};
typedef struct RBufStruct ring_buffer_t; 

// Initialises the ring_buffer structure with values
// Returns: nothing
extern void RingBufferInit(ring_buffer_t* ringBuffer, int capacity, char* pointer);

// Writes to the ring buffer
// Returns: 0 on succesfull write, 1 if buffer is full
extern int writeBuf(ring_buffer_t* ringBuffer, char writeValue);

// Reads from the ring buffer
// Returns: ascii code of read character if succesfull, 1 if buffer is empty 
extern int readBuf(ring_buffer_t* ringBuffer);
#endif