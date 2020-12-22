install_list_var="nasm binutils diffutils valgrind clang gcc qemu-system-x86 gnu-efi"
CROSS_GCC_VERSION="9.3.0"
CROSS_BINUTILS_VERSION="2.30"
TOOLCHAIN_SRC="./make/src"
TOOLCHAIN_PREFIX="./make/tools"

## INSTALLATION CONFIGURATION ##
read -p "Package Manager(dnf, apt, macos): " pm_var
read -p "Should the UEFI be compiled(y/n): " uefi_build_var
read -p "Do you want to configure other options(y/n): " extra_config_var

# Extra configuration of the build
if [ $extra_config_var == y ]
  then
    read -p "Should the x86_32/x86_64 cross-compiler be compiled(no/64/32): " x86_build_var

    # Configure the build directory
    read -p "Do you want to customize the build path(y/n): " PREFIX_OPTION
    if [ $PREFIX_OPTION == y ]
      then
        read -p "Enter a new build directory path: " TOOLCHAIN_PREFIX
    fi
fi




## PACKAGE INSTALLATIONS ##
# DNF Installations
if [ $pm_var == dnf ]
  then
    # Install developer tools and headers
    sudo dnf -y install $install_list_var @development-tools kernel-headers kernel-devel edk2-ovmf

    # Install x86/x86_64 Cross-compiler build dependencies
    if [ $x86_build_var != no ]
      then
        sudo dnf -y install gcc gcc-c++ make bison flex gmp-devel libmpc-devel mpfr-devel texinfo automake autoconf xorriso @development-tools
    fi

    # Install OVMF UEFI Dependencies
    if [ $uefi_build_var == y ]
      then
        sudo dnf -y install @development-tools gcc-c++ iasl libuuid-devel nasm edk2-tools-python
    fi
fi

# APT Installations
if [ $pm_var == apt ]
  then
    # Install developer tools and headers
    sudo apt -y install $install_list_var build-essential linux-headers-$(uname -r) ovmf

    # Install x86/x86_64 Cross-compiler build dependencies
    if [ $x86_build_var != no ]
      then
        sudo apt -y install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
    fi

    # Install OVMF UEFI build dependencies
    if [ $uefi_build_var == y ]
      then
        sudo apt -y install build-essential uuid-dev iasl nasm python3-distutils
    fi
fi

if [ $pm_var == macos ]
  then
    # Packages
    brew install gdb nasm binutils diffutils

    # Dependencies
    brew install gmp mpfr libmpc libiconv
    brew install libiconv
    echo "There maybe problems during the compilation proccess due to wrong attributes, then remove -with-sysroot and add --enable-interwork to both gcc and binutils"
fi




## COMPILATION SETUP ##
# Setup for compiling the OVMF UEFI
if [ $uefi_build_var == y ]
  then
    # Create the necessery directories
    mkdir -p "$TOOLCHAIN_SRC"/ovmf/

    # Download the source code
    git clone https://github.com/tianocore/edk2 "$TOOLCHAIN_SRC"/ovmf/
    cd "$TOOLCHAIN_SRC"/ovmf/
    git submodule update --init
fi




## BUILD PROCCESS ##
# Compile the OVMF UEFI
if [ $uefi_build_var == y ]
  then
    cd "$TOOLCHAIN_SRC"/ovmf/
    make -C BaseTools
    . edksetup.sh
    echo "
    ACTIVE_PLATFORM=OvmfPkg/OvmfPkgX64.dsc
    TARGET = DEBUG
    TARGET_ARCH = X64
    TOOL_CHAIN_CONF = Conf/tools_def.txt
    TOOL_CHAIN_TAG = GCC5
    BUILD_RULE_CONF = Conf/build_rule.txt" > Conf/target.txt
    build
    cd Build/OvmfX64/DEBUG_GCC5/FV
fi




## EXTRA ##
# If kernel headers installation on debian not work check "ls -l /usr/src/linux-headers-$(uname -r)"(if does not exist then there are no headers), insetad try to find the latest version if not installed
# Setup for compiling the x86_32 compiler
if [ $x86_build_var == 32 ]
  then
    # Create the nesecerry direcotries
    mkdir -p "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_i686/build/
    mkdir -p "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_i686/build/
    mkdir -p "$TOOLCHAIN_PREFIX"

    # Download and unpack source code
    cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_i686/
    wget https://ftp.gnu.org/gnu/binutils/binutils-"$CROSS_BINUTILS_VERSION".tar.gz
    tar -xzf binutils-"$CROSS_BINUTILS_VERSION".tar.gz
    rm binutils-"$CROSS_BINUTILS_VERSION".tar.gz

    cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_i686/
    wget https://ftp.gnu.org/gnu/gcc/gcc-"$CROSS_GCC_VERSION"/gcc-"$CROSS_GCC_VERSION".tar.gz
    tar -xzf gcc-"$CROSS_GCC_VERSION".tar.gz
    rm gcc-"$CROSS_GCC_VERSION".tar.gz

    # Declare the variables
    export PREFIX="$TOOLCHAIN_PREFIX"
    export TARGET=i686-elf
    export PATH="$PREFIX/bin:$PATH"
fi

# Setup for compiling the x86_64 compiler
if [ $x86_build_var == 64 ]
  then
    # Create the nesecerry direcotries
    mkdir -p "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_amd64/build/
    mkdir -p "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_amd64/build/
    mkdir -p "$TOOLCHAIN_PREFIX"

    # Download and unpack source code
    cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_amd64/
    wget https://ftp.gnu.org/gnu/binutils/binutils-"$CROSS_BINUTILS_VERSION".tar.gz
    tar -xzf binutils-"$CROSS_BINUTILS_VERSION".tar.gz
    rm binutils-"$CROSS_BINUTILS_VERSION".tar.gz

    cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_amd64/
    wget https://ftp.gnu.org/gnu/gcc/gcc-"$CROSS_GCC_VERSION"/gcc-"$CROSS_GCC_VERSION".tar.gz
    tar -xzf gcc-"$CROSS_GCC_VERSION".tar.gz
    rm gcc-"$CROSS_GCC_VERSION".tar.gz

    echo "MULTILIB_OPTIONS += mno-red-zone
MULTILIB_DIRNAMES += no-red-zone" > gcc-"$CROSS_GCC_VERSION"/gcc/config/i386/t-x86_64-elf
    echo '
    Add this:
      tmake_file="${tmake_file} i386/t-x86_64-elf" # include the new multilib configuration
    To make this:
    x86_64-*-elf*)
 	    tm_file="${tm_file} i386/unix.h i386/att.h dbxelf.h elfos.h newlib-stdint.h i386/i386elf.h i386/x86-64.h"
 	    ;;
    Look like this:
      x86_64-*-elf*)
	    tmake_file="${tmake_file} i386/t-x86_64-elf" # include the new multilib configuration
 	    tm_file="${tm_file} i386/unix.h i386/att.h dbxelf.h elfos.h newlib-stdint.h i386/i386elf.h i386/x86-64.h"
 	    ;;'
    read -p "Continue: " continue_var
    nano gcc-"$CROSS_GCC_VERSION"/gcc/config.gcc

    # Declare the variables
    export PREFIX="$TOOLCHAIN_PREFIX"
    export TARGET=x86_64-elf
    export PATH="$PREFIX/bin:$PATH"
fi

# Compile the x86_32 cross-compiler
if [ $x86_build_var == 32 ]
  then
    # Building binutils
    cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_i686/build/
    ../binutils-"$CROSS_BINUTILS_VERSION"/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
    make install

    # Building GCC
    cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_i686/build/
    which -- $TARGET-as || echo $TARGET-as is not in the PATH
    ../gcc-"$CROSS_GCC_VERSION"/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-language=c,c++ --without-headers
    make all-gcc
    make all-target-libgcc
    make install-gcc
    make install-target-libgcc
fi

# Compile the x86_64 cross-compiler
if [ $x86_build_var == 64 ]
  then
    # Building binutils
    cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_amd64/build/
    ../binutils-"$CROSS_BINUTILS_VERSION"/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
    make install

    # Building GCC
    cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_amd64/build/
    which -- $TARGET-as || echo $TARGET-as is not in the PATH
    ../gcc-"$CROSS_GCC_VERSION"/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-language=c,c++ --without-headers
    make all-gcc
    make all-target-libgcc
    make install-gcc
    make install-target-libgcc
fi

# Seperate build instructions if the MacOS is used
if [ $pm_var == macos ]
  then
    if [ $x86_build_var == 32 ]
      then
        # Building binutils
        cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"/build
        ../binutils-"$CROSS_BINUTILS_VERSION"/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
        make
        make install

        # Building GCC
        cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"/build
        which -- $TARGET-as || echo $TARGET-as is not in the PATH
        ../gcc-"$CROSS_GCC_VERSION"/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-language=c,c++ --without-headers --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
        make all-gcc
        make all-target-libgcc
        make install-gcc
        make install-target-libgcc
    fi
fi




## CHECK OF INSTALLTION ##
qemu-system-i386 --version
qemu-system-x86_64 --version
if [ uefi_build_var == y ]
  then
    ls "$TOOLCHAIN_SRC"/ovmf/Build
fi
if [ $pm_var == dnf ]
  then
    grub2-mkrescue --version
  else
    grub-mkrescue --version
fi

# Extra
if [ x86_build_var == 64 ]
  then
    x86_64-elf-gcc --version
    find $PREFIX/lib -name 'libgcc.a'
  else
    if [ x86_build_var == 32 ]
      then
        i686-elf-gcc --version
    fi
fi