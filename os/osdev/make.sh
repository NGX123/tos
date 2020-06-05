#! /bin/bash

crosss=$HOME/opt/cross-compiler/bin/i686-elf-gcc

nasm -felf32 boot.asm -o boot.o
#$HOME/opt/cross-compiler/bin/i686-elf-as boot.asm -o boot.o

$crosss -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
$crosss -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

mkdir -p iso/boot/grub
mv myos.bin iso/boot/
cp ../help/grub.cfg iso/boot/grub/

grub2-mkrescue -o myos.iso iso
qemu-system-i386 -cdrom myos.iso

rm -rf kernel.o boot.o myos.iso myos.bin iso/