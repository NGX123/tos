### VARIABLES ###
## Global ##
BUILDDIR = ./build
TARGET = main
ARCH = x86_64
OVMF = /usr/share/OVMF

## EFI ##
EFI_INCLUDE_PATH 	= /usr/include/efi 																		# Contains all of the headers necessery to compiler EFI apps
EFI_LINK_OBJ 		= /usr/lib/crt0-efi-$(ARCH).o															# Shared library to link the UEFI header functions
EFI_LINK_SCRIPT 	= /usr/lib/elf_$(ARCH)_efi.lds															# GNU-EFI custom linker script for linking with it's shared libraries
EFI_INCS 			= -I /usr/include/efi -I /usr/include/efi/$(ARCH) -I /usr/include/efi/protocol			# Includes general UEFI headers, platform specific UEFI headers and UEFI protocol headers
EFI_OBJS			= -T $(EFI_LINK_SCRIPT) -L /usr/lib64/gnuefi -L /usr/lib -L /usr/lib64 $(EFI_LINK_OBJ)	# Adds objects and libs for linker to link the UEFI app with

## Compilation ##
# Clang #
CC_CLANG = clang # Can be changed to gcc, then change -DHAVE_USE_MS_ABI to -DEFI_FUNCTION_WRAPPER
LD_CLANG = ld
#Flags - (-fno-stack-protector)Disable monitoring variable at stack end to indicate overflow if overwritten; (-fpic)Position independent code(can be loaded anywhere in memory); (-fshort-wchar)Makes wchar_t short(2 byte), (-mno-red-zone)Disables feature according to ABI; (-DHAVE_USE_MS_ABI)Use windows function calling convention(it is used by UEFI)
CFLAGS_CLANG 		= $(EFI_INCS) -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -DHAVE_USE_MS_ABI -Wall -Wextra -c
#Flags - (-nostdlib)Do not link against any libs except specified; (-shared)Create a shared lib; (-Bsymbolic)Bind global function references locally, (-l efi, -l gnuefi)Link with GNU EFI libraries
LDFLAGS_CLANG 		= $(EFI_OBJS) -nostdlib -znocombreloc -shared -Bsymbolic -l efi -l gnuefi

# Objcpy #
OBJ_CREATOR = objcopy
#Flags - (-j)Only copy the specified section to output; (--target=efi-app-x86_64)Select the platform
OBJCPYFLAGS = -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64



### COMPILATION ###
# Main build proccess #
all: clang disk qemu

# Clang build proccess #
clang:
	mkdir -p $(BUILDDIR)
	$(CC_CLANG) 			$(CFLAGS_CLANG) 		$(TARGET).c
	mv 												*.o							$(BUILDDIR)
	$(LD_CLANG) 			$(LDFLAGS_CLANG) 		$(BUILDDIR)/$(TARGET).o 	-o $(BUILDDIR)/$(TARGET).so
	$(OBJ_CREATOR) 			$(OBJCPYFLAGS) 			$(BUILDDIR)/$(TARGET).so 	$(BUILDDIR)/$(TARGET).efi



### RUN ###
# Create boot drive #
disk:
	mkdir -p $(BUILDDIR)

	# Create an empty disk image with size of 94 KB
	dd if=/dev/zero of=$(BUILDDIR)/$(TARGET).img bs=512 count=93750

	# Format the disk image with 1440 byte sectors
	mformat -i $(BUILDDIR)/$(TARGET).img -f 1440 ::

	# Create EFI and then EFI/BOOT directory on the disk image
	mmd -i $(BUILDDIR)/$(TARGET).img ::/EFI
	mmd -i $(BUILDDIR)/$(TARGET).img ::/EFI/BOOT

	# Copy the efi executable to the EFI/BOOT folder on the drive
	mcopy -i $(BUILDDIR)/$(TARGET).img $(BUILDDIR)/$(TARGET).efi ::/EFI/BOOT

# Run in qemu #
qemu:
	# Copy over the code and variable vars for OVMF
	cp $(OVMF)/OVMF_CODE.fd $(OVMF)/OVMF_VARS.fd $(BUILDDIR)

	# Run qemu - (-drive ...img) Boot our created disk image; (-net none) No network boot; (-cpu qemu64) Run with 64bit CPU; (-drive ...fd) Insert our compiled UEFI code and variable space
	qemu-system-x86_64 -drive format=raw,file=$(BUILDDIR)/$(TARGET).img \
	-cpu qemu64 -net none \
	-drive if=pflash,format=raw,unit=0,file=$(BUILDDIR)/OVMF_CODE.fd -drive if=pflash,format=raw,unit=1,file=$(BUILDDIR)/OVMF_VARS.fd



# Clean the unneeded files #
clean:
	rm -rf *.o *.so *.efi *.img *iso *.fd build
