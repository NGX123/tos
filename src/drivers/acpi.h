// File: acpi.c
// Description: driver for reading acpi tables from the memory


#include <stdint.h>
#include "stdio.h"
#include "string.h"

// Strucuture for the RSPD Descriptor in ACPI v2
struct RSDPDescriptor {
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    uint32_t RsdtAddress;
} __attribute__ ((packed));

// Structure for the RSPD Descriptor in ACPI v2
struct RSDPDescriptor20 {
 struct RSDPDescriptor firstPart;
 
 uint32_t Length;
 uint64_t XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed));

struct RSDPDescriptor *rsdp_tables[6];


void findRSDPDescriptor(){
    volatile char* start_serch = (volatile char*)0x00100000;	// Start of the memory
    volatile char* end_search = (volatile char*)0xFFFFFFFF;     // End of the memory
    
    char array[9];      
    long long i = 0;    // Counter for going through all memory
    long long j = 0;    // Counter for going through the string in memory
    int k = 0;          // Counter for going through the string in array  
    int l = 0;          // Counter for storing the strcutres   

    for (i = 0; &start_serch[i] != end_search; i++){
        if (start_serch[i] == 'R'){
            for (j = i, k = 0; j != (i+8); j++, k++)
                array[k] = start_serch[j];
            array[k] = 0;

            if (strcmp("RSD PTR ", array) == 0){
                printf("\nACPI Device found\n");
                rsdp_tables[l] = (struct RSDPDescriptor*)&start_serch[i];
                printf("start_serch[i] - %p\n", &start_serch[i]);
                printf("rsdp - %p\n", rsdp_tables[l]);
                printf("%d", rsdp_tables[l++]->Revision);              
            }       
        }
    }
}