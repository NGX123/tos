// File: circular_buf.c
// Description: a file containting implementation of a circular buffer algorithm that will be used to get the data from drivers

#include <stdlib.h>
#include <stdio.h>

#define SIZE_OF_BUFFER 10

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




int main(){
    char string[] = "hello world, this message is written from c code that was compiled using gcc compiler";
    int readreturn;
    ring_buffer_t ringBuffer;
    char buffer[SIZE_OF_BUFFER];  
    RingBufferInit(&ringBuffer, SIZE_OF_BUFFER, buffer);

    for (int i = 0; string[i] != 0 && string[i] != ','; i++)
        if (writeBuf(&ringBuffer, string[i]) == 1)
            break;

    for (int i = 0; 1; i++)
        if ((readreturn = readBuf(&ringBuffer)) == 1)
            break;
        else 
            printf("%c", readreturn);
    printf("\n");

    for (int i = 13; string[i] != 0 && string[i] != 'c'; i++)
        if (writeBuf(&ringBuffer, string[i]) == 1)
            break;

    for (int i = 0; 1; i++)
        if ((readreturn = readBuf(&ringBuffer)) == 1)
            break;
        else 
            printf("%c", readreturn);
}