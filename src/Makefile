### VARIABLES ###
## Global ##
BUILDDIR = ./build
SOURCES = drivers/*.c kernel/*.c init/*.c libs/*.c
OBJECTS = $(BUILDDIR)/*.o
CFLAGS_GLOBAL = -Wall -Wextra -c
CEXTRAWARNINGS = -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused # -Wredundant-decls(warns when something was redefined several times)

## Compilation ##
# x86-32 toolchain variables #
CC_x86-32 = i686-elf-gcc
AS_x86-32 = nasm
ASFLAGS_x86-32 = -felf32													# (-felf32) Set the target architecture to x86_32-elf
CFLAGS_x86-32 = -I libs/headers -std=gnu99 -ffreestanding $(CFLAGS_GLOBAL)	# (-I libs) Include the custo c library; (-std=gnu99) Becuase of code specifics should be older standard; (-ffreestanding) Startup not at main() and no standard lib
LDFLAGS_x86-32 = -T make/linker.ld -ffreestanding -nostdlib -l gcc			# (-T script.ld) Use custom linker script; (-ffreestanding)Startup not at main() and no standard lib; (-nostdlib) Do not link with standard library; (-l gcc) Link with gcc library

# x86-64 toolchain variables #
CC_x86-64 = x86_64-elf-gcc
AS_x86-64 = nasm
CFLAGS_x86-64 = -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 $(CFLAGS_GLOBAL)	# (-ffreestanding) Startup not at main() and no standard lib; (-mcmodel=large) No assumptions about addresses; (-mno-red-zone -mno-mmx -mno-sse -mno-sse2) Disable CPU specific features that could generate runtime errors
LDFLAGS_x86-64 = -T uefi/linker.ld -ffreestanding -nostdlib -l gcc											# (-T script.ld) Use custom linker script; (-ffreestanding)Startup not at main() and no standard lib; (-nostdlib) Do not link with standard library; (-l gcc) Link with gcc library



### START ###
all: x86-64



### BUILD ###
# Compile code for x86-64 #
x86-64:
	mkdir -p $(BUILDDIR)

# Compile code for x86-32 #
x86-32:
	mkdir 			-p 									$(BUILDDIR)

	$(AS_x86-32) 	$(ASFLAGS_x86-32) 	init/boot.s 	-o 	$(BUILDDIR)/boot.o
	$(CC_x86-32) 	$(CFLAGS_x86-32) 	$(SOURCES)
	mv 									*.o 			$(BUILDDIR)
	$(CC_x86-32) 	$(LDFLAGS_x86-32) 	$(OBJECTS)		-o $(BUILDDIR)/os.o

	mkdir -p iso/boot/grub
	mv $(BUILDDIR)/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o $(BUILDDIR)/os.iso iso



### QEMU ###
# Start qemu on 64-bit UEFI-only mode CPU #
qemu_x86-64-uefi:
	mkdir -p $(BUILDDIR)

	cp /usr/share/OVMF/OVMF_CODE.fd /usr/share/OVMF/OVMF_VARS.fd $(BUILDDIR)
	qemu-system-x86_64 -cpu qemu64 \
	-drive if=pflash,format=raw,unit=0,file=$(BUILDDIR)/OVMF_CODE.fd,readonly=on \
	-drive if=pflash,format=raw,unit=1,file=$(BUILDDIR)/OVMF_VARS.fd \
	-net none \
	-cdrom $(BUILDDIR)/os.iso

# Start qemu on 32-bit BIOS-mode CPU #
qemu_x86-32-bios:
	qemu-system-i386 -cdrom $(BUILDDIR)/os.iso



### SPECIAL ###
# Toolchain Build #
toolchain:
	read -p "Are you sure you want to compile the toolchain: " toolchain_compile
	chmod +x make/toolchain.sh
	make/toolchain.sh

# Build clean up #
clean:
	rm -rf *.o *.so *.out *.img *.fd *.efi *iso $(BUILDDIR)



### EXTRA ###
# MacOS #
x86-32-macos:
	mkdir -p $(BUILDDIR)

	$(AS) $(ASFLAGS) init/boot.s -o $(BUILDDIR)/boot.o
	$(CC) $(CFLAGS) $(SOURCES)
	mv *.o $(BUILDDIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(BUILDDIR)/os.o

	mkdir -p iso/boot/grub
	mv $(BUILDDIR)/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
grub-macos:
	grub-mkrescue -o $(BUILDDIR)/os.iso iso
