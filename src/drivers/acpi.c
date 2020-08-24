// File: acpi.c
// Description: driver for reading acpi tables from the memory



#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include "acpi.h"

struct RSDPDescriptor *rsdp_struct;

/// RSDP ///
// Finds for RSDP in EBDA
void findRSDPinEBDA(){
    volatile char* EBDApointer = (volatile char*)0x40E;
    volatile char* EBDA = (volatile char*)*EBDApointer;
    
    char array[9];      
    int i = 0;          // Counter for going through all memory
    int j = 0;          // Counter for going through the string in memory
    int k = 0;          // Counter for going through the string in array  

    // Search EBDA
    for (i = 0; i != 1023; i++){
        if (EBDA[i] == 'R'){
            for (j = i, k = 0; j != (i+8); j++, k++)
                array[k] = EBDA[j];
            array[k] = 0;

            if (strcmp("RSD PTR ", array) == 0){
                printf("\nRSDP struct found\n");
                rsdp_struct = (struct RSDPDescriptor*)&EBDA[i];

                printf("\nSignature - %s\n", array);
                printf("Checksum - %d\n", rsdp_struct->Checksum);
                printf("OEM ID - %s\n", rsdp_struct->OEMID);   
                printf("Revision - %d\n", rsdp_struct->Revision);  
                printf("Address - %p\n", rsdp_struct->RsdtAddress); 
                //findRSDT();
                break;       
            }       
        }
    }
    if (strcmp("RSD PTR ", array) != 0)
        findRSDPinEXTMEM();
}

// Search for RSDP in  Extra Memory
void findRSDPinEXTMEM(){
    volatile char* EXTMEMstart = (volatile char*)0x000E0000;
    volatile char* EXTMEMend = (volatile char*)0x000FFFFF;

    char array[9];      // Array for storing the string
    int i;              // Counter for going through all memory
    int j;              // Counter for going through string in memory
    int k;              // Counter for going through string in array
    
    printf("No RSDP found in the EBDA, searching extra memory\n");
    for (i = 0; &EXTMEMstart[i] != EXTMEMend; i++){
        if (EXTMEMstart[i] == 'R'){
            for (j = i, k = 0; j != (i+8); j++, k++)
                array[k] = EXTMEMstart[j];
            array[k] = 0;

            if (strcmp("RSD PTR ", array) == 0){
                printf("\nRSDP struct found\n");
                rsdp_struct = (struct RSDPDescriptor*)&EXTMEMstart[i];

                printf("\nSignature - %s\n", array);
                printf("Checksum - %d\n", rsdp_struct->Checksum);
                printf("OEM ID - %s\n", rsdp_struct->OEMID);   
                printf("Revision - %d\n", rsdp_struct->Revision);  
                printf("Address - %p\n", rsdp_struct->RsdtAddress);
                //findRSDT();
                break;     
            }       
        }
    }
    if (strcmp("RSD PTR ", array) != 0)
        printf("Failed to initialize ACPI\n");
}




/// RSDT ///
// RSDT structure
// struct RSDT {
//     struct ACPISDT h;
//     uint32_t SDTpointer[(h.Length - sizeof(h.Length)) / 4];
// };

// // XSDT structure(for ACPIv2)
// struct XSDT {
//     struct ACPISDT h;
//     uint32_t SDTpointer[(h.Length - sizeof(h.Length)) / 8];
// };
    
// void findRSDT(){
//     struct RSDT* RSDTaddress = (struct RSDT*)rsdp_tables[0]->RsdtAddress;

//     printf("\nSignatureRSDT - %s\n", RSDTaddress->h.Signature);

// }