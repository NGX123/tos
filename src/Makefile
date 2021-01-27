### VARIABLES ###
## Global ##
BUILDDIR 						= ./build
TOOLCHAIN_PREFIX				= ../toolchain/bin
OVMF 							= /usr/share/OVMF
EDK2							= ../toolchain/edk2

CFLAGS_GLOBAL 					= -Wshadow -Wall -Wextra -c
CEXTRAWARNINGS 					= -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused


## Compilation ##
BIOS_SRC_DIR					= ./boot/bios
UEFI_SRC_DIR					= ./boot/uefi
INIT_SOURCES_x86-64				= ./arch/x86-64
INIT_SOURCES_x86-32 			= ./arch/x86-32
BUILDDIR_x86-64					= $(BUILDDIR)/x86-64
BUILDDIR_x86-32					= $(BUILDDIR)/x86-32
TARGET							= tos

# x86-64-uefi_gcc toolchain #
CC_x86-64_GCC 					= $(TOOLCHAIN_PREFIX)/x86_64-elf-gcc
AS_x86-64_GCC 					= nasm
LD_x86-64_GCC					= $(TOOLCHAIN_PREFIX)/x86_64-elf-gcc
ASFLAGS_x86-64_GCC				= -felf64
CFLAGS_x86-64_GCC 				= -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2 $(CFLAGS_GLOBAL)	# (-ffreestanding) Startup not at main() and no standard lib; (-mcmodel=large) No assumptions about addresses; (-mno-red-zone -mno-mmx -mno-sse -mno-sse2) Disable CPU specific features that could generate runtime errors
LDFLAGS_x86-64_GCC 				= -T uefi/linker.ld -ffreestanding -nostdlib -l gcc											# (-T script.ld) Use custom linker script; (-ffreestanding)Startup not at main() and no standard lib; (-nostdlib) Do not link with standard library; (-l gcc) Link with gcc library
BUILDDIR_x86-64_GCC 			= $(BUILDDIR_x86-64)
SOURCES_x86-64-uefi_CLANG		= $(INIT_SOURCES_x86-64)/*.c
OBJECTS_x86-64-uefi_CLANG		= $(BUILDDIR_x86-64_GCC)/*.o

# x86-32_gcc toolchain #
CC_x86-32_GCC 					= $(TOOLCHAIN_PREFIX)/i686-elf-gcc
AS_x86-32_GCC 					= nasm
LD_x86-32_GCC					= $(TOOLCHAIN_PREFIX)/i686-elf-gcc
ASFLAGS_x86-32_GCC 				= -felf32														# (-felf32) Set the target architecture to x86_32-elf
CFLAGS_x86-32_GCC 				= -I include -std=gnu99 -ffreestanding $(CFLAGS_GLOBAL)			# (-I libs) Include the custo c library; (-std=gnu99) Becuase of code specifics should be older standard; (-ffreestanding) Startup not at main() and no standard lib
LDFLAGS_x86-32_GCC 				= -T $(BIOS_SRC_DIR)/linker.ld -ffreestanding -nostdlib -l gcc	# (-T script.ld) Use custom linker script; (-ffreestanding)Startup not at main() and no standard lib; (-nostdlib) Do not link with standard library; (-l gcc) Link with gcc library
BUILDDIR_x86-32_GCC 			= $(BUILDDIR_x86-32)
SOURCES_x86-32_GCC				= drivers/*.c kernel/*.c $(INIT_SOURCES_x86-32)/*.c lib/*.c
OBJECTS_x86-32_GCC				= $(BUILDDIR_x86-32_GCC)/*.o

# x86-64-uefi_clang toolchain #
CC_x86-64-uefi_CLANG 			= clang
AS_x86-64-uefi_CLANG			= nasm
LD_x86-64-uefi_CLANG			= clang
ASFLAGS_x86-64-uefi_CLANG		= -felf64
CFLAGS_x86-64-uefi_CLANG 		= -T $(INIT_SOURCES_x86-64)/link.ld -I include -ffreestanding $(CFLAGS_GLOBAL) -target x86_64-elf -mcmodel=large -mno-red-zone -mno-mmx -mno-sse -mno-sse2
LDFLAGS_x86-64-uefi_CLANG		= -T $(INIT_SOURCES_x86-64)/$(TARGET).ld -ffreestanding -nostdlib -target x86_64-elf
BUILDDIR_x86-64-uefi_CLANG		= $(BUILDDIR_x86-64)
SOURCES_x86-64-uefi_CLANG		= $(INIT_SOURCES_x86-64)/*.c
OBJECTS_x86-64-uefi_CLANG		= $(BUILDDIR_x86-64-uefi_CLANG)/*.o

# x86-32_clang #
CC_x86-32_CLANG 				= clang
AS_x86-32_CLANG 				= nasm
LD_x86-32_CLANG					= clang
ASFLAGS_x86-32_CLANG 			= -felf32
CFLAGS_x86-32_CLANG 			= -I include -std=gnu99 -ffreestanding $(CFLAGS_GLOBAL) -target i686-elf
LDFLAGS_x86-32_CLANG 			= -T $(BIOS_SRC_DIR)/linker.ld -ffreestanding -nostdlib -target i686-elf # -l gcc
BUILDDIR_x86-32_CLANG			= $(BUILDDIR_x86-32)
SOURCES_x86-32_CLANG			= drivers/*.c kernel/*.c $(INIT_SOURCES_x86-32)/*.c lib/*.c
OBJECTS_x86-32_CLANG			= $(BUILDDIR_x86-32_CLANG)/*.o

# x86-64-uefiapp_clang #
CC_x86-64-uefiapp_CLANG 		= clang # Can be changed to gcc, then change -DHAVE_USE_MS_ABI to -DEFI_FUNCTION_WRAPPER
LD_x86-64-uefiapp_CLANG 		= ld
OBJCPY 							= objcopy
CFLAGS_x86-64-uefiapp_CLANG		= $(EFI_INCS) -fno-stack-protector -fpic -fshort-wchar -mno-red-zone -DHAVE_USE_MS_ABI -D GNUEFI_BUILD $(CFLAGS_GLOBAL) # (-fno-stack-protector)Disable monitoring variable at stack end to indicate overflow if overwritten; (-fpic)Position independent code(can be loaded anywhere in memory); (-fshort-wchar)Makes wchar_t short(2 byte), (-mno-red-zone)Disables feature according to ABI; (-DHAVE_USE_MS_ABI)Use windows function calling convention(it is used by UEFI); (-D GNUEFI_BUILD) Define a macro to switch my code from EDK2 to gnuefi(macro checking made by me)
LDFLAGS_x86-64-uefiapp_CLANG 	= $(EFI_OBJS) -nostdlib -znocombreloc -shared -Bsymbolic -l efi -l gnuefi												# (-nostdlib)Do not link against any libs except specified; (-znocombreloc)Don't combine relocation sections; (-shared)Create a shared lib, for later conversion to binary; (-Bsymbolic)Bind global function references locally; (-l efi, -l gnuefi)Link with GNU EFI libraries
OBJCPYFLAGS 					= -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-x86_64 				# (-j)Only copy the specified section to output; (--target=efi-app-x86_64)Select the platform
BUILDDIR_x86-64-uefiapp_CLANG	= $(BUILDDIR_x86-64)
SOURCES_x86-64-uefiapp_CLANG 	= $(UEFI_SRC_DIR)/*.c

# x86-64-uefiapp_edk2 #
BUILDDIR_x86-64-uefiapp_EDK2 	= $(EDK2)/UefiAppBootloaderPkg
SOURCES_x86-64-uefiapp_EDK2 	= $(UEFI_SRC_DIR)/*.c


## EFI ##
ARCH 							= x86_64
EFI_INCLUDE_PATH 				= /usr/include/efi 																		# Contains all of the headers necessery to compiler EFI apps
EFI_LINK_OBJ 					= /usr/lib/crt0-efi-$(ARCH).o															# Shared library to link the UEFI header functions
EFI_LINK_SCRIPT 				= /usr/lib/elf_$(ARCH)_efi.lds															# GNU-EFI custom linker script for linking with it's shared libraries
EFI_INCS 						= -I /usr/include/efi -I /usr/include/efi/$(ARCH) -I /usr/include/efi/protocol			# Includes general UEFI headers, platform specific UEFI headers and UEFI protocol headers
EFI_OBJS						= -T $(EFI_LINK_SCRIPT) -L /usr/lib64/gnuefi -L /usr/lib -L /usr/lib64 $(EFI_LINK_OBJ)	# Adds objects and libs for linker to link the UEFI app with



### START ###
all: 						all_x86-32_clang
all_x86-32_gcc: 			clean build_x86-32_gcc disk_x86-32-bios qemu_x86-32-bios
all_x86-32_clang:			clean build_x86-32_clang disk_x86-32-bios qemu_x86-32-bios
all_x86-64-uefiapp_clang:	clean build_x86-64-uefiapp_clang disk_x86-64-uefi_bootloader qemu_x86-64-uefiapp
all_x86-64-uefiapp_edk2:	clean build_x86-64-uefiapp_edk2 disk_x86-64-uefi_bootloader qemu_x86-64-uefiapp



### BUILD ###
## GCC ##
# Compile code for x86-32 with gcc #
build_x86-32_gcc:
	rm -rf $(BUILDDIR_x86-32_GCC)
	mkdir 				-p 														$(BUILDDIR_x86-32_GCC)

	$(AS_x86-32_GCC) 	$(ASFLAGS_x86-32_GCC) 	$(INIT_SOURCES_x86-32)/init.s 	-o 	$(BUILDDIR_x86-32_GCC)/boot.o
	$(CC_x86-32_GCC) 	$(CFLAGS_x86-32_GCC) 	$(SOURCES_x86-32_GCC)
	mv 											*.o 							$(BUILDDIR_x86-32_GCC)
	$(CC_x86-32_GCC) 	$(LDFLAGS_x86-32_GCC) 	$(OBJECTS_x86-32_GCC)			-o $(BUILDDIR_x86-32_GCC)/$(TARGET).o


## CLANG/LLVM ##
# Compile code for x86-64 with clang #
build_x86-64-uefi_clang: clean
	rm -rf $(BUILDDIR_x86-64)
	mkdir -p $(BUILDDIR_x86-64-uefi_CLANG)

	$(AS_x86-64-uefi_CLANG) $(ASFLAGS_x86-64-uefi_CLANG) $(INIT_SOURCES_x86-64)/init.s -o $(BUILDDIR_x86-64-uefi_CLANG)/boot.o
	$(LD_x86-64-uefi_CLANG)	$(LDFLAGS_x86-64-uefi_CLANG) $(OBJECTS_x86-64-uefi_CLANG) -o $(BUILDDIR_x86-64-uefi_CLANG)/$(TARGET)

# Compile code for x86-32 with clang #
build_x86-32_clang:
	rm -rf $(BUILDDIR_x86-32_CLANG)
	mkdir 				-p 															$(BUILDDIR_x86-32_CLANG)

	$(AS_x86-32_CLANG) 	$(ASFLAGS_x86-32_CLANG) 	$(INIT_SOURCES_x86-32)/init.s	-o $(BUILDDIR_x86-32_CLANG)/boot.o
	$(CC_x86-32_CLANG) 	$(CFLAGS_x86-32_CLANG)  	$(SOURCES_x86-32_CLANG)
	mv												*.o								$(BUILDDIR_x86-32_CLANG)
	$(LD_x86-32_CLANG)	$(LDFLAGS_x86-32_CLANG)		$(OBJECTS_x86-32_CLANG)			-o $(BUILDDIR_x86-32_CLANG)/$(TARGET).o

# Compile code for x86-64-uefi with clang #
build_x86-64-uefiapp_clang:
	rm -rf $(BUILDDIR_x86-64-uefiapp_CLANG)
	mkdir 						-p 																								$(BUILDDIR_x86-64-uefiapp_CLANG)

	$(CC_x86-64-uefiapp_CLANG)	$(CFLAGS_x86-64-uefiapp_CLANG)	$(SOURCES_x86-64-uefiapp_CLANG)
	mv															*.o																$(BUILDDIR_x86-64-uefiapp_CLANG)
	$(LD_x86-64-uefiapp_CLANG)	$(LDFLAGS_x86-64-uefiapp_CLANG)	$(BUILDDIR_x86-64-uefiapp_CLANG)/*.o 							-o $(BUILDDIR_x86-64-uefiapp_CLANG)/$(TARGET).so
	$(OBJCPY) 					$(OBJCPYFLAGS)					$(BUILDDIR_x86-64-uefiapp_CLANG)/$(TARGET).so 	$(BUILDDIR_x86-64-uefiapp_CLANG)/$(TARGET).efi

# Compile code for x86-64-uefi with edk2 toolchain #
build_x86-64-uefiapp_edk2: clean
	rm -rf $(BUILDDIR_x86-64)
	rm -rf $(BUILDDIR_x86-64-uefiapp_EDK2)
	mkdir -p $(BUILDDIR_x86-64)

	cp -r $(UEFI_SRC_DIR) $(BUILDDIR_x86-64-uefiapp_EDK2)
	cd $(EDK2) && \
	. ./edksetup.sh &&		\
	build -p UefiAppBootloaderPkg/$(TARGET).dsc -t GCC5 -a X64

	cp $(BUILDDIR_x86-64-uefiapp_EDK2)/Build/DEBUG_GCC5/X64/UefiAppBootloaderPkg.efi $(BUILDDIR_x86-64)/$(TARGET).efi



### DISK IMAGES ###
# x86-64 UEFI Disk Image Build #
disk_x86-64-uefi_bootloader:
	mkdir -p $(BUILDDIR_x86-64)

	# Create an empty disk image with size of 94 KB
	dd if=/dev/zero of=$(BUILDDIR_x86-64)/$(TARGET).img bs=512 count=93750

	# Format the disk image with 1440 byte sectors
	mformat -i $(BUILDDIR_x86-64)/$(TARGET).img -f 1440 ::

	# Create EFI and then EFI/BOOT directory on the disk image
	mmd -i $(BUILDDIR_x86-64)/$(TARGET).img ::/EFI
	mmd -i $(BUILDDIR_x86-64)/$(TARGET).img ::/EFI/BOOT

	# Copy the efi executable to the EFI/BOOT folder on the drive
	mv $(BUILDDIR_x86-64)/$(TARGET).efi $(BUILDDIR_x86-64)/BOOTX64.EFI
	mcopy -i $(BUILDDIR_x86-64)/$(TARGET).img $(BUILDDIR_x86-64)/BOOTX64.EFI ::/EFI/BOOT

# x86-32 BIOS Disk Image Build #
disk_x86-32-bios:
	mkdir -p $(BUILDDIR_x86-32)/iso/boot/grub
	mv $(BUILDDIR_x86-32)/$(TARGET).o $(BUILDDIR_x86-32)/iso/boot
	cp $(BIOS_SRC_DIR)/grub.cfg $(BUILDDIR_x86-32)/iso/boot/grub
	grub2-mkrescue -o $(BUILDDIR_x86-32)/$(TARGET).iso $(BUILDDIR_x86-32)/iso || grub-mkrescue -o $(BUILDDIR_x86-32)/$(TARGET).iso $(BUILDDIR_x86-32)/iso



### QEMU ###
# Start QEMU in 64-bit UEFI mode with disc(ISO) inserted #
qemu_x86-64-uefi:
	cp /usr/share/OVMF/OVMF_CODE.fd /usr/share/OVMF/OVMF_VARS.fd $(BUILDDIR_x86-64)
	qemu-system-x86_64 -cpu qemu64 \
	-drive if=pflash,format=raw,unit=0,file=$(BUILDDIR_x86-64)/OVMF_CODE.fd,readonly=on \
	-drive if=pflash,format=raw,unit=1,file=$(BUILDDIR_x86-64)/OVMF_VARS.fd \
	-net none \
	-cdrom $(BUILDDIR_x86-64)/$(TARGET).iso

# Start qemu on 32-bit BIOS-mode CPU #
qemu_x86-32-bios:
	qemu-system-i386 -cdrom $(BUILDDIR_x86-32)/$(TARGET).iso

# Start qemu on 64-bit UEFI-only mode CPU with UEFI app inserted #
qemu_x86-64-uefiapp:
	# Copy over the code and variable vars for OVMF
	cp $(OVMF)/OVMF_CODE.fd $(OVMF)/OVMF_VARS.fd $(BUILDDIR_x86-64)

	# Run qemu - (-drive ...img) Boot our created disk image; (-net none) No network boot; (-cpu qemu64) Run with 64bit CPU; (-drive ...fd) Insert our compiled UEFI code and variable space
	qemu-system-x86_64 -drive format=raw,file=$(BUILDDIR_x86-64)/$(TARGET).img \
	-cpu qemu64 -net none \
	-drive if=pflash,format=raw,unit=0,file=$(BUILDDIR_x86-64)/OVMF_CODE.fd -drive if=pflash,format=raw,unit=1,file=$(BUILDDIR_x86-64)/OVMF_VARS.fd



### SPECIAL ###
# Toolchain Build #
toolchain:
	read -p "Are you sure you want to compile the toolchain(Press any button to continue): " toolchain_compile
	chmod +x make/toolchain.sh
	make/toolchain.sh

# Build clean up #
clean:
	rm -rf *.o *.so *.out *.img *.fd *.efi *iso $(BUILDDIR)