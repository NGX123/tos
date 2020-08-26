// File: acpi.h
// Description: includes the declarations of ACPI structs and declarations

#ifndef ACPI_H
#define ACPI_H
#include <stdint.h>

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

// Contains header for ACPI structures
struct ACPISDT {
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
};

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

// Find the RSDT by RSDP
extern void findSDT(struct RSDP* RSDPstruct);
#endif