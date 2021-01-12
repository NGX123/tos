/*
    @author = ngx123
    @brief = ACPI hardware detection driver internal header
*/


#ifndef ACPI_DEFINES_H
#define ACPI_DEFINES_H


#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <drivers/acpi.h>
#include <drivers/x86.h>


struct RSDP     // For ACPIv1
{
    char Signature[8];
    uint8_t Checksum;
    char OEMID[6];
    uint8_t Revision;
    void* RsdtAddress;
} __attribute__ ((packed));

struct RSDP2    // For ACPIv2
{
 struct RSDP firstPart;

 uint32_t Length;
 void* XsdtAddress;
 uint8_t ExtendedChecksum;
 uint8_t reserved[3];
} __attribute__ ((packed));

struct RSDT // For ACPIv1
{
    struct ACPISDT h;
    void* sdtptr;
};

struct XSDT // For ACPIv2
{
    struct ACPISDT h;
    void* stdptr;
};


/*
    @brief = Function to checksum RSDP structure
    @param RSDPptr = pointer to RSDP structure to checksum
    @return = 0 on success, -1 on fail
*/
static int checksumRSDP(void* RSDPptr);

/*
    @brief = Function to checksum ACPI Header struct
    @param STDptr = pointer to ACPI header struct to checksum
    @return = 0 on success, -1 on fail
*/
static int checksumSDT(void* STDptr);

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
    @param RSDPptr = pointer to RSDT that should be used for searching
    @param signature = string(signature) to search for in RSDT
    @return = address to selected SDT on success, NULL on fail
*/
static void* findSDT(void* RSDPptr, char* signature);
#endif