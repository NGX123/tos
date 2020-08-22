// File: acpi.c
// Description: driver for reading acpi tables from the memory


#include <stdint.h>
#include "stdio.h"
#include "string.h"

/// RSPD ///
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
    volatile char* EBDApointer = (volatile char*)0x40E;
    volatile char* EBDA = (volatile char*)*EBDApointer;
    volatile char* EXTMEMstart = (volatile char*)0x000E0000;
    volatile char* EXTMEMend = (volatile char*)0x000FFFFF;
    printf("%p\n", EBDA);
    
    char array[9];      
    int i = 0;          // Counter for going through all memory
    int j = 0;          // Counter for going through the string in memory
    int k = 0;          // Counter for going through the string in array  
    int l = 0;          // Counter for storing the strcutres   

    // Search EBDA
    for (i = 0; i != 1023; i++){
        if (EBDA[i] == 'R'){
            for (j = i, k = 0; j != (i+8); j++, k++)
                array[k] = EBDA[j];
            array[k] = 0;

            if (strcmp("RSD PTR ", array) == 0){
                printf("\nACPI Device found\n");
                rsdp_tables[l] = (struct RSDPDescriptor*)&EBDA[i];
                printf("start_serch[i] - %p\n", &EBDA[i]);
                printf("rsdp - %p\n", rsdp_tables[l]);
                printf("%d", rsdp_tables[l++]->Revision);              
            }       
        }
    }
    i = 0; j = 0; k = 0;

    // Search Extra Memory
    for (int i = 0; &EXTMEMstart[i] != EXTMEMend; i++){
        if (EXTMEMstart[i] == 'R'){
            for (j = i, k = 0; j != (i+8); j++, k++)
                array[k] = EXTMEMstart[j];
            array[k] = 0;

            if (strcmp("RSD PTR ", array) == 0){
                printf("\nACPI Device found\n");
                rsdp_tables[l] = (struct RSDPDescriptor*)&EXTMEMstart[i];
                printf("EXTMEMstart[i] - %p\n", &EXTMEMstart[i]);
                printf("Revision - %d\n", rsdp_tables[l]->Revision);     
                printf("OEM ID - %s\n", rsdp_tables[l++]->OEMID);     
            }       
        }
    }
}