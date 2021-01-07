/*
    @author = ngx123
    @brief = header for ringbuffer library
*/


#ifndef RINGBUF_H
#define RINGBUF_H


#include <stdint.h>


struct RBufStruct
{
    int capacity;               // Capacity
    int size;                   // Current size(filled in)
    int readIndex;              // Current position to read from the buffer
    int writeIndex;             // Current position to write to the buffer
    uint8_t* buffer;               // Pointer to the buffer
};
typedef struct RBufStruct ring_buffer_t;


/*
    @brief = initialises the ring_buffer structure with values
    @param ringBuffer = pointer to a fing_buffer struct variable
    @param capacity = size of the ring buffer
    @param bufferPointer = pointer to the buffer that should be used as a ring buffer
*/
extern void RingBufferInit(ring_buffer_t* ringBuffer, int capacity, uint8_t* bufferPointer);

/*
    @brief = writes value to the ring buffer
    @param ringBuffer = pointer to the ringbuffer structure that should be used to write to buffer
    @param writeValue = the value that should be written
    @return = 0 on success, -1 if buffer is full
*/
extern int writeBuf(ring_buffer_t* ringBuffer, uint8_t writeValue);

/*
    @brief = reads the value from the ring buffer
    @param ringBuffer = pointer to the ringbuffer structure that should be used to read from
    @return = ascii code of read character if succesfull, -1 if buffer is empty
*/
extern int readBuf(ring_buffer_t* ringBuffer);
#endif