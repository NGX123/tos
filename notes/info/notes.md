## Fixes
### Hardware
* The memory map has reserved areas below 1mb and right at the end of it, all other space is free. The map will change depending on amount of RAM(in 200mb, only about 150 will be free)

### Code
* When using functions from `<stdarg.h>` like `va_start(), va_end(), va_arg()`, the function where they were used should not call other functions(e.g. if `func1()` uses va_start(), `func2()` or any other function can't be called from `func1()`)

### Build tools
* When compiling x86-32_clang with target set to i386-elf the os was incorrectly linked
	* Changed of target to i686-elf fixed incorrect linkage
* Older c compilers added _ before all symbols(e.g. "symbol" was made into "_symbol"), newer c compilers do not do this so when interfacing with assembly same function/variable names should be used
	* To check if compiler adds _ before symbols use - "objdump -t filename.ext"
* When linking problems occured when in linker flags -L <lib>(include library) was used after -l <lib>(use library)
	* Swaping -L and -l places fixed the linker errors
