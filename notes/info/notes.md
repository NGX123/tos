## Fixes
* When linking problems occured when in linker flags -L <lib>(include library) was used after -l <lib>(use library)
	* Swaping -L and -l places fixed the linker errors
* When compiling x86-32_clang with target set to i386-elf the os was incorrectly linked
	* Changed of target to i686-elf fixed incorrect linkage