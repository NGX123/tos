# TOS
My own os bulit from scratch

Later will be integrated with NUXOS


## TODO
### PS/2 Keyboard
	* Implement detection of PS/2 controller presence
	* Implement PS/2 controller as a device file
		* Character device
		* Will put structures with information about keyress into read() buffer
		* Will send commands from write() to the PS/2 controller
	* Implement access to keymap to allow changing it
		* Will be done with ioctl syscall

### Devices
	* TTY
	* PIT
		* Implemenent it as a device file
		* Commands from write() buffer will be sent to the PIT
		* PIT interrupts will be sent to the function that was binded to this interrupt with signal() syscall
	* Implement full featured VGA Driver
		* Make it a character device
			* Changing modes would be done with ioctl() syscall
		* Add support for the graphics mode
	* Other: RTC, USB, PCI, ATA

### Switch to modern hardware
	* Change from protected mode to long mode
		* Find a bootloader to boot into long mode
		* Change the addresses where needed to void* or uint64_t*
	* Implement UEFI support instead of BIOS
	* Switch APIC and LAPIC instead of PIC
	* Switch to HPET instead of RTC & PIT
	* Switch to AHCI instead of ATA
	* Add support for ACPICA

### Other
	* Memory allocator
	* Terminal
		* Make a copy of sh for linux, but for the tos
	* Make shortcuts(e.g. CTRL+C) interrupt that should be tied to a function with signal() syscall
	* Implement ioctl() syscall for all devices
