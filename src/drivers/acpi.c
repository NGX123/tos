// File: acpi.c
// Description: driver for reading acpi tables from the memory



#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include "acpi.h"

// Finds for RSDP in EBDA
struct RSDP* findRSDPinEBDA(){
    volatile char* EBDApointer = (volatile char*)0x40E;
    volatile char* EBDA = (volatile char*)((int)*EBDApointer);
    
    struct RSDP* RSDPstruct;    // RSDP structure
    char signature[9];          // Signature 
    int i = 0;                  // Counter for going through all memory
    int j = 0;                  // Counter for going through the string in memory
    int k = 0;                  // Counter for going through the string in array  

    // Search EBDA
    for (i = 0; i != 1023; i++){
        // Put the signature in the array
        if (EBDA[i] == 'R'){
            for (j = i, k = 0; j != (i+8); j++, k++)
                signature[k] = EBDA[j];
            signature[k] = 0;
            
            // Case if signature found
            if (strcmp("RSD PTR ", signature) == 0){
                RSDPstruct = (struct RSDP*)&EBDA[i];
                return RSDPstruct;       
            }       
        }
    }
    // Case if signature not found
    if (strcmp("RSD PTR ", signature) != 0)
        RSDPstruct =  NULL;

    return RSDPstruct;
}

// Search for RSDP in  Extra Memory
struct RSDP* findRSDPinEXTMEM(){
    volatile char* EXTMEMstart = (volatile char*)0x000E0000;
    volatile char* EXTMEMend = (volatile char*)0x000FFFFF;

    struct RSDP* RSDPstruct;
    char signature[9];  // Array for storing the string
    int i;              // Counter for going through all memory
    int j;              // Counter for going through string in memory
    int k;              // Counter for going through string in array
    
    for (i = 0; &EXTMEMstart[i] != EXTMEMend; i++){
        // Put signature into array
        if (EXTMEMstart[i] == 'R'){
            for (j = i, k = 0; j != (i+8); j++, k++)
                signature[k] = EXTMEMstart[j];
            signature[k] = 0;

            // Case if signature was found
            if (strcmp("RSD PTR ", signature) == 0){
                RSDPstruct = (struct RSDP*)&EXTMEMstart[i];
                return RSDPstruct;   
            }       
        }
    }
    // Case if signature not found
    if (strcmp("RSD PTR ", signature) != 0)
        RSDPstruct = NULL;

    return RSDPstruct;
}

// Find the SDTs in memory
void findSDT(struct RSDP* RSDPstruct){
    int i, j;
    struct RSDT* RSDTstruct = (struct RSDT*)RSDPstruct->RsdtAddress;
    int entries_amount = (RSDTstruct->h.Length - sizeof(struct ACPISDT)) / 4;
    printf("\nSignatureRSDT - %s\n", RSDTstruct->h.Signature);

    for (i = 0; i < entries_amount; i++){
        struct ACPISDT* ACPISDTtmp = (struct ACPISDT*)(RSDTstruct->sdtptr+i);
        char strtmp[5];
        for (j = 0; j < 4; j++)
            strtmp[j] = ACPISDTtmp->Signature[j];
        strtmp[j] = 0; 

        printf("%s\n", strtmp);
    }
}

// void *findFACP(void *RootSDT)
// {
//     RSDT *rsdt = (RSDT *) RootSDT;
//     int entries = (rsdt->h.Length - sizeof(rsdt->h)) / 4;
 
//     for (int i = 0; i < entries; i++)
//     {
//         ACPISDTHeader *h = (ACPISDTHeader *) rsdt->PointerToOtherSDT[i];
//         if (!strncmp(h->Signature, "FACP", 4))
//             return (void *) h;
//     }
 
//     // No FACP found
//     return NULL;
// }