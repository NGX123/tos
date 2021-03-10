## Hardware
- Accessing peripherals, hardware... on ARM and RISC-V(platforms with no PC-like standartisation(in terms of video output, keyboard input...))
	+ No generic interface exists so specific board(e.g. RPI) should be selected and documentation for it should be used
		* To select a specific board(computer) - `qemu-system-* -machine boardname`
		* To get all supported boards - `qemu-system-* -machine help`
	+ Links
		* [QEMU ARM Docs](https://wiki.qemu.org/Documentation/Platforms/ARM)
		* [Forum Post](https://stackoverflow.com/questions/20811203/how-can-i-output-to-vga-through-qemu-arm)

- Endianness
	- Little endian(e.g. x86) - here the last bit is located in the start and the first(0) is located at the end
		* e.g. Value 0xaaff will be stored in memory as 0xffaa
		* e.g. In a struct with bit fields a(0:7), b(8:15), c(16:31) the fields should be located as follows: a, b, c - this is the same as they should be becuase the endianness only applies to individual variables/values and structure is a collection of them so it does not apply to it as a whole
		* When you initialize a variable then - var = `Bit 7(last bit) is here ->`0xFF`<- Bit 0(first bit) is here`



## Code
- Combining 32-Bit and 64-Bit code - code compiled by 32-Bit compiler or assembler can not be linked with code compiled by 64-Bit compiler or assembler. The only way to unite such code(of different bitness) is it to write all the bit switch code(from 16 to 32, and from 32 to 64 bits) in assembly where parts of code could be seperated into 32-Bit and 64-Bit with directives like [BITS 16/32/64] and then make the output format of the assembler be the maximum used bitness(e.g. 64, if there is 32 and 64 bit code) and then it could be linked with 64-Bit C code which would be called by the 64-Bit assembly and switch could be forgot about after that(after switch to 64-Bit C code)

- Align in assembly/linker - aligns the next instruction/piece of data to start at address that is divisible by number align is supplied with. Normally next instruction/data will start right after the finish of previous, but in this case if the address after finish of previos data/instruction is not divisible by supplied number, the instruction after align will be moved to start at the closest address that is divisible by supplied number

- Storage for global varibles(that are initialized at run time and not at write time) could be allocated in the .bss section when programming in assembly(with directives like `resb`) and labels could just be placed near it for easy access



## Build tools
- Older c compilers added `_` before all symbols(e.g. "symbol" was made into "_symbol"), newer c compilers do not do this so when interfacing with assembly same function/variable names should be used("symbol" not "_symbol")
	* To check if compiler adds _ before symbols use - "objdump -t filename.ext"

- Clang
	+ [Chaning target](https://clang.llvm.org/docs/CrossCompilation.html)
		* To list the architecutres supported by clang use `llc --version`
		* To list available CPUs/features on an architecture use `llc -march=ARCH -mattr=help`
		* **Change target when compiling with clang:**
			* To change target arch - `-target <arch><sub>-<vendor>-<sys>-<abi>`(some fields can be missed like vendor and sys e.g. x86_64-elf)
			* To change CPU model in arch - `-mcpu <cpu>`
		* **Change target when linking with clang:**
			* By default clang uses systems default linker, which is usually GNU ld and it can't change the target so to make chaning target work there are two methods:
				* Change default linker to lld(the one that can change target) in clang - `-fuse-ld=lld`
				* Use lld directly and not through clang(ld.lld on unix) - `ld.lld <file> <file>`
			* To change the target
				* When using `clang -fuse-ld=lld`, no additional steps are needed as ld.lld will link files in right format automatically(if both files are in the same format)
				* When using `ld.lld <file> <file>`, no additional steps for same reason as above

- In NASM in the .bss section `align` has to be changed to `alignb`(same as align in everything, but has a different meaning to NASM) because it is a special instruction for .bss section, all other sections should continue using `align`. If the `align` is used in .bss or `alignb` is used in other sections then there will be warnings and errors



## Information
- In grub2 there are multiple modules that can be found by searching different folders in `grub-source/grub-core/*`, to understand that something is a module search the c file for `GRUB MOD LICENSE *` or similar, then load the module by the name of c file. e.g. to load `gop_efi.c` add `insmod gop_efi` before the kernel executable entry in grub config

- The multiboot header is very recommended to be located in it's own section(can be called anything e.g. .multi_sec, .multiboot) because then it could be linked so it would be 100% located at the start of executable
	1. .multiboot section should be located in the part with .text permissions in the linkerscript and it should be absolutely the first section in the linker script(the .text permission part will be first in linkerscript and in .text part the .multiboot will be the first one linked)
	2. In the assembly code it should be put into it's own section and not .text
	* This should be done because there will be many .text sections and it will cause that any of the could be located first in the executable, on the other side there will most probably only one .multiboot section so it will definitily be linked first

- LaTeX and TeX - TeX is a document format which uses macros to convert it the text info PDF, HTML and more and can be used to add mathemtical formulas and technical details. LaTeX is a set of macros on top of TeX. These are just formats and there exists multiple compilers that could compile TeX and LaTex into nearly any format(PDF, HTML, DVI...)
	* Texlive - command line latex compiler
	* TexStudio - texinfo editor
		* Includes utilities to convert to pdf - `sudo apt/dnf install texlive`, `pdflatex file.tex



## Fixes
- When pasting another project repositoy as a folder(like a bootloader) into you own git repo - git will detect it as a submodule and to stop it:
	* Remove: all .git* files and folders, LICENSE.md, all submodules folders in the project you are pasting(if there are any in the project that is pasted in your git repo)

- When compiling x86-32_clang with target set to `i386-elf` the os will link incorrectly or not compile at all(without error). When compiling x86-64-clang with target set to `x86_64-elf` same problem as in previous sentance would occur.
	* In general, even with other architectures try to look at llc CPU list on selected arch - `llc -march=ARCH -mattr=help`(e.g. ARCH=x86) or recheck how architecture is called in `llc --version`
	1. Change target from `i386-elf` to `i686-elf`, linkage and compilation would be fixed
	2. Change target compiler target to `--target=x86_64-elf` and linker target to `-march=x86_64-elf`

- If multiboot2 says "no video found" or similar on UEFI systems:
	* in GRUB config before the `multiboot /path/to/bin` insert `insmod efi_gop`

- When making a Multiboot2 2 kernel, changes are needed in conifg files to make it work
	* `multiboot executable` in menuentry {} in grub.cfg should be changed to `multiboot2 executable`
	+ If grub uefi is used the start of file system is not /, so disk name should be added before the path - instead of `multiboot2 /path/to/exe`, use `multiboot2 (hd0)/path/to/exe`
		* `(hd0)` can be changed to any other disk, to check the available disks use the GRUB console `ls` command

- [The compiler does not follow packed attribute if there is 64bit field in the structure](https://forum.osdev.org/viewtopic.php?t=30318)