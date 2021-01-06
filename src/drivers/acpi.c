/*
    @author = ngx123
    @brief = ACPI hardware detection driver
*/


#include <stdint.h>
#include "stdio.h"
#include "string.h"
#include "headers/acpi.h"
#include "drivers/acpi.h"


static int checksumRSDP(void* RSDPstruct){
    int i;
    uint8_t check = 0;
    uint8_t* bptr = (uint8_t*)RSDPstruct;

    // Checksum ACPI v1
    if (((struct RSDP*)RSDPstruct)->Revision == 0){
        for (i = 0; i < (int)sizeof(struct RSDP); i++){
            check += *bptr;
            bptr++;
        }
    }
    // Checksum ACPI v2+
    else if (((struct RSDP*)RSDPstruct)->Revision > 0){
        for (i = 0; i < (int)sizeof(struct RSDP2); i++){
            check += *bptr;
            bptr++;
        }
    }

    if (check == 0)
        return 0;

    return -1;
}

static int checksumHeaderACPI(void* voidPtr)
{
    uint32_t* ptr = (uint32_t*)voidPtr;

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

static void* findRSDPinEBDA(){
    volatile char* EBDApointer = (volatile char*)0x40E;
    volatile char* EBDA = (volatile char*)((int)*EBDApointer);

    void* RSDPstruct;           // RSDP structure
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
                RSDPstruct = (void*)&EBDA[i];
                if (checksumRSDP(RSDPstruct) == 0)
                    return RSDPstruct;
            }
        }
    }
    return NULL;
}

static void* findRSDPinEXTMEM(){
    volatile char* EXTMEMstart = (volatile char*)0x000E0000;
    volatile char* EXTMEMend = (volatile char*)0x000FFFFF;

    void* RSDPstruct;
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
                RSDPstruct = (void*)&EXTMEMstart[i];
                if (checksumRSDP(RSDPstruct) == 0)
                    return RSDPstruct;
            }
        }
    }
    return NULL;
}

static void* findSDT(void* RSDPstruct, char* signature){
    int i;
    int entries_amount;
    void* RSDTstruct;
    struct ACPISDT* ACPISDTtmp;

    // Case for ACPI v1
    if (((struct RSDP*)RSDPstruct)->Revision == 0){
        RSDTstruct = (struct RSDT*) (((struct RSDP*)RSDPstruct)->RsdtAddress);

        if (checksumHeaderACPI((void*)RSDTstruct) != 0)
            return NULL;

        entries_amount = (((struct RSDT*)RSDTstruct)->h.Length - sizeof(struct ACPISDT)) / 4;
    }
    // Case for ACPI v2+
    else if (((struct RSDP*)RSDPstruct)->Revision > 0){
        RSDTstruct = (struct XSDT*) (((struct RSDP2*)RSDPstruct)->XsdtAddress);

        if (checksumHeaderACPI((void*)RSDTstruct) != 0)
            return NULL;

        if (strncmp(((struct XSDT*)RSDTstruct)->h.Signature, "XSDT", 4) != 0)
            return NULL;

        entries_amount = (((struct XSDT*)RSDTstruct)->h.Length - sizeof(struct ACPISDT)) / 8;
    }
    else
        return NULL;

    for (i = 0; i < entries_amount; i++){
        if (((struct RSDP*)RSDPstruct)->Revision == 0)
            ACPISDTtmp = (struct ACPISDT*)(((struct RSDT*)RSDTstruct)->sdtptr+i);
        else if (((struct RSDP*)RSDPstruct)->Revision > 0)
            ACPISDTtmp = (struct ACPISDT*)(((struct XSDT*)RSDTstruct)->stdptr+i);

        if (strncmp(ACPISDTtmp->Signature, signature, 4) == 0)
            if (checksumHeaderACPI((unsigned int*)ACPISDTtmp) == 0)
                return (void*)ACPISDTtmp;
    }

    return NULL;
}

void* ACPIcontrol(int action){
    if (action == ACPI_CONTROL_FIND_FADT){
        // Find the RSDP
        void* RSDPstruct;
        void* FADTstruct;
        if ((RSDPstruct = findRSDPinEBDA()) == NULL)
            if ((RSDPstruct = findRSDPinEXTMEM()) == NULL)
                return NULL;

        if ((FADTstruct = findSDT(RSDPstruct, "FACP")) == NULL)
            return NULL;
        else
            return FADTstruct;

    }
    return NULL;
}