# @author = ngx123
# @brief = a make file with instructions to build tOS for x86-32


## Variables ##
# x86-32_gcc
CC_x86-32_GCC 					= $(TOOLCHAIN_PREFIX)/i686-elf-gcc
AS_x86-32_GCC 					= nasm
LD_x86-32_GCC					= $(TOOLCHAIN_PREFIX)/i686-elf-gcc
ASFLAGS_x86-32_GCC 				= -felf32														# (-felf32) Set the target architecture to x86_32-elf
CFLAGS_x86-32_GCC 				= -I include -std=gnu99 -ffreestanding $(CFLAGS_GLOBAL)			# (-I libs) Include the custo c library; (-std=gnu99) Becuase of code specifics should be older standard; (-ffreestanding) Startup not at main() and no standard lib
LDFLAGS_x86-32_GCC 				= -T $(INIT_SOURCES_x86-32)/link-x86_32.ld -ffreestanding -nostdlib -l gcc	# (-T script) Use custom linker script; (-ffreestanding)Startup not at main() and no standard lib; (-nostdlib) Do not link with standard library; (-l gcc) Link with gcc library
BUILDDIR_x86-32_GCC 			= $(BUILDDIR_x86-32)
SOURCES_x86-32_GCC				= drivers/*.c kernel/*.c $(INIT_SOURCES_x86-32)/*.c lib/*.c
OBJECTS_x86-32_GCC				= $(BUILDDIR_x86-32_GCC)/*.o

# x86-32_clang
CC_x86-32_CLANG 				= clang
AS_x86-32_CLANG 				= nasm
LD_x86-32_CLANG					= clang
ASFLAGS_x86-32_CLANG 			= -felf32
CFLAGS_x86-32_CLANG 			= -I include -std=gnu99 -ffreestanding $(CFLAGS_GLOBAL) -target i686-elf
LDFLAGS_x86-32_CLANG 			= -T $(INIT_SOURCES_x86-32)/link-x86_32.ld -ffreestanding -nostdlib -target i686-elf # -l gcc
BUILDDIR_x86-32_CLANG			= $(BUILDDIR_x86-32)
SOURCES_x86-32_CLANG			= drivers/*.c kernel/*.c $(INIT_SOURCES_x86-32)/*.c lib/*.c
OBJECTS_x86-32_CLANG			= $(BUILDDIR_x86-32_CLANG)/*.o


## Compilation instructions ##
# x86-32 gcc
build_x86-32_gcc:
	rm -rf $(BUILDDIR_x86-32_GCC)
	mkdir 				-p 																$(BUILDDIR_x86-32_GCC)

	$(AS_x86-32_GCC) 	$(ASFLAGS_x86-32_GCC) 	$(INIT_SOURCES_x86-32)/init-x86_32.s 	-o 	$(BUILDDIR_x86-32_GCC)/boot.o
	$(CC_x86-32_GCC) 	$(CFLAGS_x86-32_GCC) 	$(SOURCES_x86-32_GCC)
	mv 											*.o 									$(BUILDDIR_x86-32_GCC)
	$(CC_x86-32_GCC) 	$(LDFLAGS_x86-32_GCC) 	$(OBJECTS_x86-32_GCC)					-o $(BUILDDIR_x86-32_GCC)/$(TARGET).o

# x86-32 clang
build_x86-32_clang:
	rm -rf $(BUILDDIR_x86-32_CLANG)
	mkdir 				-p 																	$(BUILDDIR_x86-32_CLANG)

	$(AS_x86-32_CLANG) 	$(ASFLAGS_x86-32_CLANG) 	$(INIT_SOURCES_x86-32)/init-x86_32.s	-o $(BUILDDIR_x86-32_CLANG)/boot.o
	$(CC_x86-32_CLANG) 	$(CFLAGS_x86-32_CLANG)  	$(SOURCES_x86-32_CLANG)
	mv												*.o										$(BUILDDIR_x86-32_CLANG)
	$(LD_x86-32_CLANG)	$(LDFLAGS_x86-32_CLANG)		$(OBJECTS_x86-32_CLANG)					-o $(BUILDDIR_x86-32_CLANG)/$(TARGET).o


## BIOS readable disk build instructions ##
disk_x86-32-bios:
	mkdir -p $(BUILDDIR_x86-32)/iso/boot/grub
	mv $(BUILDDIR_x86-32)/$(TARGET).o $(BUILDDIR_x86-32)/iso/boot
	cp $(BIOS_SRC_DIR)/grub.cfg $(BUILDDIR_x86-32)/iso/boot/grub
	grub2-mkrescue -o $(BUILDDIR_x86-32)/$(TARGET).iso $(BUILDDIR_x86-32)/iso || grub-mkrescue -o $(BUILDDIR_x86-32)/$(TARGET).iso $(BUILDDIR_x86-32)/iso


## Run with qemu-system-i386 with BIOS ##
qemu_x86-32-bios:
	qemu-system-i386 -cdrom $(BUILDDIR_x86-32)/$(TARGET).iso