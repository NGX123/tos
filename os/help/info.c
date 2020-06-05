/*
// Prepare Environemnt
env_setup.sh


// Assembling 
nasm -felf32 boot.asm -o boot.o (NASM)
i686-elf-as boot.s -o boot.o    (GAS)



// Compiling
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

    -c -o           - input file and output file
    -std=gnu99      - choose the c standard
    -ffreestanding  - freestanding environement does not use c standard library
    -Wall           - shows all warning about the code
    -Wextra         - enables extra warning flags
    -O2             - optimization option(optional)



// Link the kernel and bootloader
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

    -nostdlib       - no use of standard lib during linking
    -ffreestanding  - freestanding environement does not use os dependent libraries
    -O2             - optimization option(optional)
    -T <script>     - specifies linker script
    -o <file>       - specifies optput file 
    boot.o kernel.o - linked objects



// Make the bootable iso
mkdir -p iso/boot/grub
cp myos.bin iso/boot/
cp grub.cfg iso/boot/grub/

/ There are two types of grubs grub and grub2, depending on which is used it may be: 
    - grub-command
    - grub2-command
grub2-mkrescue -o myos.iso iso



// Test the OS
qemu-system-i386 myos.iso       (full os)
qemu-system-i386 -cdrom myos.iso(extra)
qemu-system-i386 -kernel myos.bin(only kernel)

*/





