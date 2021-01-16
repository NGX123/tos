/*
    @author = ngx123
    @brief = ACPI hardware detection driver
*/


#include "include/acpi.h"


/* WARNING - DOES, NOT WORK AND SHOULD BE MALLOCED BEFORE USAGE*/
void* tableArray;   // The array to where ACPI structs will be copied


static int checksumRSDP(void* RSDPptr)
{
    int i;
    uint8_t check = 0;
    uint8_t* RSDPptr_uint8 = (uint8_t*)RSDPptr;

    if (((struct RSDP*)RSDPptr)->Revision == 0)     // Checksum ACPI v1
    {
        for (i = 0; i < (int)sizeof(struct RSDP); i++)
        {
            check += *RSDPptr_uint8;
            RSDPptr_uint8++;
        }
    }
    else if (((struct RSDP*)RSDPptr)->Revision > 0)  // Checksum ACPI v2+
    {
        for (i = 0; i < (int)sizeof(struct RSDP2); i++)
        {
            check += *RSDPptr_uint8;
            RSDPptr_uint8++;
        }
    }

    if (check != 0)
        return -1;

    return 0;
}

static int checksumSDT(void* SDTptr)
{
    uint32_t* SDTptr_uint32 = (uint32_t*)SDTptr;

    char *checkPtr = (char*) SDTptr_uint32;
    int len = *(SDTptr_uint32 + 1);
    char check = 0;

    while (0 < len--)
    {
        check += *checkPtr;
        checkPtr++;
    }

    if (check != 0)
        return -1;

    return 0;
}

static void* findRSDPinEBDA()
{
    volatile char* EBDAaddr = (volatile char*)0x40E;
    volatile char* EBDAptr = (volatile char*)((int)*EBDAaddr);

    void* RSDPptr;
    char signature[9];
    int i = 0;
    int j = 0;
    int k = 0;

    for (i = 0; i != 1023; i++)
    {
        if (EBDAptr[i] == 'R'){
            for (j = i, k = 0; j != (i+8); j++, k++)
                signature[k] = EBDAptr[j];
            signature[k] = 0;

            if (strcmp("RSD PTR ", signature) == 0)
            {
                RSDPptr = (void*)&EBDAptr[i];
                if (checksumRSDP(RSDPptr) == 0)
                    return RSDPptr;
            }
        }
    }
    return NULL;
}

static void* findRSDPinEXTMEM()
{
    volatile char* EXTMEMstart = (volatile char*)0x000E0000;
    volatile char* EXTMEMend = (volatile char*)0x000FFFFF;

    void* RSDPptr;
    char signature[9];
    int i;
    int j;
    int k;

    for (i = 0; &EXTMEMstart[i] != EXTMEMend; i++)
    {
        if (EXTMEMstart[i] == 'R')
        {
            for (j = i, k = 0; j != (i+8); j++, k++)
                signature[k] = EXTMEMstart[j];
            signature[k] = 0;

            if (strcmp("RSD PTR ", signature) == 0)
            {
                RSDPptr = (void*)&EXTMEMstart[i];
                if (checksumRSDP(RSDPptr) == 0)
                    return RSDPptr;
            }
        }
    }
    return NULL;
}

static int saveSDT(void* RSDPptr){
    int i, j;
    int SDTentriesAmount;
    void* RSDTptr;
    struct SDTheader* SDTptr;

    if (((struct RSDP*)RSDPptr)->Revision == 0)     // Case for ACPI v1
    {
        RSDTptr = (struct RSDT*) (((struct RSDP*)RSDPptr)->RsdtAddress);
        if (checksumSDT((void*)RSDTptr) != 0)
            return -1;
        SDTentriesAmount = (((struct RSDT*)RSDTptr)->h.Length - sizeof(struct SDTheader)) / 4;
    }
    else if (((struct RSDP*)RSDPptr)->Revision > 0)  // Case for ACPI v2+
    {
        RSDTptr = (struct XSDT*) (((struct RSDP2*)RSDPptr)->XsdtAddress);
        if (checksumSDT((void*)RSDTptr) != 0)
            if (strncmp(((struct XSDT*)RSDTptr)->h.Signature, "XSDT", 4) != 0)
                return -1;
        SDTentriesAmount = (((struct XSDT*)RSDTptr)->h.Length - sizeof(struct SDTheader)) / 8;
    }
    else
        return -1;

    if (((struct RSDP*)RSDPptr)->Revision == 0)
        SDTptr = (struct SDTheader*)(((struct RSDT*)RSDTptr)->sdtptr);
    else if (((struct RSDP*)RSDPptr)->Revision > 0)
        SDTptr = (struct SDTheader*)(((struct XSDT*)RSDTptr)->stdptr);

    return 0;
}

int retrieveSDT(void* buf, int size, char* signature){

    return 0;
}

int ACPIinit()
{
    void* RSDPptr;
    struct FADT FADTptr;

    if ((RSDPptr = findRSDPinEBDA()) == NULL)
        if ((RSDPptr = findRSDPinEXTMEM()) == NULL)
            return -1;

    saveSDT(RSDPptr);
    // if (saveSDT(RSDPptr) != -1)
    // {
    //     retrieveSDT(&FADTptr, sizeof(FADTptr), "FACP");
    //     outb(FADTptr.SMI_CommandPort, FADTptr.AcpiEnable);
    // }
    // else
    //     return -1;

    return 0;
}


// static void* findSDTinMem(void* RSDPptr, char* signature)
// {
//     int i;
//     int SDTentriesAmount;
//     void* RSDTptr;
//     struct SDTheader* SDTptr;

//     if (((struct RSDP*)RSDPptr)->Revision == 0)     // Case for ACPI v1
//     {
//         RSDTptr = (struct RSDT*) (((struct RSDP*)RSDPptr)->RsdtAddress);

//         if (checksumSDT((void*)RSDTptr) != 0)
//             return NULL;

//         SDTentriesAmount = (((struct RSDT*)RSDTptr)->h.Length - sizeof(struct SDTheader)) / 4;
//     }
//     else if (((struct RSDP*)RSDPptr)->Revision > 0)  // Case for ACPI v2+
//     {
//         RSDTptr = (struct XSDT*) (((struct RSDP2*)RSDPptr)->XsdtAddress);

//         if (checksumSDT((void*)RSDTptr) != 0)
//             return NULL;

//         if (strncmp(((struct XSDT*)RSDTptr)->h.Signature, "XSDT", 4) != 0)
//             return NULL;

//         SDTentriesAmount = (((struct XSDT*)RSDTptr)->h.Length - sizeof(struct SDTheader)) / 8;
//     }
//     else
//         return NULL;

//     for (i = 0; i < SDTentriesAmount; i++)
//     {
//         if (((struct RSDP*)RSDPptr)->Revision == 0)
//             SDTptr = (struct SDTheader*)(((struct RSDT*)RSDTptr)->sdtptr+i);
//         else if (((struct RSDP*)RSDPptr)->Revision > 0)
//             SDTptr = (struct SDTheader*)(((struct XSDT*)RSDTptr)->stdptr+i);

//         if (strncmp(SDTptr->Signature, signature, 4) == 0)
//             if (checksumSDT((unsigned int*)SDTptr) == 0)
//                 return (void*)SDTptr;
//     }

//     return NULL;
// }