## To Do
1. Fix multiboot2_bootstrap(so for example the multibootsetup does not have to be commented out to run the 32 bit OS)
	1. Instead make the code after the init - kernel(64-Bit code in case of x86-32) call bootloaderInterface with INIT command and it would do the needed bootloader setup based on the boot protocol
	2. Make getmemorymap add entries about where multiboot_info starts and finishes - so it is not touched and where framebuffer starts and finished
2. Implement simple memory management
	1. Write Page Frame Allocator
		* Should have a function to reserve physical memory based a request from device driver - special function that asks not to give it a page(4096) but to reserved all pages from start address to end address(and the ones on which start and end are located)
			* Drivers like the framebuffer driver should mark the memory mapped I/O and the memory areas that they need as reserved using PFA special function
	2. Write Virtual Memory Manager(Page manager)
	3. Write Virtual Memory allocator(malloc)
3. Implement 64-Bit interrupts
	* Look into APIC
	1. Read intel manual on 64 bit interrupt and IDT
	2. Read Intel manual on APIC
		* Understand differences between IOAPIC, LAPIC and APIC
4. Implement exeption handling to handle things like page faults
	* How to spot difference between exeptions and interrupts if same interrupt vectors are used
	* Map needed interrupt handlers to functions that handler the exeptions
5. Do a check if the toolchain.sh works right - look through source code, execute and then checks if everything done in source is in the results

### [Hardware](https://wiki.osdev.org/Category:Hardware)
1. [64bit CPU support(Long mode)](https://wiki.osdev.org/Category:X86-64)
	1. [Setting up stack](https://forum.osdev.org/viewtopic.php?f=1&t=21772)
	2. [Setup Paging](https://wiki.osdev.org/Setting_Up_Paging)
2. [Graphics](https://wiki.osdev.org/Category:Video)
	* Implement video output detection - GRUB, GOP, VGA Text Mode...
	* [GRUB Framebuffer]()
		1. Supply the multiboot2 header with framebuffer tag(5 in documentation)
		2. Copy and modify the driver from documentation
	* [GOP](https://wiki.osdev.org/GOP)
		1. Use the GRUB Header tags to make it load os into "i386 with EFI boot servicers not exited"
		2. Setup a framebuffer with GOP
		3. exitbootservices()
		4. Write a driver for using the raw framebuffer that was before used by UEFI
		* [GOP Thread](https://forum.osdev.org/viewtopic.php?f=1&t=26796)
2. [Interrupts](https://wiki.osdev.org/Category:Interrupts)
	1. [APIC(Modern)](https://wiki.osdev.org/APIC)
		* [IOAPIC](https://wiki.osdev.org/IOAPIC)
		* [APIC Timer](https://wiki.osdev.org/APIC_timer)
	* [PIC(Old)](https://wiki.osdev.org/8259_PIC)
3. Time devices
	1. [HPET(Modern RTC & PIT))](https://wiki.osdev.org/HPET)
	* [PIT(Old)](https://wiki.osdev.org/Programmable_Interval_Timer)
		* Implemenent it as a device file
		* Commands from write() buffer will be sent to the PIT
		* PIT interrupts will be sent to the function that was binded to this interrupt with signal() syscall
	* [RTC(Old)](https://wiki.osdev.org/RTC)
4. [ACPI](https://wiki.osdev.org/ACPI)
	1. Fix the driver
		1. Make the addresses more architecture independant(instead of uint32_t and uint64_t where possible change to unsigned long)
		2. saveSDT:
			* Loop through tables pointed to by RSDT, fix the problem where only FADT has the signature
			* Count size of all tables pointed to by RSDT
			* Allocate the space for the tables with malloc in a static global variable so it could be used by other functions
			* Memcpy the tables into allocated buffer
			* Memcmp the original tables and ones in the buffer
		3. retrieveSDT:
			* Find the SDT in the malloced buffer using signature requested by user
			* Memcpy SDT from buffer to user supplied location if size of table does not exceed max size specified by user
			* Memcmp buffer and user versions
	* [ACPICA support](https://wiki.osdev.org/Category:ACPI)
5. [ATA](https://wiki.osdev.org/Category:ATA)
	1. [AHCI(Modern SATA native way)](https://wiki.osdev.org/AHCI)
	* [PIO(Old, easier mode)](https://wiki.osdev.org/ATA_PIO_Mode)
6. [USB support(PS/2, Serial, Parallel... alternative)](https://wiki.osdev.org/Category:USB)
	* [PS/2 Controller](https://wiki.osdev.org/%228042%22_PS/2_Controller)
		* Add 8042 presence check using ACPI
		* Instead of checking the status byte once and if the buffer full/empty bit is not in the right position failing, poll the status bit until time is out or it is in the right position, tracking time should be done with PIT
		* Implement PS/2 controller as a device file
			* Character device file
			* Output buffer(read with read()) will contain structures with keypress information
			* Input buffer(wrote to with write()) will send commands to the PS/2 controller
				* The message sent will first be checked to be valid(in range of possible)
7. [PCI](https://wiki.osdev.org/PCI)
	* [PCI Express](https://wiki.osdev.org/PCI_Express)
	* Devices
		* [Networking](https://wiki.osdev.org/Category:Network_Hardware)
		* [Intel Sound](https://wiki.osdev.org/Intel_High_Definition_Audio)
8. [Advanced integrated peripheral - alternative to all of the old chips(e.g. PIC)](https://wiki.osdev.org/Advanced_Integrated_Peripheral)





## Kernel
1. [Memory management](https://wiki.osdev.org/Category:Memory_management)
	1. [Physical Memory Manager(PMM) - allocates physical pages](https://wiki.osdev.org/Page_Frame_Allocation)
		* Mark all memory regions reserved except the ones specified in memory map as free(no matter if region is really free or not, mem map should be followed)
		* The maximum address at which page could end should be no more then physical amount of upper RAM
		* For x86 - mark all areas below 1mb as reserved
	2. [Virtual Memory Manager - mapps physical pages to virtual](https://wiki.osdev.org/Memory_Allocation)
	3. [Memory Allocator - allocates space from virtual pages]()
2. [Time Library]()
	* A timer could be set by a programm and kernel will call the binded function when the time is up, the kernel will manage the time with whatever time device was found using the device manager(e.g. PIC)
3. Driver functions should be not global symbols and should only be accessed through things like write and read and not symbols
4. [Device manager](https://wiki.osdev.org/Device_Management)
	* [Guide](https://wiki.osdev.org/User:Kmcguire/Quick_And_Dirty_Device_Management)
	* [Thread](https://www.reddit.com/r/osdev/comments/dr2qym/device_driver_model_for_oshypervisor/)
	* Initialise drivers based on devices found
	* Make a device tree
5. [Multitasking](https://wiki.osdev.org/Category:Processes_and_Threads)
	* [Multitasking types](https://wiki.osdev.org/Multitasking_Systems)



## Features
1. Make a kernel data allocator to have a certain space in memory where kernel data structures will be stored to be safe, instead of the stack. And pieces of memory will be allocated from this space and given for filling in with data
2. Reworked Universal Keyboard Driver
3. [TTY](https://www.reddit.com/r/osdev/comments/hgzg6k/tty_questions/)
	* Be able to be used with PS/2, USB... interfaced keyboards and different video outputs
	* STDIN implementation to be read with read() syscall
	* STDOUT & STDERR to be wrote to with write() syscall
	* termios structure
	* Interchangable keymap for the keybaord with ioctl code to change it
	* Shortcuts(e.g. CTRL+C) will be implemented with signals
		* When an interrupt occurs it will be transferred to tty and the signal library will be called by it which in turn will call function binded to the interrupt(if there is one)
4. [File System](https://wiki.osdev.org/Category:Filesystems)



## Extra
* Documentation
	1. Write OS requirements in texinfo documentation
		1. Init functions that should be present - all that are prefixed with `arch_` or `kernel_`
		2. In what order should the base functions be called - bootstarp script -> kernel_setup -> kernel_init
		3. Kernel requirements - `32+ Bit` Architecture, `unsigned long` in the compiler should represent largest possible var size - include/types.h has address_tt define which should be changed if compiler does not obej
		4. The arch_bootloaderInterface should be present and have an INIT function
	2. Add documentation for the man page-like descriptions for functions that might be used by user or during porting to other platform e.g. *_arch functions
	* Translate .md notes to .texinfo to create one file out of them
	* https://www.gnu.org/software/texinfo/manual/
	* https://www.gnu.org/software/teximpatient
* Remove the 32bit mode OS, but instead have a BIOS variant of the 64bit OS(maybe by just adding another grub type in makefile)
* Clean up the source tree
	1. Change files in boot to boot protocols and not platforms
	2. Rename all the same name files like init.s to init-x86_64.s
* Try to use EDK2 headers and EDK2 objects to build a UEFI app with a normal compiler and not the EDK2 build system
	* [TomatBoot does it](https://github.com/TomatOrg/TomatBoot)
* [Assembly](https://wiki.osdev.org/Assembly)
	* Change assembly compilation from inclusion of everything into one file to seperate compilation and then linking together with other executables
	* Switch code from NASM to GAS or improve NASM skills
		* Ask on reddit, how to use clang as assembler and write in GNU GAS syntax
		* [OSdev GAS](https://wiki.osdev.org/GAS)
		* [OSdev NASM](https://wiki.osdev.org/NASM)
		* [GAS tutorial](https://en.wikibooks.org/wiki/X86_Assembly/GAS_Syntax)
		* [NASM tutorial](https://en.wikibooks.org/wiki/X86_Assembly/NASM_Syntax)
* Create a x86_64 UEFI Bootloader
	* Use tomatboot sources stripped from multiboot, bios and other useless stuff
	* Check what other stuff should be done by the bootloader, read the UEFI manuals
	1. Setup a framebuffer with GOP(that will work after exiting boot services)
	2. Exit boot services
	3. Parse the ELF headers and load OS executable
* Port the OS to ARM/RISC-V
	* [ARM Manual Collection](https://community.arm.com/developer/ip-products/processors/b/processors-ip-blog/posts/important-arm-technical-learning-resources)
	* [RISC-V Manual Collection](https://gist.github.com/cb372/5f6bf16ca0682541260ae52fc11ea3bb)


## Done
- Kernel
	* [Rework Interrupts](https://wiki.osdev.org/Category:Interrupts)
		* Make a c code for interacting with interrupts(e.g. a function to set that when particular interrupt occures it should call the supplied function pointer), and the implementation of the interrupts will be seperate for arch and will be located in it's folder and then just linked with the kernel interrupts code. Architecture specific code will have all the variables for function pointers, it's own copy of the function to bind a function pointer to interrupt, the interrupts implementation - each interrupt being a function that will call a corresponding function pointer if it was set by user. Kernel wide interrupt code will contain functions for user to bind interrupt to specific function pointer, check if the interrupt exists, permissions for binding to it...
- Hardware
	* [64bit CPU support(Long mode)](https://wiki.osdev.org/Category:X86-64)
		* Find a bootloader to use with long mode
		* [Switch to long mode](https://wiki.osdev.org/Setting_Up_Long_Mode)
	* [UEFI firmware support(boot with UEFI services)](https://wiki.osdev.org/Category:UEFI)
		* Find a working or implement a new UEFI 64bit bootloader
		* Find a way to setup video output after exitbootservices()
	* [USB support(PS/2, Serial, Parallel... alternative)](https://wiki.osdev.org/Category:USB)
		* [PS/2 Controller](https://wiki.osdev.org/%228042%22_PS/2_Controller)
			* Implement detection of PS/2 controller presence