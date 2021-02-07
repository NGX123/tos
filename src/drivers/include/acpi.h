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
#include <arch/io_x86.h>


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
    struct SDTheader h;
    uint32_t sdtptr;
};

struct XSDT // For ACPIv2
{
    struct SDTheader h;
    uint64_t stdptr;
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
    @brief = Saves all the ACPI tables in a known memory location
    @param RSDPptr = pointer to the RSDP pointer struct to get SDTs using it
    @return = 0 on success, -1 on fail
*/
static int saveSDT(void* RSDPptr);
#endif