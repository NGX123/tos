## Hardware Interaction
### PS/2 Keyboard
* Implement detection of PS/2 controller presence
* Implement PS/2 controller as a device file
	* Character device file
	* Output buffer(read with read()) will contain structures with keypress information
	* Input buffer(wrote to with write()) will send commands to the PS/2 controller
		* The message sent will first be checked to be valid(in range of possible)
* Interchangable keymap with ioctl code to change it
* Shortcuts(e.g. CTRL+C) will be implemented with signals
	* When an interrupt occurs the signal library will be called which in turn will call function binded to the interrupt(if there is one)


### Devices
* TTY
	* STDIN implementation to be read with read() syscall
	* STDOUT & STDERR to be wrote to with write() syscall
	* termios structure
* PIT
	* Implemenent it as a device file
	* Commands from write() buffer will be sent to the PIT
	* PIT interrupts will be sent to the function that was binded to this interrupt with signal() syscall
* Implement full featured VGA Driver
	* Make it a character device
		* Changing modes would be done with ioctl() syscall
		* Add support for the graphics mode
* Other
	* RTC
	* PCI
	* ATA


### Modern Hardware
* Long Mode
	* Find a bootloader to boot into long mode
	* Change the addresses where needed to void* or uint64_t*
* System boot/initialization with UEFI
	* Implement a bootloader
* APIC & LAPIC(PIC alternative)
* HPET(RTC & PIT alternative)
* AHCI hard drive access(IDE alternative)
* ACPICA support
* USB support(PS/2, Serial, Parallel... alternative)




## Kernel
* Make the same macro mechanism as in UEFI for all extra features
* Rework Interrupts - make a c code for interacting with interrupts(e.g. a function to set that when particular interrupt occures it should call the supplied function pointer), and the implementation of the interrupts will be seperate for arch and will be located in it's folder and then just linked with the kernel interrupts code. Architecture specific code will have all the variables for function pointers, it's own copy of the function to bind a function pointer to interrupt, the interrupts implementation - each interrupt being a function that will call a corresponding function pointer if it was set by user. Kernel wide interrupt code will contain functions for user to bind interrupt to specific function pointer, check if the interrupt exists, permissions for binding to it...
* Memory allocation
* Device manager
	* Initialise drivers based on devices found
	* Make a device tree