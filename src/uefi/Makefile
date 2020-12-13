### VARIABLES ###
## Global ##
BUILDDIR = ./build
SOURCES = uefi-app.c
OBJECTS = $(BUILDDIR)/uefi-app.o
ARCH = x86_64
OVMF = /usr/share/OVMF

## EFI ##
EFI_INCLUDE_PATH 	= /usr/include/efi 																# Contains all of the headers necessery to compiler EFI apps
EFI_LINK_OBJ 		= /usr/lib/crt0-efi-$(ARCH).o													# Shared library to link the UEFI header functions
EFI_LINK_SCRIPT 	= /usr/lib/elf_$(ARCH)_efi.lds													# GNU-EFI custom linker script for linking with it's shared libraries
EFI_INCS 			= -I /usr/include/efi -I /usr/include/efi/$(ARCH) -I /usr/include/efi/protocol	# Includes general UEFI headers, platform specific UEFI headers and UEFI protocol headers
EFI_OBJS			= -T $(EFI_LINK_SCRIPT) -L /usr/lib $(EFI_LINK_OBJ)								# Adds objects and libs for linker to link the UEFI app with

## Compilation ##
# Clang #
CC_CLANG = clang
LD_CLANG = ld
#Flags - (-fno-stack-protector)Disable monitoring variable at stack end to indicate overflow if overwritten; (-fpic)Position independent code(can be loaded anywhere in memory); (-fshort-wchar)Makes wchar_t short(2 byte), (-mno-red-zone)Disables feature according to ABI; (-DHAVE_USE_MS_ABI)Use windows function calling convention(it is used by UEFI)
CFLAGS_CLANG 		= $(EFI_INCS) -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -DHAVE_USE_MS_ABI -Wall -Wextra -c
#Flags - (-nostdlib)Do not link against any libs except specified; (-shared)Create a shared lib; (-Bsymbolic)Bind global function references locally, (-lefi, -lgnuefi)Link with GNU EFI libraries
LDFLAGS_CLANG 		= $(EFI_OBJS) -nostdlib -znocombreloc -shared -Bsymbolic #-lefi -lgnuefi

# Objcpy #
OBJ_CREATOR = objcopy
#Flags - (-j)Only copy the specified section to output; (--target=efi-app-x86_64)Select the platform
OBJCPYFLAGS = -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64



### COMPILATION ###
# Clang build proccess #
clang:
	mkdir -p $(BUILDDIR)
	$(CC_CLANG) 			$(CFLAGS_CLANG) 		$(SOURCES)
	mv *.o $(BUILDDIR)
	$(LD_CLANG) 			$(LDFLAGS_CLANG) 		$(OBJECTS) 				-o $(BUILDDIR)/uefi-app.so
	$(OBJ_CREATOR) 			$(OBJCPYFLAGS) 			$(BUILDDIR)/uefi-app.so $(BUILDDIR)/uefi-app.efi



### RUN ###
# Create boot drive #
disk:
	mkdir -p $(BUILDDIR)
	dd if=/dev/zero of=$(BUILDDIR)/uefi-app.img bs=512 count=93750
	mformat -i $(BUILDDIR)/uefi-app.img -f 1440 ::
	mmd -i $(BUILDDIR)/uefi-app.img ::/EFI
	mmd -i $(BUILDDIR)/uefi-app.img ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/uefi-app.img $(BUILDDIR)/uefi-app.efi ::/EFI/BOOT

# Run in qemu #
qemu:
	cp $(OVMF)/OVMF_CODE.fd $(OVMF)/OVMF_VARS.fd $(BUILDDIR)
	qemu-system-x86_64 -drive format=raw,file=$(BUILDDIR)/uefi-app.img \
	-cpu qemu64 -m 256M -net none \
	-drive if=pflash,format=raw,unit=0,file=$(BUILDDIR)/OVMF_CODE.fd -drive if=pflash,format=raw,unit=1,file=$(BUILDDIR)/OVMF_VARS.fd



# Clean the unneeded files #
clean:
	rm -rf *.o *.so *.efi *.img *iso *.fd build
