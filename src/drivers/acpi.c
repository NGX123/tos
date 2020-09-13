// File: acpi.c
// Description: driver for reading acpi tables from the memory



#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include "headers/acpi-defines.h"

#include "drivers/acpi.h"

// Checksum RSDP structure
int checksumRSDP(struct RSDP* RSDPstruct){
    int i;
    uint8_t check = 0;
    uint8_t* bptr = (uint8_t*)RSDPstruct;

    for (i = 0; i < (int)sizeof(struct RSDP); i++){
        check += *bptr;
        bptr++;
    }

    if (check == 0)
        return 0;
    
    return -1;
}

// checks for a given header and validates checksum
int checksumHeaderACPI(unsigned int *ptr)
{
    char *checkPtr = (char*) ptr;
    int len = *(ptr + 1);
    char check = 0;
    
    while (0 < len--){
        check += *checkPtr;
        checkPtr++;
    }

    if (check == 0)
        return 0;

   return -1;
}

// One function to control the ACPI and the only one to be exposed
// 1 - find the fadt, 
void* ACPIcontrol(int action){
    if (action == ACPI_CONTROL_FIND_FADT){
        // Find the RSDP
        struct RSDP* RSDPstruct;
        struct FADT* FADTstruct;
        if ((RSDPstruct = findRSDPinEBDA()) == NULL)
            if ((RSDPstruct = findRSDPinEXTMEM()) == NULL)
                return NULL;

        if ((FADTstruct = findSDT(RSDPstruct, "FACP")) == NULL)
            return NULL;
        else{
            return (struct FADT*)FADTstruct;
        }
            
    }
    return NULL;
}

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
                if (checksumRSDP(RSDPstruct) == 0)
                    return RSDPstruct;       
            }       
        }
    }
    return NULL;
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
                if (checksumRSDP(RSDPstruct) == 0)
                    return RSDPstruct;   
            }       
        }
    }
    return NULL;
}

// Find the SDTs in memory
void* findSDT(struct RSDP* RSDPstruct, char* signature){
    int i;
    struct RSDT* RSDTstruct = (struct RSDT*)RSDPstruct->RsdtAddress;
    if (checksumHeaderACPI((unsigned int*)RSDTstruct) != 0)
        return NULL;
    int entries_amount = (RSDTstruct->h.Length - sizeof(struct ACPISDT)) / 4;

    for (i = 0; i < entries_amount; i++){
        struct ACPISDT* ACPISDTtmp = (struct ACPISDT*)(RSDTstruct->sdtptr+i);
    
        if (strncmp(ACPISDTtmp->Signature, signature, 4) == 0)
            if (checksumHeaderACPI((unsigned int*)ACPISDTtmp) == 0)
                return (void*)ACPISDTtmp;
    }

    return NULL;
}