/*
    @author = ngx123
    @brief = ACPI hardware detection driver internal header
*/


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


/*
    @brief = Function to checksum RSDP structure
    @param RSDPstruct = pointer to RSDP structure to checksum
    @return = 0 on success, -1 on fail
*/
static int checksumRSDP(void* RSDPstruct);

/*
    @brief = Function to checksum ACPI Header struct
    @param voidPtr = pointer to ACPI header struct to checksum
    @return = 0 on success, -1 on fail
*/
static int checksumHeaderACPI(void* voidPtr);

/*
    @brief = Finds pointer to RSDP in EBDA
    @return = pointer to RSDP struct on success, NULL on fail
*/
static void* findRSDPinEBDA();

/*
    @brief = Finds pointer to RSDP in Extended EBDA
    @return = pointer to RSDP struct on success, NULL on fail
*/
static void* findRSDPinEXTMEM();

/*
    @brief = Find chosen selected SDT in memory using pointer in RSDT
    @param RSDPstruct = pointer to RSDT that should be used for searching
    @param signature = string(signature) to search for in RSDT
    @return = address to selected SDT on success, NULL on fail
*/
static void* findSDT(void* RSDPstruct, char* signature);
#endif