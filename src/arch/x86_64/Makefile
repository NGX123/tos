# @author = ngx123
# @brief = a make file with instructions to build tOS for x86_64


## Variables ##
BUILDDIR_x86_64				= $(BUILDDIR)/x86_64
INIT_SOURCES_x86_64			= arch/x86_64
SOURCES_x86_64				=  kernel/kernel64_temp.c lib/arch/io_x86.c drivers/serial.c lib/*.c
ASM_SOURCES_x86_64 			= $(INIT_SOURCES_x86_64)/init-x86_64.s
OBJECTS_x86_64				= $(BUILDDIR_x86_64)/*.o

BOOTLOADER_DIR_TOMATBOOT 	= ../tomatboot

BOOTSTRAP_DIR_MULTIBOOT2	= boot/multiboot2
BOOTSTRAP_DIR_STIVALE2		= boot/stivale2/

UEFI_BOOTLOADER 			= tomatboot

# Include the bootstrap directories into sources and define the needed macros depending on the bootloader
ifeq ($(UEFI_BOOTLOADER), grub)
SOURCES_x86_64 					+= $(BOOTSTRAP_DIR_MULTIBOOT2)/*.c
ASSEMBLY_DEFINES 				+= -D ENABLE_MULTIBOOT2_FRAMEBUFFER -D ENABLE_64_BIT_BOOTSTRAP
ASSEMBLY_BOOTSTRAP_SOURCES		+= $(BOOTSTRAP_DIR_MULTIBOOT2)/multiboot2_bootstrap.s
endif
ifeq ($(UEFI_BOOTLOADER), tomatboot)
SOURCES_x86_64 += $(BOOTSTRAP_DIR_STIVALE2)/*.c
endif

# x86_64_gcc
CC_x86_64_GCC 					= $(TOOLCHAIN_PREFIX)/x86_64-elf-gcc
AS_x86_64_GCC 					= nasm
LD_x86_64_GCC					= $(TOOLCHAIN_PREFIX)/x86_64-elf-gcc
ASFLAGS_x86_64_GCC				= -felf64 $(ASSEMBLY_DEFINES)
CFLAGS_x86_64_GCC 				= -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 $(CFLAGS_GLOBAL)	# (-ffreestanding) Startup not at main() and no standard lib; (-mcmodel=large) No assumptions about addresses; (-mno-red-zone -mno-mmx -mno-sse -mno-sse2) Disable CPU specific features that could generate runtime errors
LDFLAGS_x86_64_GCC 				= -T $(INIT_SOURCES_x86_64)/link-x86_64.ld -ffreestanding -nostdlib -l gcc											# (-T script) Use custom linker script; (-ffreestanding)Startup not at main() and no standard lib; (-nostdlib) Do not link with standard library; (-l gcc) Link with gcc library

# x86_64_clang
CC_x86_64_CLANG 			= clang
AS_x86_64_CLANG				= nasm
LD_x86_64_CLANG				= clang
ASFLAGS_x86_64_CLANG		= -felf64 $(ASSEMBLY_DEFINES)
# Attributes - (-ffreestanding, -nostdlib) Startup not at main() and no standard lib; (-mno-*) disable different compiler features; (-mcmodel=large) No assumptions about addresses; (-target) specify target arch; (-fuse-lld) choose alternative linker
CFLAGS_x86_64_CLANG 		= -I include -ffreestanding -target x86_64-elf -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 $(CFLAGS_GLOBAL)
LDFLAGS_x86_64_CLANG		= -T $(INIT_SOURCES_x86_64)/link-x86_64.ld -ffreestanding -nostdlib -fuse-ld=lld


## Compilation instructions ##
# x86_64 clang #
build_x86_64_clang:
	rm -rf $(BUILDDIR_x86_64)
	mkdir -p $(BUILDDIR_x86_64)

	# Bootstrap
	$(AS_x86_64_CLANG) 	$(ASFLAGS_x86_64_CLANG) 	$(ASSEMBLY_DEFINES) 	$(ASM_SOURCES_x86_64)							-o $(BUILDDIR_x86_64)/boot.o
ifdef ASSEMBLY_BOOTSTRAP_SOURCES
	$(AS_x86_64_CLANG) 	$(ASFLAGS_x86_64_CLANG) 	$(ASSEMBLY_DEFINES) 	$(ASSEMBLY_BOOTSTRAP_SOURCES)					-o $(BUILDDIR_x86_64)/bootloader.o
endif

	$(CC_x86_64_CLANG)	$(CFLAGS_x86_64_CLANG)								$(SOURCES_x86_64) -I include
	mv *.o $(BUILDDIR_x86_64)
	$(LD_x86_64_CLANG)	$(LDFLAGS_x86_64_CLANG)								$(OBJECTS_x86_64)								-o $(BUILDDIR_x86_64)/$(TARGET).o


## x86_64 UEFI Readable Disk with bootloader app build ##
disk_x86_64-uefi:
	mkdir -p $(BUILDDIR_x86_64)

	# Create an empty disk image with size of 94 KB
	dd if=/dev/zero of=$(BUILDDIR_x86_64)/$(TARGET).img bs=512 count=500000

	# Format the disk image with 1440 byte sectors, if problems occur add -f 1440
	mformat -i $(BUILDDIR_x86_64)/$(TARGET).img ::

	# Create EFI and then EFI/BOOT directory on the disk image
	mmd -i $(BUILDDIR_x86_64)/$(TARGET).img ::/EFI
	mmd -i $(BUILDDIR_x86_64)/$(TARGET).img ::/EFI/BOOT

	# Create the uefi app of selected bootloader
	make $(UEFI_BOOTLOADER)

	# Copy the efi bootloader to the EFI/BOOT folder on the drive
	mcopy -i $(BUILDDIR_x86_64)/$(TARGET).img $(BUILDDIR_x86_64)/BOOTX64.EFI ::/EFI/BOOT

	# Copy the kernel/executable to the drive
	mcopy -i $(BUILDDIR_x86_64)/$(TARGET).img $(BUILDDIR_x86_64)/$(TARGET).o ::/EFI/BOOT

# Create the GRUB Uefiapplication/Bootloader Binary
grub:
	grub2-mkstandalone -O x86_64-efi -o $(BUILDDIR_x86_64)/BOOTX64.EFI "boot/grub/grub.cfg=$(BOOTSTRAP_DIR_MULTIBOOT2)/grub.cfg"	|| \
	grub-mkstandalone -O x86_64-efi -o $(BUILDDIR_x86_64)/BOOTX64.EFI "boot/grub/grub.cfg=$(BOOTSTRAP_DIR_MULTIBOOT2)/grub.cfg"

# Create the tomatboot Uefi application/Bootloader Binary
tomatboot:
	make -C $(BOOTLOADER_DIR_TOMATBOOT)
	cp $(BOOTLOADER_DIR_TOMATBOOT)/bin/BOOTX64.EFI $(BUILDDIR_x86_64)

	mcopy -i $(BUILDDIR_x86_64)/$(TARGET).img $(BOOTSTRAP_DIR_STIVALE2)/limine.cfg ::


## Run with qemu-system-x86_64 with UEFI(if problems occur add this flag - "-cpu qemu64") ##
qemu_x86_64-uefi:
	cp /usr/share/OVMF/OVMF_CODE.fd /usr/share/OVMF/OVMF_VARS.fd $(BUILDDIR_x86_64)
	qemu-system-x86_64 \
	-drive if=pflash,format=raw,unit=0,file=$(BUILDDIR_x86_64)/OVMF_CODE.fd,readonly=on \
	-drive if=pflash,format=raw,unit=1,file=$(BUILDDIR_x86_64)/OVMF_VARS.fd \
	-net none \
	-drive format=raw,file=$(BUILDDIR_x86_64)/$(TARGET).img \
	$(QEMU_FLAGS)
	# QEMU_FLAGS="-serial file:$(BUILDDIR_x86_64)/serial.log"


## Main Compilation Routines ##
all_x86_64_clang:	clean build_x86_64_clang disk_x86_64-uefi qemu_x86_64-uefi