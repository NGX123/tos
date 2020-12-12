SOURCES = uefi-app.c
OBJECTS = uefi-app.o
ARCH = x86_64
OVMF = /usr/share/OVMF


EFI_INCLUDE_PATH 	= /usr/include/efi 																# Contains all of the headers necessery to compiler EFI apps
EFI_LINK_OBJ 		= /usr/lib/crt0-efi-$(ARCH).o													# Shared library to link the UEFI header functions
EFI_LINK_SCRIPT 	= /usr/lib/elf_$(ARCH)_efi.lds													# GNU-EFI custom linker script for linking with it's shared libraries
EFI_INCS 			= -I /usr/include/efi -I /usr/include/efi/$(ARCH) -I /usr/include/efi/protocol	# Includes general UEFI headers, platform specific UEFI headers and UEFI protocol headers
EFI_OBJS			= -T $(EFI_LINK_SCRIPT) -L /usr/lib $(EFI_LINK_OBJ)								# Adds objects and libs for linker to link the UEFI app with


## Clang ##
CC_CLANG = clang
LD_CLANG = ld

# Flags - (-fno-stack-protector)Disable monitoring variable at stack end to indicate overflow if overwritten; (-fpic)Position independent code(can be loaded anywhere in memory); (-fshort-wchar)Makes wchar_t short(2 byte), (-mno-red-zone)Disables feature according to ABI; (-DHAVE_USE_MS_ABI)Use windows function calling convention(it is used by UEFI)
CFLAGS_CLANG 		= $(EFI_INCS) -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -DHAVE_USE_MS_ABI -Wall -Wextra -c
# Flags - (-nostdlib)Do not link against any libs except specified; (-shared)Create a shared lib; (-Bsymbolic)Bind global function references locally, (-lefi, -lgnuefi)Link with GNU EFI libraries
LDFLAGS_CLANG 		= $(EFI_OBJS) -nostdlib -znocombreloc -shared -Bsymbolic #-lefi -lgnuefi


## GCC ##
CC_GCC = x86_64-elf-gcc
LD_GCC = ld

CFLAGS_GCC 	= $(EFI_INCS) -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -DEFI_FUNCTION_WRAPPER -ffreestanding -nostdlib -Wall -Wextra
LDFLAGS_GCC = $(EFI_OBJS) -nostdlib -znocombreloc -Bsymbolic -l:libgnuefi.a -l:libgnuefi.a -ffreestanding -nostdlib

OBJ_CREATOR = objcopy
# Flags - (-j)Only copy the specified section to output; (--target=efi-app-x86_64)Select the platform
OBJCPYFLAGS	= -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64



## COMPILATION ##
# Compile with clang
clang:
	$(CC_CLANG) 			$(CFLAGS_CLANG) 		$(SOURCES)
	$(LD_CLANG) 			$(LDFLAGS_CLANG) 		uefi-app.o 	-o uefi-app.so
	$(OBJ_CREATOR) 			$(OBJCPYFLAGS) 			uefi-app.so uefi-app.efi

# Compile with gcc
gcc:
	$(CC_GCC) 			$(CFLAGS_GCC) 		uefi-app.c 	-o uefi-app.o
	$(LD_GCC) 			$(LDFLAGS_GCC) 		uefi-app.o 	-o uefi-app.so
	$(OBJ_CREATOR) 		$(OBJCPYFLAGS) 		uefi-app.so uefi-app.efi


## RUN ##
# Make the UEFI boot disk
disk:
	dd if=/dev/zero of=fat.img bs=1k count=1440
	mformat -i fat.img -f 1440 ::
	mmd -i fat.img ::/EFI
	mmd -i fat.img ::/EFI/BOOT
	mcopy -i fat.img $(OBJECTS) ::/EFI/BOOT

	mkdir -p iso
	cp fat.img iso
	xorriso -as mkisofs -R -f -e fat.img -no-emul-boot -o cdimage.iso iso

	# dd if=/dev/zero of=uefi.img bs=512 count=93750
	# parted uefi.img -s -a minimal mklabel gpt
	# parted uefi.img -s -a minimal mkpart EFI FAT16 2048s 93716s
	# parted uefi.img -s -a minimal toggle 1 boot

	# dd if=/dev/zero of=tmp.img bs=512 count=91669
	# mformat -i tmp.img -h 32 -t 32 -n 64 -c 1

	# mcopy -i tmp.img uefi-app.efi ::

	# dd if=tmp.img of=uefi.img bs=512 count=91669 seek=2048 conv=notrunc

# Run in qemu
qemu:
	qemu-system-x86_64 -L $(OVMF) -bios $(OVMF)/OVMF_CODE.fd -cdrom cdimage.iso -net none
	#qemu-system-x86_64 -cpu qemu64 -bios $(OVMF)/OVMF_CODE.fd -drive file=uefi.img,if=ide -net none


# Clean the unneeded files
clean:
	rm -rf *.o *.so *.efi *.img *iso