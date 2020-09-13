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
    uint32_t RsdtAddress;
} __attribute__ ((packed));

// ACPIv2 RSPD structure
struct RSDP2 {
 struct RSDP firstPart;
 
 uint32_t Length;
 uint64_t XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed));

// RSDT structure
struct RSDT {
    struct ACPISDT h;
    uint32_t sdtptr;
};

// XSDT structure(for ACPIv2)
struct XSDT {
    struct ACPISDT h;
    uint32_t stdptr;
};


// Finds pointer to RSDP in EBDA
extern struct RSDP* findRSDPinEBDA();

// Finds pointer to RSDP in Extra Memory
extern struct RSDP* findRSDPinEXTMEM();

// Find the different SDTs using RSDT
extern void* findSDT(struct RSDP* RSDPstruct, char* signature);


#endif