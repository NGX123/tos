// File: gdt.h
// Description: contains the structure for the GLobal Descriptor Table


struct gdt{
    unsigned int address;
    unsigned short size;
} __attribute__((packed));