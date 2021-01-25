## Fixes
### Hardware
* The memory map has reserved areas below 1mb and right at the end of it, all other space is free. The map will change depending on amount of RAM(in 200mb, only about 150 will be free)

### Code
* When using functions from `<stdarg.h>` like `va_start(), va_end(), va_arg()`, the function where they were used should not call other functions(e.g. if `func1()` uses va_start(), `func2()` or any other function can't be called from `func1()`)
* When splitting fields in structures or variables - like splitting a 64 bit field in structure into two 32 bit fields, on x86 as it is little endian first part will be in second var
	- e.g. If `struct test {uint64_t i;}` is split into `struct test2 {uint32_t i1; uint32_t i2;}`, and if `test.i = 0x12A05F200` then `test2.i1 = 0x2A05F200` and `test2.i2 = 0x1`
	- To combine the two split variables back into on x86 - e.g. if `uint64_t original` is split into `uint32_t part1`, `uint32_t part2` then to combine use `part1 | (uint64_t)part2 << 32`

### Build tools
* When compiling x86-32_clang with target set to i386-elf the os was incorrectly linked
	* Changed of target to i686-elf fixed incorrect linkage
* Older c compilers added _ before all symbols(e.g. "symbol" was made into "_symbol"), newer c compilers do not do this so when interfacing with assembly same function/variable names should be used
	* To check if compiler adds _ before symbols use - "objdump -t filename.ext"
* When linking problems occured when in linker flags -L <lib>(include library) was used after -l <lib>(use library)
	* Swaping -L and -l places fixed the linker errors
* [The compiler does not follow packed attribute if there is 64bit field in the structure](https://forum.osdev.org/viewtopic.php?t=30318)
