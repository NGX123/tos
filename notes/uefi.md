# UEFI Toolchain
## BUILD TOOLCHAIN
* [GNU-EFI](https://sourceforge.net/projects/gnu-efi/)
	* Build GNU-EFI from source
		1. [Download](https://sourceforge.net/projects/gnu-efi/)
		2. Run - make

	* Get GNU-EFI binaries
		1. sudo package-manager install gnu-efi
			* Includes and libs will be on their normal places  -/usr/lib and /usr/include

* [EDK2](https://www.tianocore.org/)
	1. [Setup dependencies](https://github.com/tianocore/tianocore.github.io/wiki/Using-EDK-II-with-Native-GCC)
	2. [Follow build instructions](https://github.com/tianocore/tianocore.github.io/wiki/Common-instructions)(Contains an error: before initialising submodules do - cd edk2)
	* The tools should be located in Build/MdeModule



## BUILD RUN ENVIRONMENT
* EDK2 OVMF UEFI Firmware
	* [Build OVMF from source](https://github.com/tianocore/tianocore.github.io/wiki/OVMF)
		1. [Setup dependencies using instructions](https://github.com/tianocore/tianocore.github.io/wiki/Using-EDK-II-with-Native-GCC)
		2. [Follow build instructions](https://github.com/tianocore/tianocore.github.io/wiki/Common-instructions)
		3. [Build the UEFI firmware itself with instructions](https://github.com/tianocore/tianocore.github.io/wiki/How-to-build-OVMF)
		* [OVMF main wiki Page](https://github.com/tianocore/tianocore.github.io/wiki/OVMF)

	* Get OVMF binaries
		1. sudo package-manager install edk2-ovmf
			* Firmware will be located in /usr/share/OVMF
		* Use package unpacker to extract .fd and .iso files from jenkins/edk2/*-ovmf-x64-*(on https://www.kraxel.org/repos)



## PROGRAMMING
* [GNU-EFI](https://github.com/tianocore/tianocore.github.io/wiki/EFI-Toolkit)

* [EDK2](https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Documents)
	* Compile with EDK2
		1. cd edk2src
		2. . edksetup.sh BaseTools
		3. mkdir edk2src/PROJECT_FOLDER
		4. cp FILE.dsc FILE.c FILE.inf edk2src/PROJECT_FOLDER
		5. build -p PROJECT_FOLDER/FILE.dsc
		6. Find compiled project in edk2src/Build

	* Programming with EDK2
		* Files - EDK2 compiles packages, which are built using instructions in a DSC file whos layout is described in documentation

		* Build options - build -\<option> \<value>, e.g. build -p hello/hello.dsc
			* -p \<PATH>/file.dsc - module to compile; releative path if inside edk2 dir, else full path
			* -b \<TYPE> - release type, e.g. DEBUG/RELEASE
			* -t \<TOOLCHAIN> - the compiler and toolchain to be used, e.g. GCC5
			* -a \<ARCHITECTURE> - architecture to compile for, e.g. X64
			* -h - get cmd options for other defines

		* Packages
			* MdePkg - includes and libs for industry standard specs
			* MdeModulePkg - module only defenitions from industry standard specs

* Documentation
	* Official Specs Collection
		* [EDK2 Documents](https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Documents)
		* [EDK2 Documentation](https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Documentation)
	* Official Guides Collection - [All articles](https://github.com/tianocore-training/Tianocore_Training_Contents/wiki)
		* [Get stared](https://github.com/tianocore/tianocore.github.io/wiki/Getting-Started-with-EDK-II)

	* UEFI Applications Building
		* [Official Specs - "Build" document describes build proccess, other - configuration](https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-Documentation#specifications)
		* Official Guide
			* [Building](https://github.com/tianocore-training/Presentation_FW/blob/master/FW/Presentations/_B_02_EDK_II_Build_Spec_Files_Pres_gp.pdf)
			* [Modules](https://github.com/tianocore-training/Presentation_FW/blob/master/FW/Presentations/_B_03_EDK_II_Modules_Libs_Drivers_Pres_gp.pdf)

	* UEFI Applications Writing
		* [Official Specs](https://github.com/tianocore/tianocore.github.io/wiki/EDK-II-User-Documentation)
			* User and Module Writer manuals
		* Official Guides
			* [App writing](https://github.com/tianocore-training/Presentation_FW/blob/master/FW/Presentations/_L_10_Writing_UEFI_App_Linux_Lab_gp.pdf)
			* [Module writing](https://github.com/tianocore-training/Presentation_FW/blob/master/FW/Presentations/_B_03_EDK_II_Modules_Libs_Drivers_Pres_gp.pdf)

	* UEFI Applications Running
		* [Firmware specs](https://github.com/tianocore/tianocore.github.io/wiki/OVMF)
		* [Shell functions specs](https://github.com/tianocore/tianocore.github.io/wiki/ShellPkg)



## RUNNING
* [To run OVMF follow insturctions and get binaries in OVMF/Build direcotry](https://github.com/tianocore/tianocore.github.io/wiki/How-to-run-OVMF)



## NOTES
* On boot UEFI searches all available storage devices for the "BOOTX64.EFI" file located in "DRIVE_NAME:EFI/BOOT/" - full path "DRIVE_NAME:EFI/BOOT/BOOTX64.EFI"(::/EFI/BOOT/BOOTX64.EFI). For other platforms the file should be names BOOT\<ARCH>.EFI
	* If the file is found - it will be ran, otherwise UEFI boots into shell
* To find the correct package that contains right includes and libs to compile use find
	* In current directory - find -name "\<REGEX>"
	* In selected directory - find \<DIRNAME> -name "\<REGEX>"