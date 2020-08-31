// File: ringbuf.c
// Description: ringbuffer library used for giving drivers an easy interface to use the ring buffer



struct RBufStruct {
    int capacity;               // Capacity
    int size;                   // Current size(filled in)
    int readIndex;              // Current position to read from the buffer
    int writeIndex;             // Current position to write to the buffer
    char* buffer;               // Pointer to the buffer
};
typedef struct RBufStruct ring_buffer_t; 

void RingBufferInit(ring_buffer_t* ringBuffer, int capacity, char* pointer){
    ringBuffer->capacity = capacity;
    ringBuffer->size = 0;
    ringBuffer->readIndex = 0;
    ringBuffer->writeIndex = -1;
    ringBuffer->buffer = pointer;
    //return ringBuffer;
}

// Writes to the ring buffer
int writeBuf(ring_buffer_t* ringBuffer, char writeValue){
    if (ringBuffer->size == ringBuffer->capacity)
        return 1;
    else {
        ringBuffer->writeIndex = (ringBuffer->writeIndex + 1) % ringBuffer->capacity;

        ringBuffer->buffer[ringBuffer->writeIndex] = writeValue;
        ringBuffer->size++;
        return 0;
    }
    
}

// Reads from the ring buffer
int readBuf(ring_buffer_t* ringBuffer){
    if (ringBuffer->size == 0)
        return 1;
    else {
        int readValue = ringBuffer->buffer[ringBuffer->readIndex];
        
        ringBuffer->readIndex = (ringBuffer->readIndex + 1) % ringBuffer->capacity;
        ringBuffer->size--;

        return readValue;
    }
}