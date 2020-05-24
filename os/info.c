/*
Magic number - a 0xaa55 value that is written in the end of 512 byte sector to indicate it is 
a boot sector
Word - maximum proccessing unit in current mode of cpu, e.g. 16bit 32bit 
Hexadecimals - 
 + Each hexadecimal symbol is 4 bits long
 + Bit values - 0(forth 8) 0(third 4) 0(second 2) 0(first number 1)
 + If the binary value is 1 bit is on and represents number stated before else it is zero
 + When you got all the values you add them up, and look up the hexadecimal number that goes 
 Nth - conversion finished.
 e.g. 0110 = 0420, 0+2+4+0=6, 6th hexadecimal is 6, so 0110 = 0x6


+++ Debugging and making +++
CPU EMU - qemu(qemu-system-i386) <image>
Assembler - nasm file.asm -f format -o output.bin, format used is bin(raw binary)

Hex view
 + xxd <file.bin>
 + od -t x1 -A n <file.bin>


+++ CPU +++
16bit Real Mode - to keep backward compatibility cpu's have a 16 bit mode from where the OS
    could go to the 32/64-bit protected mode
    
Interrupts - mechanism that allows the CPU to temporarily halt what it is doing and run
    some other task before returning to the original

ISR(Interrupt Service Routine) - a sequence of machine instructions used to call an interrupt
    they are stored in a table set by BIOS at the start of memory(0x0), there is no ISR for 
    everything becuase it would be not enough, so to impact what ISR will do you need to 
    set regiser ax with a certain value

Loading - our program will be loaded into memory, but it can not be located at the start(0x0) 
    becuase it is reserved, so the BIOS puts the program into 0x7c00 memory address usually



Memory Map
x100000+ - FREE
0xC0000 - BIOS(256 KB)
0xA0000 - Video Memory(128 KB)
0x9fc00 - Extended BIOS Data Area(639 KB)
0x7e00 - Free(638 KB)
0x7c00 - Loaded Boot Sector (512 bytes)
0x500 - Free
0x400 - BIOS Data Area (256 bytes)
0x0 - Interrupt Vector Table (1 KB)


Stack - cpu has a limited amount of registers, so we cn store our temporary variable in the 
main memory, without counting the memory address each time - we do this using the stack.
The stack uses two extra registers bp and sp, first we set the start of the stack in memory
and them we can write and read from it, but important thing is that stack grows down wards
in the memory e.g. start - 0x8000 finish - 0x6ffe

Addressing - like in c if there is an address of a string it pints only to it's start
and if you add numbers to the address you move forward, for assembler label is an address
so you can add to it
*/