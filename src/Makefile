# Cross-compiler 
CC = $(HOME)/opt/cross-compiler/bin/i686-elf-gcc
AS = nasm

ASFLAGS = -felf32
CFLAGS = -I libs/headers -std=gnu99 -ffreestanding -nostdlib -O2 -Wall -Wextra -c
LDFLAGS = -T make/linker.ld -ffreestanding -nostdlib -O2  -lgcc


# No Cross-compiler(Look in little osdev book for flags)
CC_NOCROSS = gcc 		
AS_NOCROSS = nasm

ASFLAGS_NOCROSS = -felf32
CFLAGS_NOCROSS = -I libs/headers -ffreestanding -nostdlib -nodefaultlibs -nostartfiles -fno-builtin -fno-stack-protector -Wall -Wextra -m32 -c		# -nostdinc
LDFLAGS_NOCROSS = -T make/linker.ld -ffreestanding -nostdlib -m elf_i386																								


SOURCES = drivers/*.c kernel/*.c init/*.c libs/*.c #playground/*.c
OBJECTS = *.o #boot.o kernel.o


# Main compilation command with cross-compiler
all:
	$(AS) $(ASFLAGS) init/boot.s -o boot.o
	$(CC) $(CFLAGS) $(SOURCES)
	$(CC) $(LDFLAGS) $(OBJECTS) -o os.o

	mkdir -p iso/boot/grub
	mv os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o os.iso iso

# Main compilation command with system compiler
all-nocross:
	$(AS_NOCROSS) $(ASFLAGS_NOCROSS) init/boot.s -o boot.o
	$(CC_NOCROSS) $(CFLAGS_NOCROSS) $(SOURCES)
	ld $(LDFLAGS_NOCROSS) $(OBJECTS) -o os.o

	mkdir -p iso/boot/grub
	mv os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o os.iso iso


# Qemu start options 
qemu:
	qemu-system-i386 -cdrom os.iso
qemu-serial:
	qemu-system-i386 -cdrom os.iso -serial file:serial.log


# Cleans objects after the 
clean:
	rm -rf *.o *.out *iso 

