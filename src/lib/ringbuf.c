/*
    @author = ngx123
    @brief = ringbuffer library used for giving drivers an easy interface to use the ring buffer
*/


#include <ringbuf.h>


void RingBufferInit(ring_buffer_t* ringBuffer, int capacity, uint8_t* bufferPointer)
{
    ringBuffer->capacity = capacity;
    ringBuffer->size = 0;
    ringBuffer->readIndex = 0;
    ringBuffer->writeIndex = -1;
    ringBuffer->buffer = bufferPointer;
}

int writeBuf(ring_buffer_t* ringBuffer, uint8_t writeValue)
{
    if (ringBuffer->size == ringBuffer->capacity)
        return -1;
    else
    {
        ringBuffer->writeIndex = (ringBuffer->writeIndex + 1) % ringBuffer->capacity;

        ringBuffer->buffer[ringBuffer->writeIndex] = writeValue;
        ringBuffer->size++;
        return 0;
    }

}

int readBuf(ring_buffer_t* ringBuffer)
{
    if (ringBuffer->size == 0)
        return -1;
    else
    {
        uint8_t readValue = ringBuffer->buffer[ringBuffer->readIndex];

        ringBuffer->readIndex = (ringBuffer->readIndex + 1) % ringBuffer->capacity;
        ringBuffer->size--;

        return readValue;
    }
}