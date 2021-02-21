# @author = ngx123
# @brief = a make file with instructions to build tOS for x86-32


## Variables ##
SOURCES_x86-64			= $(INIT_SOURCES_x86-64)/*.c lib/arch/io_x86.c drivers/serial.c $(MULTIBOOT2_DIR)/*.c #$(INIT_SOURCES_x86-64)/*.c
OBJECTS_x86-64			= $(BUILDDIR_x86-64)/*.o

ASSEMBLY_DEFINES		= -D ENABLE_MULTIBOOT2_FRAMEBUFFER

# x86-64_gcc
CC_x86-64_GCC 					= $(TOOLCHAIN_PREFIX)/x86_64-elf-gcc
AS_x86-64_GCC 					= nasm
LD_x86-64_GCC					= $(TOOLCHAIN_PREFIX)/x86_64-elf-gcc
ASFLAGS_x86-64_GCC				= -felf64 $(ASSEMBLY_DEFINES)
CFLAGS_x86-64_GCC 				= -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 $(CFLAGS_GLOBAL)	# (-ffreestanding) Startup not at main() and no standard lib; (-mcmodel=large) No assumptions about addresses; (-mno-red-zone -mno-mmx -mno-sse -mno-sse2) Disable CPU specific features that could generate runtime errors
LDFLAGS_x86-64_GCC 				= -T $(INIT_SOURCES_x86-64)/link-x86_64.ld -ffreestanding -nostdlib -l gcc											# (-T script) Use custom linker script; (-ffreestanding)Startup not at main() and no standard lib; (-nostdlib) Do not link with standard library; (-l gcc) Link with gcc library

# x86-64_clang
CC_x86-64_CLANG 			= clang
AS_x86-64_CLANG				= nasm
LD_x86-64_CLANG				= clang
ASFLAGS_x86-64_CLANG		= -felf64 $(ASSEMBLY_DEFINES)
CFLAGS_x86-64_CLANG 		= -I include -ffreestanding $(CFLAGS_GLOBAL) -target x86-64-elf -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2
LDFLAGS_x86-64_CLANG		= -T $(INIT_SOURCES_x86-64)/link-x86_64.ld -ffreestanding -nostdlib -target x86-64-elf


## Compilation instructions ##
# x86-64 clang #
build_x86-64_clang:
	rm -rf $(BUILDDIR_x86-64)
	mkdir -p $(BUILDDIR_x86-64)

	# Bootstrap
	$(AS_x86-64_CLANG) 	$(ASFLAGS_x86-64_CLANG) 	$(ASSEMBLY_DEFINES) 	$(INIT_SOURCES_x86-64)/init-x86_64.s			-o $(BUILDDIR_x86-64)/boot.o
	$(LD_x86-64_CLANG)	$(LDFLAGS_x86-64_CLANG)								$(OBJECTS_x86-64)								-o $(BUILDDIR_x86-64)/$(TARGET).o


## x86-64 UEFI Readable Disk with bootloader app build ##
disk_x86-64-uefi:
	mkdir -p $(BUILDDIR_x86-64)

	# Create an empty disk image with size of 94 KB
	dd if=/dev/zero of=$(BUILDDIR_x86-64)/$(TARGET).img bs=512 count=500000

	# Format the disk image with 1440 byte sectors, if problems occur add -f 1440
	mformat -i $(BUILDDIR_x86-64)/$(TARGET).img ::

	# Create EFI and then EFI/BOOT directory on the disk image
	mmd -i $(BUILDDIR_x86-64)/$(TARGET).img ::/EFI
	mmd -i $(BUILDDIR_x86-64)/$(TARGET).img ::/EFI/BOOT

	# Create the GRUB Uefiapplication/Bootloader Binary
	grub2-mkstandalone -O x86_64-efi -o $(BUILDDIR_x86-64)/BOOTX64.EFI "boot/grub/grub.cfg=$(MULTIBOOT2_DIR)/grub.cfg"	|| \
	grub-mkstandalone -O x86_64-efi -o $(BUILDDIR_x86-64)/BOOTX64.EFI "boot/grub/grub.cfg=$(MULTIBOOT2_DIR)/grub.cfg"

	# Copy the efi bootloader to the EFI/BOOT folder on the drive
	mcopy -i $(BUILDDIR_x86-64)/$(TARGET).img $(BUILDDIR_x86-64)/BOOTX64.EFI ::/EFI/BOOT

	# Copy the kernel/executable to the drive
	mcopy -i $(BUILDDIR_x86-64)/$(TARGET).img $(BUILDDIR_x86-64)/$(TARGET).o ::/EFI/BOOT


## Run with qemu-system-x86_64 with UEFI(if problems occur add this flag - "-cpu qemu64") ##
qemu_x86-64-uefi:
	cp /usr/share/OVMF/OVMF_CODE.fd /usr/share/OVMF/OVMF_VARS.fd $(BUILDDIR_x86-64)
	qemu-system-x86_64 \
	-drive if=pflash,format=raw,unit=0,file=$(BUILDDIR_x86-64)/OVMF_CODE.fd,readonly=on \
	-drive if=pflash,format=raw,unit=1,file=$(BUILDDIR_x86-64)/OVMF_VARS.fd \
	-net none \
	-drive format=raw,file=$(BUILDDIR_x86-64)/$(TARGET).img \
	$(QEMU_FLAGS)
	# QEMU_FLAGS="-serial file:$(BUILDDIR_x86-64)/serial.log"