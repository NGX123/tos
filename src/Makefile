CC = $(HOME)/opt/cross-compiler/bin/i686-elf-gcc
AS = nasm

ASFLAGS = -felf32
CFLAGS = -std=gnu99 -ffreestanding -nostdlib -O2 -Wall -Wextra -c
LDFLAGS = -T make/linker.ld -ffreestanding -nostdlib -O2  -lgcc

SOURCES = drivers/*.c kernel/*.c init/*.c
OBJECTS = *.o #boot.o kernel.o


# Main compilation command
all:
	$(AS) $(ASFLAGS) init/boot.s -o boot.o
	$(CC) $(CFLAGS) $(SOURCES)
	$(CC) $(LDFLAGS) $(OBJECTS) -o os.o

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
	rm -rf *.o *iso



# Test before adding to main compilaion routine
test:
	$(AS) $(ASFLAGS) init/boot.s -o boot.o

	$(CC) $(CFLAGS) drivers/*.c 
	$(CC) $(CFLAGS) kernel/*.c
	
	$(CC) $(LDFLAGS) $(OBJECTS) -o os.o
	
	mkdir -p iso/boot/grub
	mv os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o os.iso iso
