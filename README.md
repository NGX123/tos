# The tOS Operating System

t Operating System is an independant OS written in C



## Information
### Description
tOS is an operating system that i want to be very seperated in pieces very independent from each other that should be able to run on any modern PC. I write this OS to improve my coding and computer skills as a hobby.

### Supported Platfroms
 * x86-64(amd64) with BIOS and UEFI boot environments
 * x86-32(IA-32) with BIOS boot environment
 * (TO BE IMPLEMENTED IN FUTURE)UEFI native application

### Screenshots
![tOS Running in QEMU with BIOS](notes/screenshot-bios.png)
![tOS Running in QEMU with UEFI(not really ready yet)](notes/screenshot-uefi.png)



## Building TOS
_NOTE: These are the instructions that will were tested on Debian based systems(e.g. Debian, Ubuntu) and RHEL based systems(e.g. Fedora)_

### Setup
Get ready for the build by getting source code and going to the directory with code

`git clone https://github.com/tos`

`cd tos/src/`


### Toolchain
First the toolchain should be made to compile the operating system. The toolchain compilation script gives a lot of options, but in most cases only the things that are outputed by default(without going to configuration) are needed, so only first two questions should be answeared

`make toolchain`

### Build



### Run
