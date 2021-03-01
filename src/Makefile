## Variables ##
BUILDDIR 						= build
TOOLCHAIN_PREFIX				= ../toolchain/bin
TARGET							= tos

CFLAGS_GLOBAL 					= -Wmissing-prototypes -Wshadow -Wall -Wextra -c
CEXTRAWARNINGS 					= -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused


## Start ##
all: all_x86_64_clang


## Includes ##
include arch/x86_64/Makefile
include arch/x86_32/Makefile


## Special ##
# Toolchain Build
toolchain:
	read -p "Are you sure you want to compile the toolchain(Press any button to continue): " toolchain_compile
	chmod +x make/toolchain.sh
	make/toolchain.sh

# Build clean up
clean:
	rm -rf *.o *.so *.out *.img *.fd *.efi *iso $(BUILDDIR)