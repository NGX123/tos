### VARIABLES ###
## Global ##
BUILDDIR 				= ./build
SOURCES 				= drivers/*.c kernel/*.c init/*.c libs/*.c
OBJECTS 				= $(BUILDDIR)/*.o
CFLAGS_GLOBAL 			= -Wall -Wextra -c
CEXTRAWARNINGS 			= -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused # -Wredundant-decls(warns when something was redefined several times)
OVMF 					= /usr/share/OVMF


## Compilation ##
# x86-32_gcc toolchain #
CC_x86-32 				= i686-elf-gcc
AS_x86-32 				= nasm
ASFLAGS_x86-32 			= -felf32													# (-felf32) Set the target architecture to x86_32-elf
CFLAGS_x86-32 			= -I libs/headers -std=gnu99 -ffreestanding $(CFLAGS_GLOBAL)	# (-I libs) Include the custo c library; (-std=gnu99) Becuase of code specifics should be older standard; (-ffreestanding) Startup not at main() and no standard lib
LDFLAGS_x86-32 			= -T make/linker.ld -ffreestanding -nostdlib -l gcc			# (-T script.ld) Use custom linker script; (-ffreestanding)Startup not at main() and no standard lib; (-nostdlib) Do not link with standard library; (-l gcc) Link with gcc library

# x86-64_gcc #
CC_x86-64_GCC 				= x86_64-elf-gcc
AS_x86-64_GCC 				= nasm
CFLAGS_x86-64_GCC 			= -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 $(CFLAGS_GLOBAL)	# (-ffreestanding) Startup not at main() and no standard lib; (-mcmodel=large) No assumptions about addresses; (-mno-red-zone -mno-mmx -mno-sse -mno-sse2) Disable CPU specific features that could generate runtime errors
LDFLAGS_x86-64_GCC 			= -T uefi/linker.ld -ffreestanding -nostdlib -l gcc											# (-T script.ld) Use custom linker script; (-ffreestanding)Startup not at main() and no standard lib; (-nostdlib) Do not link with standard library; (-l gcc) Link with gcc library

# x86-64-uefiapp_clang #
CC_x86-64-uefiapp_CLANG 				= clang # Can be changed to gcc, then change -DHAVE_USE_MS_ABI to -DEFI_FUNCTION_WRAPPER
LD_x86-64-uefiapp_CLANG 				= ld
OBJCPY 									= objcopy
x86-64-uefiapp_TARGET 					= uefi
#Flags - (-fno-stack-protector)Disable monitoring variable at stack end to indicate overflow if overwritten; (-fpic)Position independent code(can be loaded anywhere in memory); (-fshort-wchar)Makes wchar_t short(2 byte), (-mno-red-zone)Disables feature according to ABI; (-DHAVE_USE_MS_ABI)Use windows function calling convention(it is used by UEFI)
CFLAGS_x86-64-uefiapp_CLANG 			= $(EFI_INCS) -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -DHAVE_USE_MS_ABI -Wall -Wextra -c
#Flags - (-nostdlib)Do not link against any libs except specified; (-shared)Create a shared lib; (-Bsymbolic)Bind global function references locally, (-l efi, -l gnuefi)Link with GNU EFI libraries
LDFLAGS_x86-64-uefiapp_CLANG 			= $(EFI_OBJS) -nostdlib -znocombreloc -shared -Bsymbolic -l efi -l gnuefi
#Flags - (-j)Only copy the specified section to output; (--target=efi-app-x86_64)Select the platform
OBJCPYFLAGS 							= -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64

# x86-32_clang #
CC_x86-32_CLANG 		= clang
AS_x86-32_CLANG 		= nasm
LD_x86-32_CLANG			= clang
CFLAGS_x86-32_CLANG 	= -I libs/headers -std=gnu99 -ffreestanding $(CFLAGS_GLOBAL) -target i386-elf
LDFLAGS_x86-32_CLANG 	= -T make/linker.ld -ffreestanding -nostdlib -l gcc -target i386-elf
ASFLAGS_x86-32_CLANG 	= -felf32

## EFI ##
ARCH 					= x86_64
EFI_INCLUDE_PATH 		= /usr/include/efi 																		# Contains all of the headers necessery to compiler EFI apps
EFI_LINK_OBJ 			= /usr/lib/crt0-efi-$(ARCH).o															# Shared library to link the UEFI header functions
EFI_LINK_SCRIPT 		= /usr/lib/elf_$(ARCH)_efi.lds															# GNU-EFI custom linker script for linking with it's shared libraries
EFI_INCS 				= -I /usr/include/efi -I /usr/include/efi/$(ARCH) -I /usr/include/efi/protocol			# Includes general UEFI headers, platform specific UEFI headers and UEFI protocol headers
EFI_OBJS				= -T $(EFI_LINK_SCRIPT) -L /usr/lib64/gnuefi -L /usr/lib -L /usr/lib64 $(EFI_LINK_OBJ)	# Adds objects and libs for linker to link the UEFI app with



### START ###
all: x86-64_gcc



### BUILD ###
## GCC ##
# Compile code for x86-64 #
x86-64_gcc:
	echo "No source to compile yet"

# Compile code for x86-32 #
x86-32_gcc:
	mkdir 			-p 									$(BUILDDIR)

	$(AS_x86-32) 	$(ASFLAGS_x86-32) 	init/boot.s 	-o 	$(BUILDDIR)/boot.o
	$(CC_x86-32) 	$(CFLAGS_x86-32) 	$(SOURCES)
	mv 									*.o 			$(BUILDDIR)
	$(CC_x86-32) 	$(LDFLAGS_x86-32) 	$(OBJECTS)		-o $(BUILDDIR)/os.o

	mkdir -p iso/boot/grub
	mv $(BUILDDIR)/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o $(BUILDDIR)/os.iso iso


## CLANG/LLVM ##
x86-64_clang:

x86-32_clang:
	mkdir 				-p 										$(BUILDDIR)

	$(AS_x86-32_CLANG) 	$(ASFLAGS_x86-32_CLANG) 	init/boot.s	-o $(BUILDDIR)/boot.o
	$(CC_x86-32_CLANG) 	$(CFLAGS_x86-32_CLANG)  	$(SOURCES)
	mv												*.o			$(BUILDDIR)
	$(CC_x86-32_CLANG)	$(LDFLAGS_x86-32_CLANG)		$(OBJECTS)	-o $(BUILDDIR)/os.o

	mkdir -p iso/boot/grub
	mv $(BUILDDIR)/os.o iso/boot/
	cp make/grub.cfg iso/boot/grub/
	grub2-mkrescue -o $(BUILDDIR)/os.iso iso

# Compile code for x86-64-uefi #
x86-64-uefi_clang:
	mkdir 						-p 																			$(BUILDDIR)

	$(CC_x86-64-uefiapp_CLANG)	$(CFLAGS_x86-64-uefiapp_CLANG)	$(x86-64-uefiapp_TARGET)/*.c
	mv															*.o											$(BUILDDIR)
	$(LD_x86-64-uefiapp_CLANG)	$(LDFLAGS_x86-64-uefiapp_CLANG)	$(BUILDDIR)/*.o 							-o $(BUILDDIR)/$(x86-64-uefiapp_TARGET).so
	$(OBJCPY) 					$(OBJCPYFLAGS)					$(BUILDDIR)/$(x86-64-uefiapp_TARGET).so 	$(BUILDDIR)/$(x86-64-uefiapp_TARGET).efi



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

# Start qemu on 64-bit UEFI-only mode CPU with UEFI app inserted #
qemu_x86-64-uefiapp:
	# Copy over the code and variable vars for OVMF
	cp $(OVMF)/OVMF_CODE.fd $(OVMF)/OVMF_VARS.fd $(BUILDDIR)

	# Run qemu - (-drive ...img) Boot our created disk image; (-net none) No network boot; (-cpu qemu64) Run with 64bit CPU; (-drive ...fd) Insert our compiled UEFI code and variable space
	qemu-system-x86_64 -drive format=raw,file=$(BUILDDIR)/$(x86-64-uefiapp_TARGET).img \
	-cpu qemu64 -net none \
	-drive if=pflash,format=raw,unit=0,file=$(BUILDDIR)/OVMF_CODE.fd -drive if=pflash,format=raw,unit=1,file=$(BUILDDIR)/OVMF_VARS.fd

# Start qemu on 32-bit BIOS-mode CPU #
qemu_x86-32-bios:
	qemu-system-i386 -cdrom $(BUILDDIR)/os.iso



### SPECIAL ###
# UEFI Disk Image Build
uefi-disk:
	mkdir -p $(BUILDDIR)

	# Create an empty disk image with size of 94 KB
	dd if=/dev/zero of=$(BUILDDIR)/$(x86-64-uefiapp_TARGET).img bs=512 count=93750

	# Format the disk image with 1440 byte sectors
	mformat -i $(BUILDDIR)/$(x86-64-uefiapp_TARGET).img -f 1440 ::

	# Create EFI and then EFI/BOOT directory on the disk image
	mmd -i $(BUILDDIR)/$(x86-64-uefiapp_TARGET).img ::/EFI
	mmd -i $(BUILDDIR)/$(x86-64-uefiapp_TARGET).img ::/EFI/BOOT

	# Copy the efi executable to the EFI/BOOT folder on the drive
	mcopy -i $(BUILDDIR)/$(x86-64-uefiapp_TARGET).img $(BUILDDIR)/$(x86-64-uefiapp_TARGET).efi ::/EFI/BOOT

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
