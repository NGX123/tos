# The TOS Operating System

TOS is an independant OS written in C



## Building TOS
_NOTE: These are the instructions that will were tested on Debian based systems(e.g. Debian, Ubuntu) and RHEL based systems(e.g. Fedora)_

### Cloning

`git clone https://github.com/tos`


### Toolchain
The toolchain is all the utilities neccesery for building and running the OS, to start the proccess run

`make toolchain`

Now you will be asked the package manager and you should select the appropriate, for the compiler select either depending on which version of the OS you need, OVMF should be compiled only if you know why you need it


### Build
You can build the OS when the toolchain is installed, but there are two option uefi64 which should be selected if you made a 64-bit cross-compiler and a bios32 if you made the 32 bit compiler

`make uefi64`

or

`make bios32`


### Run
Now you to run the OS just select the qemu option with the right platform(uefi64 or bios32)

`make qemu_uefi64`

or

`make qemu_bios32`


## Resources
	* The Design of the UNIX OS - Maurice Bach
	* Modern OSs - Andrew Tanenbaum
	* OSs: 3 Easy Pieces - Remzi Arpaci-Dusseau
	* OSs: Design and Implementation - Andrew Tanenbaum
	* Lion's Commentary on UNIX v6 - John Lions