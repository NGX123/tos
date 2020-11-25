# Cross-compiler
CC = i686-elf-gcc
AS = nasm

ASFLAGS = -felf32
CFLAGS = -I libs/headers -std=gnu99 -ffreestanding -nostdlib -O2 -Wall -Wextra -c
CEXTRAWARNINGS = -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused
LDFLAGS = -T make/linker.ld -ffreestanding -nostdlib -O2  -lgcc


# No Cross-compiler(Look in little osdev book for flags)
CC_NOCROSS = gcc
AS_NOCROSS = nasm

ASFLAGS_NOCROSS = -felf32
CFLAGS_NOCROSS = -I libs/headers -ffreestanding -nostdlib -nodefaultlibs -nostartfiles -fno-builtin -fno-stack-protector -Wall -Wextra -m32 -c		# -nostdinc
LDFLAGS_NOCROSS = -T make/linker.ld -ffreestanding -nostdlib -m elf_i386


SOURCES = drivers/*.c kernel/*.c init/*.c libs/*.c #playground/*.c
OBJECTS = build/*.o


# Main compilation command with cross-compiler
all:
	mkdir build
	$(AS) $(ASFLAGS) init/boot.s -o build/boot.o
	$(CC) $(CFLAGS) $(SOURCES)
	mv *.o ./build
	$(CC) $(LDFLAGS) $(OBJECTS) -o build/os.o

	mkdir -p iso/boot/grub
	mv build/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o build/os.iso iso

# Main compilation command with system compiler
all-nocross:
	$(AS_NOCROSS) $(ASFLAGS_NOCROSS) init/boot.s -o build/boot.o
	$(CC_NOCROSS) $(CFLAGS_NOCROSS) $(SOURCES)
	mv *.o ./build
	ld $(LDFLAGS_NOCROSS) $(OBJECTS) -o build/os.o

	mkdir -p iso/boot/grub
	mv build/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o build/os.iso iso

# Compiler with a ton of extra warnings
all-extrawarning:
	$(AS) $(ASFLAGS) init/boot.s -o build/boot.o
	$(CC) $(CEXTRAWARNINGS) $(CFLAGS) $(SOURCES)
	mv *.o ./build
	$(CC) $(LDFLAGS) $(OBJECTS) -o build/os.o

	mkdir -p iso/boot/grub
	mv build/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o build/os.iso iso

# MacOS
all-macos:
	$(AS) $(ASFLAGS) init/boot.s -o build/boot.o
	$(CC) $(CFLAGS) $(SOURCES)
	mv *.o ./build
	$(CC) $(LDFLAGS) $(OBJECTS) -o build/os.o

	mkdir -p iso/boot/grub
	mv build/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
grub-macos:
	grub-mkrescue -o build/os.iso iso

# Qemu start options
qemu:
	qemu-system-i386 -cdrom build/os.iso
qemu-serial:
	qemu-system-i386 -cdrom build/os.iso -serial file:serial.log


# Cleans objects after the
clean:
	rm -rf *.o *.out *iso build
