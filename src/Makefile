CC_bios32 = i686-elf-gcc
AS_bios32 = nasm

ASFLAGS_bios32 = -felf32
CFLAGS_bios32 = -I libs/headers -std=gnu99 -ffreestanding -nostdlib -O2 -Wall -Wextra -c
LDFLAGS_bios32 = -T make/linker.ld -ffreestanding -nostdlib -O2  -lgcc


CC_uefi64 = x86_64-elf-gcc
AS_uefi64 = nasm

ASFLAGS_uefi64 = none
CFLAGS_uefi64 = -I libs/headers -std=gnu99 -ffreestanding -nostdlib -O2 -Wall -Wextra -c
LDFLAGS_uefi64 = -T make/linker.ld -ffreestanding -nostdlib -O2 -lgcc

SOURCES = drivers/*.c kernel/*.c init/*.c libs/*.c #playground/*.c
OBJECTS = build/*.o


CEXTRAWARNINGS = -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused # -Wredundant-decls(warns when something was redefined several times)



## START ##
all: uefi64



## BUILD ##
# Compile the code for a x86 64-bit target running UEFI-only mode
uefi64:
	mkdir -p build

# Compile the code for a x86 32-bit target running BIOS
bios32:
	mkdir -p build
	$(AS_bios32) $(ASFLAGS_bios32) init/boot.s -o build/boot.o
	$(CC_bios32) $(CFLAGS_bios32) $(SOURCES)
	mv *.o ./build
	$(CC_bios32) $(LDFLAGS_bios32) $(OBJECTS) -o build/os.o

	mkdir -p iso/boot/grub
	mv build/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o build/os.iso iso



## QEMU ##
# Start qemu on 64-bit UEFI-only mode CPU
qemu_uefi64:
	mkdir -p build
	cp /usr/share/OVMF/OVMF_CODE.fd /usr/share/OVMF/OVMF_VARS.fd build
	qemu-system-x86_64 -cpu qemu64 \
	-drive if=pflash,format=raw,unit=0,file=build/OVMF_CODE.fd,readonly=on \
	-drive if=pflash,format=raw,unit=1,file=build/OVMF_VARS.fd \
	-net none \
	-cdrom build/os.iso

# Start qemu on 32-bit BIOS-mode CPU
qemu_bios32:
	qemu-system-i386 -cdrom build/os.iso



## SPECIAL ##
# Compiles and installs all the components neseccery to build and run the OS
toolchain:
	read -p "Are you sure you want to compile the toolchain: " toolchain_compile
	chmod +x make/toolchain.sh
	make/toolchain.sh

# Cleans up after the build
clean:
	rm -rf *.o *.out *iso build



## EXTRA ##
# MacOS
all-macos:
	mkdir -p build
	$(AS) $(ASFLAGS) init/boot.s -o build/boot.o
	$(CC) $(CFLAGS) $(SOURCES)
	mv *.o ./build
	$(CC) $(LDFLAGS) $(OBJECTS) -o build/os.o

	mkdir -p iso/boot/grub
	mv build/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
grub-macos:
	grub-mkrescue -o build/os.iso iso

# Compile with a ton of extra warnings
all-extrawarning:
	mkdir -p build
	$(AS) $(ASFLAGS) init/boot.s -o build/boot.o
	$(CC) $(CEXTRAWARNINGS) $(CFLAGS) $(SOURCES)
	mv *.o ./build
	$(CC) $(LDFLAGS) $(OBJECTS) -o build/os.o

	mkdir -p iso/boot/grub
	mv build/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o build/os.iso iso
