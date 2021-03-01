# @author = ngx123
# @brief = a make file with instructions to build tOS for x86_32


## Variables ##
BUILDDIR_x86_32			= $(BUILDDIR)/x86_32
INIT_SOURCES_x86_32 	= arch/x86_32
BUILDDIR_x86_32_CLANG	= $(BUILDDIR_x86_32)
SOURCES_x86_32			= drivers/*.c kernel/*.c $(INIT_SOURCES_x86_32)/*.c lib/*.c lib/arch/*.c
OBJECTS_x86_32			= $(BUILDDIR_x86_32)/*.o

BOOTSTRAP_DIR_MULTIBOOT2	= boot/multiboot2
OS_MACROS 					= -D PRINTF_NO_LONG_LONG

# x86_32_gcc
CC_x86_32_GCC 					= $(TOOLCHAIN_PREFIX)/i686-elf-gcc
AS_x86_32_GCC 					= nasm
LD_x86_32_GCC					= $(TOOLCHAIN_PREFIX)/i686-elf-gcc
ASFLAGS_x86_32_GCC 				= -felf32																	# (-felf32) Set the target architecture to x86_32-elf
CFLAGS_x86_32_GCC 				= -I include -std=gnu99 -ffreestanding $(CFLAGS_GLOBAL)	$(OS_MACROS)		# (-I libs) Include the custo c library; (-std=gnu99) Becuase of code specifics should be older standard; (-ffreestanding) Startup not at main() and no standard lib
LDFLAGS_x86_32_GCC 				= -T $(INIT_SOURCES_x86_32)/link-x86_32.ld -ffreestanding -nostdlib -l gcc	# (-T script) Use custom linker script; (-ffreestanding)Startup not at main() and no standard lib; (-nostdlib) Do not link with standard library; (-l gcc) Link with gcc library

# x86_32_clang
CC_x86_32_CLANG 				= clang
AS_x86_32_CLANG 				= nasm
LD_x86_32_CLANG					= clang
ASFLAGS_x86_32_CLANG 			= -felf32
CFLAGS_x86_32_CLANG 			= -I include -std=gnu99 -ffreestanding $(CFLAGS_GLOBAL) -target i686-elf $(OS_MACROS)
LDFLAGS_x86_32_CLANG 			= -T $(INIT_SOURCES_x86_32)/link-x86_32.ld -ffreestanding -nostdlib -target i686-elf # -l gcc


## Compilation instructions ##
# x86_32 gcc
build_x86_32_gcc:
	rm -rf $(BUILDDIR_x86_32)
	mkdir 				-p 																$(BUILDDIR_x86_32)

	$(AS_x86_32_GCC) 	$(ASFLAGS_x86_32_GCC) 	$(INIT_SOURCES_x86_32)/init-x86_32.s 	-o 	$(BUILDDIR_x86_32)/boot.o
	$(CC_x86_32_GCC) 	$(CFLAGS_x86_32_GCC) 	$(SOURCES_x86_32)
	mv 											*.o 									$(BUILDDIR_x86_32)
	$(CC_x86_32_GCC) 	$(LDFLAGS_x86_32_GCC) 	$(OBJECTS_x86_32)						-o $(BUILDDIR_x86_32)/$(TARGET).o

# x86_32 clang
build_x86_32_clang:
	rm -rf $(BUILDDIR_x86_32)
	mkdir 				-p 																	$(BUILDDIR_x86_32)

	$(AS_x86_32_CLANG) 	$(ASFLAGS_x86_32_CLANG) 	$(INIT_SOURCES_x86_32)/init-x86_32.s	-o $(BUILDDIR_x86_32)/boot.o
	$(CC_x86_32_CLANG) 	$(CFLAGS_x86_32_CLANG)  	$(SOURCES_x86_32)
	mv												*.o										$(BUILDDIR_x86_32)
	$(LD_x86_32_CLANG)	$(LDFLAGS_x86_32_CLANG)		$(OBJECTS_x86_32)						-o $(BUILDDIR_x86_32)/$(TARGET).o


## BIOS readable disk build instructions ##
disk_x86_32-bios:
	mkdir -p $(BUILDDIR_x86_32)/iso/boot/grub
	mv $(BUILDDIR_x86_32)/$(TARGET).o $(BUILDDIR_x86_32)/iso/boot
	cp $(BOOTSTRAP_DIR_MULTIBOOT2)/grub.cfg $(BUILDDIR_x86_32)/iso/boot/grub
	grub2-mkrescue -o $(BUILDDIR_x86_32)/$(TARGET).iso $(BUILDDIR_x86_32)/iso || grub-mkrescue -o $(BUILDDIR_x86_32)/$(TARGET).iso $(BUILDDIR_x86_32)/iso


## Run with qemu-system-i386 with BIOS ##
qemu_x86_32-bios:
	qemu-system-i386 -cdrom $(BUILDDIR_x86_32)/$(TARGET).iso


## Main Compilation Routines ##
all_x86_32_gcc: 			clean build_x86_32_gcc disk_x86_32-bios qemu_x86_32-bios
all_x86_32_clang:			clean build_x86_32_clang disk_x86_32-bios qemu_x86_32-bios