// File: gdt.h 
// Description: includes defenitions for loading the global, local and interrupt descriptor tables


// A gdt entry to be loaded using pointer
struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

// Pointer to the GDT, The max bytes taken up by the GDT, minus 1
struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// Our GDT, with 3 entries, and finally our special GDT pointer
struct gdt_entry gdt[3];
struct gdt_ptr gp;

// This will be a function in start.asm. We use this to properly reload the new segment registers
extern void gdt_flush();
