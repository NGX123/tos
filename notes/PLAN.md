## Hardware Plans
### PS/2 Keyboard
	+ Implement detection of PS/2 controller presence
	+ Implement PS/2 controller as a device file
		+ Character device
		+ Will put structures with information about keyress into read() buffer
		+ Will send commands from write() to the PS/2 controller
	+ Implement access to keymap to allow changing it
		+ Will be done with ioctl syscall
	+ Make shortcuts(e.g. CTRL+C) interrupt that should be tied to a function with signal() syscall

### Devices
	+ TTY
	+ PIT
		+ Implemenent it as a device file
		+ Commands from write() buffer will be sent to the PIT
		+ PIT interrupts will be sent to the function that was binded to this interrupt with signal() syscall
	+ Implement full featured VGA Driver
		+ Make it a character device
			+ Changing modes would be done with ioctl() syscall
		+ Add support for the graphics mode
	+ Other: RTC, USB, PCI, ATA

### Switch to modern hardware
	+ Change from protected mode to long mode
		+ Find a bootloader to boot into long mode
		+ Change the addresses where needed to void* or uint64_t*
	+ Implement UEFI support instead of BIOS
	+ Switch APIC and LAPIC instead of PIC
	+ Switch to HPET instead of RTC & PIT
	+ Switch to AHCI instead of ATA
	+ Add support for ACPICA

## OS Plans
	1. UEFI
		1. Implement bootloader
		2. Implement all the features of the BIOS x86-32 OS with UEFI routines
		3. Implement all coreutils and load them together with UEFI OS
		4. Define a macro during x86-64 compilation, kernel code will check if it is defined and if it is exitBootServices()
	2. Make the same macro mechanism as in UEFI for all extra features
	3. Rework Interrupts - make a c code for interacting with interrupts(e.g. a function to set that when particular interrupt occures it should call the supplied function pointer), and the implementation of the interrupts will be seperate for arch and will be located in it's folder and then just linked with the kernel interrupts code. Architecture specific code will have all the variables for function pointers, it's own copy of the function to bind a function pointer to interrupt, the interrupts implementation - each interrupt being a function that will call a corresponding function pointer if it was set by user. Kernel wide interrupt code will contain functions for user to bind interrupt to specific function pointer, check if the interrupt exists, permissions for binding to it...
	4. Memory allocator
	5. Device manager - initialises all the drivers based on devices found