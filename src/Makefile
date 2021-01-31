## Variables ##
BUILDDIR 						= build
TOOLCHAIN_PREFIX				= ../toolchain/bin

MULTIBOOT2_DIR					= boot/multiboot2
BIOS_SRC_DIR					= boot/bios
UEFI_SRC_DIR					= boot/uefi
INIT_SOURCES_x86-64				= arch/x86-64
INIT_SOURCES_x86-32 			= arch/x86-32
BUILDDIR_x86-64					= $(BUILDDIR)/x86-64
BUILDDIR_x86-32					= $(BUILDDIR)/x86-32
TARGET							= tos

CFLAGS_GLOBAL 					= -Wshadow -Wall -Wextra -c
CEXTRAWARNINGS 					= -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused


## Start ##
all: 						all_x86-64_clang
all_x86-32_gcc: 			clean build_x86-32_gcc disk_x86-32-bios qemu_x86-32-bios
all_x86-64_clang:     		clean build_x86-64-uefi_clang disk_x86-64-uefi qemu_x86-64-uefi
all_x86-32_clang:			clean build_x86-32_clang disk_x86-32-bios qemu_x86-32-bios


## Includes ##
include arch/x86-64/Makefile
include arch/x86-32/Makefile


## Special ##
# Toolchain Build
toolchain:
	read -p "Are you sure you want to compile the toolchain(Press any button to continue): " toolchain_compile
	chmod +x make/toolchain.sh
	make/toolchain.sh

# Build clean up
clean:
	rm -rf *.o *.so *.out *.img *.fd *.efi *iso $(BUILDDIR)