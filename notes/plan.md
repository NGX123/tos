### [Hardware](https://wiki.osdev.org/Category:Hardware)
* [PS/2 Controller](https://wiki.osdev.org/%228042%22_PS/2_Controller)
	* Implement detection of PS/2 controller presence
	* Implement PS/2 controller as a device file
		* Character device file
		* Output buffer(read with read()) will contain structures with keypress information
		* Input buffer(wrote to with write()) will send commands to the PS/2 controller
			* The message sent will first be checked to be valid(in range of possible)
* [64bit CPU support(Long mode)](https://wiki.osdev.org/Category:X86-64)
	* Find a bootloader to boot in long mode
	* Change the addresses(to void* or uint64_t*) in certain places to correspond with 64 bit mode
	* [Setup Paging](https://wiki.osdev.org/Setting_Up_Paging)
* [UEFI firmware support(boot with UEFI services)](https://wiki.osdev.org/Category:UEFI)
	* Find a working or implement a new UEFI 64bit bootloader
	* Find a way to setup video output after exitbootservices()
* [USB support(PS/2, Serial, Parallel... alternative)](https://wiki.osdev.org/Category:USB)
* [PCI](https://wiki.osdev.org/PCI)
* [ACPICA support](https://wiki.osdev.org/Category:ACPI)
* [ATA](https://wiki.osdev.org/Category:ATA)
* [Advanced integrated peripheral](https://wiki.osdev.org/Advanced_Integrated_Peripheral)
	1. [PIO(Old, easier mode)](https://wiki.osdev.org/ATA_PIO_Mode)
	2. [AHCI(Modern SATA native way)](https://wiki.osdev.org/AHCI)
* Time devices
	1. [PIT](https://wiki.osdev.org/Programmable_Interval_Timer)
		* Implemenent it as a device file
		* Commands from write() buffer will be sent to the PIT
		* PIT interrupts will be sent to the function that was binded to this interrupt with signal() syscall
	2. [RTC](https://wiki.osdev.org/RTC)
	3. [HPET(Modern RTC & PIT alternative)](https://wiki.osdev.org/HPET)
* [Interrupts](https://wiki.osdev.org/Category:Interrupts)
	* [APIC(PIC alternative)](https://wiki.osdev.org/APIC)
	* [IOAPIC](https://wiki.osdev.org/IOAPIC)



## Kernel
* [Rework Interrupts](https://wiki.osdev.org/Category:Interrupts)
	* Make a c code for interacting with interrupts(e.g. a function to set that when particular interrupt occures it should call the supplied function pointer), and the implementation of the interrupts will be seperate for arch and will be located in it's folder and then just linked with the kernel interrupts code. Architecture specific code will have all the variables for function pointers, it's own copy of the function to bind a function pointer to interrupt, the interrupts implementation - each interrupt being a function that will call a corresponding function pointer if it was set by user. Kernel wide interrupt code will contain functions for user to bind interrupt to specific function pointer, check if the interrupt exists, permissions for binding to it...
* [Time Library]()
	* A timer could be set by a programm and kernel will call the binded function when the time is up, the kernel will manage the time with whatever time device was found using the device manager(e.g. PIC)
* [Memory management](https://wiki.osdev.org/Category:Memory_management)
* [Device manager](https://wiki.osdev.org/Device_Management)
	* [Guide](https://wiki.osdev.org/User:Kmcguire/Quick_And_Dirty_Device_Management)
	* [Thread](https://www.reddit.com/r/osdev/comments/dr2qym/device_driver_model_for_oshypervisor/)
	* Initialise drivers based on devices found
	* Make a device tree



## Features
* [TTY](https://www.reddit.com/r/osdev/comments/hgzg6k/tty_questions/)
	* Be able to be used with PS/2, USB... interfaced keyboards and different video outputs
	* STDIN implementation to be read with read() syscall
	* STDOUT & STDERR to be wrote to with write() syscall
	* termios structure
	* Interchangable keymap for the keybaord with ioctl code to change it
	* Shortcuts(e.g. CTRL+C) will be implemented with signals
		* When an interrupt occurs it will be transferred to tty and the signal library will be called by it which in turn will call function binded to the interrupt(if there is one)
* [File System](https://wiki.osdev.org/Filesystem)
* A CLI shell that is close to UNIX/POSIX


## Extra
* Add special macros to hide parts of code(if macro isn't defined) to all the files that contain extra features