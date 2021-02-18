### Hardware
* Accessing peripherals, hardware... on ARM and RISC-V(platforms with no PC-like standartisation(in terms of video output, keyboard input...))
	* No generic interface exists so specific board(e.g. RPI) should be selected and documentation for it should be used
		* To select a specific board(computer) - `qemu-system-* -machine boardname`
		* To get all supported boards - `qemu-system-* -machine help`
	* Links
		* [QEMU ARM Docs](https://wiki.qemu.org/Documentation/Platforms/ARM)
		* [Forum Post](https://stackoverflow.com/questions/20811203/how-can-i-output-to-vga-through-qemu-arm)

### Code
* Align in assembly/linker - aligns the next instruction/piece of data to start at address that is divisible by number align is supplied with. Normally next instruction/data will start right after the finish of previous, but in this case if the address after finish of previos data/instruction is not divisible by supplied number, the instruction after align will be moved to start at the closest address that is divisible by supplied number
* When splitting fields in structures or variables - like splitting a 64 bit field in structure into two 32 bit fields, on x86 as it is little endian first part will be in second var
	- e.g. If `struct test {uint64_t i;}` is split into `struct test2 {uint32_t i1; uint32_t i2;}`, and if `test.i = 0x12A05F200` then `test2.i1 = 0x2A05F200` and `test2.i2 = 0x1`
	- To combine the two split variables back into on x86 - e.g. if `uint64_t original` is split into `uint32_t part1`, `uint32_t part2` then to combine use `part1 | (uint64_t)part2 << 32`

### Build tools
* Older c compilers added `_` before all symbols(e.g. "symbol" was made into "_symbol"), newer c compilers do not do this so when interfacing with assembly same function/variable names should be used("symbol" not "_symbol")
	* To check if compiler adds _ before symbols use - "objdump -t filename.ext"
* Clang
	- To list the architecutres supported by clang use `llc --version`
	- To list available CPUs/features on an architecture use `llc -march=ARCH -mattr=help`

## Information
* In grub2 there are multiple modules that can be found by searching different folders in `grub-source/grub-core/*`, to understand that something is a module search the c file for `GRUB MOD LICENSE *` or similar, then load the module by the name of c file. e.g. to load `gop_efi.c` add `insmod gop_efi` before the kernel executable entry in grub config
* The multiboot header is very recommended to be located in it's own section(can be called anything e.g. .multi_sec, .multiboot) because then it could be linked so it would be 100% located at the start of executable
	1. .multiboot section should be located in the part with .text permissions in the linkerscript and it should be absolutely the first section in the linker script(the .text permission part will be first in linkerscript and in .text part the .multiboot will be the first one linked)
	2. In the assembly code it should be put into it's own section and not .text
	* This should be done because there will be many .text sections and it will cause that any of the could be located first in the executable, on the other side there will most probably only one .multiboot section so it will definitily be linked first

## Fixes
* When compiling x86-32_clang with target set to `i386-elf` the os will link incorrectly
	* Target change to `i686-elf` will fix linkage
* If multiboot2 says "no video found" or similar
	* in grub config before the `multiboot /path/to/bin` insert `insmod efi_gop`
* When making a Multiboot2 2 kernel, changes are needed in conifg files to make it work
	* `multiboot executable` in menuentry {} in grub.cfg should be changed to `multiboot2 executable`
	* If grub uefi is used the start of file system is not /, so disk name should be added before the path - instead of `multiboot2 /path/to/exe`, use `multiboot2 (hd0)/path/to/exe`
		* `(hd0)` can be changed to any other disk, to check the available disks use the GRUB console `ls` command
* [The compiler does not follow packed attribute if there is 64bit field in the structure](https://forum.osdev.org/viewtopic.php?t=30318)