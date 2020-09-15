// File: acpi-defines.h
// Description: internal to the acpi driver header



#ifndef ACPI_DEFINES_H
#define ACPI_DEFINES_H

#include <stdint.h>
#include "drivers/acpi.h"

// ACPIv1 RSPD structure
struct RSDP {
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    void* RsdtAddress;
} __attribute__ ((packed));

// ACPIv2 RSPD structure
struct RSDP2 {
 struct RSDP firstPart;
 
 uint32_t Length;
 void* XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed));

// RSDT structure
struct RSDT {
    struct ACPISDT h;
    void* sdtptr;
};

// XSDT structure(for ACPIv2)
struct XSDT {
    struct ACPISDT h;
    void* stdptr;
};


// Finds pointer to RSDP in EBDA
static void* findRSDPinEBDA();

// Finds pointer to RSDP in Extra Memory
static void* findRSDPinEXTMEM();

// Find the different SDTs using RSDT
static void* findSDT(void* RSDPstruct, char* signature);


#endif