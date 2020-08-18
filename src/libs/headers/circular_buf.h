// File: circular_buf.c
// Description: a file containting implementation of a circular buffer algorithm that will be used to get the data from drivers


#ifndef CIRC_BUF
#define CIRC_BUF

#include <stdint.h>

#define SIZE_OF_BUFFER 8    // Stores the size of cicular buffer

int bufflen = 0;            // Stores the number of currently read elements in the buffer, decreased when read, and increased when wrote
int readIndex = 0;          // Stores index position to read from
int writeIndex = 0;         // Stores index position to read from


int checkBufLen(){
    if (bufflen == SIZE_OF_BUFFER){
        
    }
}
#endif