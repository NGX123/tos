# OS Development Resources(code, books, websites...)
## Books
* Best
	* Operating Systems: 3 Easy pieces - Remzi Arpaci-Dusseau
	* OSs: Design and Implementation - Andrew Tanenbaum
	* xv6 Book(RISC-V and x86) - MIT

* Practical
	* **OSs: Design and Implementation - Andrew Tanenbaum**
	* MMURTL - Richard Burgess
	* OS Design: XINU - Douglas Comer

* Theory
	* Modern OSs - Andrew Tanenbaum
	* **xv6 Book(RISC-V and x86) - MIT**
	* **Operating Systems: 3 Easy pieces - Remzi Arpaci-Dusseau**
	* OS Concepts - Avi Silberschatz
	* Operating Systems: Internals and Design Principles - W. Stallings

* Existing OS Theory
	* Lion's Commentary on UNIX - John Lions
	* Design and Implementation of FreeBSD - Marshall McKusick
	* Design of a UNIX OS - Maurice Bach

* Linux Kernel
	* Linux Kernel Development - Robert Love
	* How Linux Works - Brian Ward
	* Understanding the Linux Kernel - D. Bovet
	* A Heavily Commented Linux Kernel Code - Zhao Jiong

* Driver Development
	* Linux Device Drivers Development - John Madieu
	* Linux Device Drivers - Jonathan Corbet
	* FreeBSD Device Drivers - Jospeh Kong



## Specifications
* CPU
	* [Intel Manuals](https://software.intel.com/content/www/us/en/develop/articles/intel-sdm.html)
		* [Intel 64 & IA-32 Programmers Manual(All Volumes)](https://software.intel.com/content/www/us/en/develop/download/intel-64-and-ia-32-architectures-sdm-combined-volumes-1-2a-2b-2c-2d-3a-3b-3c-3d-and-4.html)
		* [Intel i386(80386)(1986) Programmers Manual](https://css.csail.mit.edu/6.858/2014/readings/i386.pdf)
	* [AMD64 Manuals](https://developer.amd.com/resources/developer-guides-manuals/)
		* [AMD64 Programmer's Manual(All Volumes)](https://www.amd.com/system/files/TechDocs/40332.pdf)
* Boot
	* [UEFI Specifications](https://www.uefi.org/specifications)
	* [BIOS Specification](https://www.scs.stanford.edu/nyu/04fa/lab/specsbbs101.pdf)
	* [Multiboot 2 Specification(Used by GRUB)](https://www.gnu.org/software/grub/manual/multiboot2/)
* Programming
	* [NASM Documentation](https://www.nasm.us/docs.php)



## Courses
* Tutorials
	* [OSDev Wiki - Writing an Operating system](https://wiki.osdev.org/Creating_an_Operating_System)
	* [Bham OS Book](https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf)
	* [Bona Fide OSdev](http://www.osdever.net/tutorials/)
	* [James KernelDev Guide](http://www.jamesmolloy.co.uk/tutorial_html/)
	* [BrokenThron OS Tutorials](http://www.brokenthorn.com/Resources/OSDevIndex.html)
	* [WhyOS Lectures and Tutorials](http://wyoos.org/impressum.php?redirect=%2Findex.php)
	* [UEFI From Scratch](https://youtu.be/01-UA2LKQ9Y)
	* [OS Lectures and Classes](https://ops-class.org/)

* University Courses
	* [MIT 6.828 Course](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-828-operating-system-engineering-fall-2012/index.htm)
	* [MIT 6.828 Course Practical](https://pdos.csail.mit.edu/6.828/2020/schedule.html)
	* [MIT 6.828 Course xv6 Practical](https://pdos.csail.mit.edu/6.828/2012/)
	* [MIT 6.828 Course Tasks](https://pdos.csail.mit.edu/6.828/2016/homework/)
	* [Wisconsin University OS Lectures](http://pages.cs.wisc.edu/~bart/537/lecturenotes/titlepage.html)
	* [Harward Course PintOS Practical](https://web.stanford.edu/class/cs140/projects/pintos/pintos_1.html)

* Not standard OS courses
	* [RPi OS Tutorial](https://jsandler18.github.io/)
	* [Rust RISC-V OS](https://osblog.stephenmarz.com/index.html)
	* [Rust x86 OS](https://os.phil-opp.com/)



## Source Code
* Professional OSs
	* [UNIX(All versions)(10k)](https://minnie.tuhs.org/cgi-bin/utree.pl)
	* [Minix 1,2(50k)](https://wiki.minix3.org/doku.php?id=www:download:previousversions)
	* [Linux 0.01-0.99(10k)](https://mirrors.edge.kernel.org/pub/linux/kernel/Historic/)
	* [LittleKernel - ARM/Android proffessional kernel/bootloader(700k)](https://github.com/littlekernel/lk)
	* [FreeBSD - Clean, large code; lots of docs](https://freebsd.org/developers/cvs.html)
	* [OpenBSD - Security focused, hard code](https://github.com/openbsd/src)
	* [NetBSD - Portable, clean code](https://netbsd.org/docs/guide/en/part-compile.html)

* Educational OSs
	* [xv6(6k lines)](https://github.com/mit-pdos/xv6-public)
	* [BaseKernel - education all OS with all nesecarry parts(interrupts, video...)(14k lines)](https://github.com/dthain/basekernel)
	* [Building a 64bit bootloader](https://github.com/gmarino2048/64bit-os-tutorial)

* Hobby OSs
	* [TomatOS - small conveniet c-only OS and bootloader](https://github.com/TomatOrg/TomatOS)
	* [Micro OS - smallest OS with all PC hardware support(700 lines)](https://github.com/ssaroussi/Micro-OS)
	* [Melvix OS](https://github.com/marvinborner/Melvix)
	* [Raspberry Pi OS](https://github.com/s-matyukevich/raspberry-pi-os)
	* [Sortix - POSIX Hobby OS](https://gitlab.com/sortix/sortix)
		* Easy C Library implementation - sortix/libc