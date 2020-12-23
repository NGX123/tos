#! /bin/bash

CROSSPLATFORM_DEPENDENCIES="nasm binutils diffutils valgrind clang gcc qemu-system-x86 gnu-efi"
CROSS_GCC_VERSION="9.3.0"
CROSS_BINUTILS_VERSION="2.30"
TOOLCHAIN_SRC=$(realpath ../toolchain/src)
TOOLCHAIN_PREFIX=$(realpath ../toolchain/)
MAKE_FOLDER=$(realpath ./make)
OVMF_BUILD_OPTION=n
CROSS_GNU_TOOLS_BUILD_OPTION=n

## INSTALLATION CONFIGURATION ##
read -p "Package Manager(dnf, apt, macos, other): " TOOLCHAIN_PM
read -p "Do you want to compile the EDK2 tools(y/n): " EDK2_TOOLS_BUILD_OPTION
read -p "Do you want to configure other options(y/n): " EXTRA_CONFIG_OPTION

# Extra configuration of the build
if [ $EXTRA_CONFIG_OPTION == y ]
  then
    # GCC Toolchain Build option
    read -p "Should the x86_32/x86_64 cross-compiler be compiled(no/64/32): " CROSS_GNU_TOOLS_BUILD_OPTION

    # OVMF Build option
    read -p "Should the UEFI be compiled(y/n): " OVMF_BUILD_OPTION

    # Configure the build directory
    read -p "Do you want to customize the build path(y/n): " PREFIX_OPTION
    if [ $PREFIX_OPTION == y ]
      then
        read -p "Enter a new build directory path: " TOOLCHAIN_PREFIX
    fi
fi




## PACKAGE INSTALLATIONS ##
# DNF Installations
if [ $TOOLCHAIN_PM == dnf ]
  then
    # Install developer tools and headers
    sudo dnf -y install $CROSSPLATFORM_DEPENDENCIES @development-tools kernel-headers kernel-devel edk2-ovmf

    # Install x86/x86_64 Cross-compiler build dependencies
    if [ $CROSS_GNU_TOOLS_BUILD_OPTION != no ]
      then
        sudo dnf -y install gcc gcc-c++ make bison flex gmp-devel libmpc-devel mpfr-devel texinfo automake autoconf xorriso @development-tools
    fi

    # Install OVMF UEFI Dependencies
    if [ $OVMF_BUILD_OPTION == y ]
      then
        sudo dnf -y install @development-tools gcc-c++ iasl libuuid-devel nasm edk2-tools-python
    fi
fi

# APT Installations
if [ $TOOLCHAIN_PM == apt ]
  then
    # Install developer tools and headers
    sudo apt -y install $CROSSPLATFORM_DEPENDENCIES build-essential linux-headers-$(uname -r) ovmf

    # Install x86/x86_64 Cross-compiler build dependencies
    if [ $CROSS_GNU_TOOLS_BUILD_OPTION != no ]
      then
        sudo apt -y install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
    fi

    # Install OVMF UEFI build dependencies
    if [ $OVMF_BUILD_OPTION == y ]
      then
        sudo apt -y install build-essential uuid-dev iasl nasm python3-distutils
    fi
fi

# MacOS
if [ $TOOLCHAIN_PM == macos ]
  then
    # Packages
    brew install gdb nasm binutils diffutils coreutils

    # Dependencies
    brew install gmp mpfr libmpc libiconv
    brew install libiconv
    echo "There maybe problems during the compilation proccess due to wrong attributes, then remove -with-sysroot and add --enable-interwork to both gcc and binutils"
fi

# Other package managers
if [ $TOOLCHAIN_PM == other ]
  then
    echo "If you are here, your package manager is probably not in the list, so you need to make sure all of the libs are installed before preceding, here is the list: "
    echo $CROSSPLATFORM_DEPENDENCIES

    # GCC cross-compiler dependencies
    if [ $CROSS_GNU_TOOLS_BUILD_OPTION != no ]
      then
        echo "build-essentail(platform specific package), linux-headers, ovmf, bison, flex, gmp, libmpc, libmpfr, texinfo, xorriso, autoconf, automake"
    fi

    # OVMF dependencies
    if [ $OVMF_BUILD_OPTION == y ]
      then
        echo "build-essential uuid-dev iasl nasm python3-distutils"
    fi

    read -p "Press any button to continue, if all libs are installed"
fi




## COMPILATION SETUP ##
# Setup for compiling the OVMF UEFI
if [[ $OVMF_BUILD_OPTION == y || $EDK2_TOOLS_BUILD_OPTION == y ]]
  then
    # Create the necessery directories
    mkdir -p "$TOOLCHAIN_PREFIX"/edk2/

    # Download the source code
    git clone https://github.com/tianocore/edk2 "$TOOLCHAIN_PREFIX"/edk2/
    cd "$TOOLCHAIN_PREFIX"/edk2/
    git submodule update --init
fi




## BUILD PROCCESS ##
# Compile the OVMF UEFI
if [ $OVMF_BUILD_OPTION == y ]
  then
    cd "$TOOLCHAIN_PREFIX"/edk2/
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
fi

# Compile EDK2 Tools
if [[ $EDK2_TOOLS_BUILD_OPTION == y && $OVMF_BUILD_OPTION != y ]]
  then
    cd "$TOOLCHAIN_PREFIX"/edk2/
    make -C BaseTools
    . edksetup.sh
fi




## EXTRA ##
# If kernel headers installation on debian not work check "ls -l /usr/src/linux-headers-$(uname -r)"(if does not exist then there are no headers), insetad try to find the latest version if not installed
# Setup for compiling the x86_32 compiler
if [ $CROSS_GNU_TOOLS_BUILD_OPTION == 32 ]
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
    export PREFIX=$(realpath $TOOLCHAIN_PREFIX)
    export TARGET=i686-elf
    export PATH="$PREFIX/bin:$PATH"
fi

# Setup for compiling the x86_64 compiler
if [ $CROSS_GNU_TOOLS_BUILD_OPTION == 64 ]
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
    cat "$MAKE_FOLDER"/config.gcc > gcc-"$CROSS_GCC_VERSION"/gcc/config.gcc

    # Declare the variables
    export PREFIX=$(realpath $TOOLCHAIN_PREFIX)
    export TARGET=x86_64-elf
    export PATH="$PREFIX/bin:$PATH"
fi

# Compile the x86_32 cross-compiler
if [ $TOOLCHAIN_PM != macos ]
  then
    if [ $CROSS_GNU_TOOLS_BUILD_OPTION == 32 ]
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
    if [ $CROSS_GNU_TOOLS_BUILD_OPTION == 64 ]
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
fi

# Seperate build instructions if the MacOS is used
if [ $TOOLCHAIN_PM == macos ]
  then
    # Compile x86-32 gcc cross-compiler
    if [ $CROSS_GNU_TOOLS_BUILD_OPTION == 32 ]
      then
        # Building binutils
        cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_i686/build
        ../binutils-"$CROSS_BINUTILS_VERSION"/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
        make
        make install

        # Building GCC
        cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_i686/build
        which -- $TARGET-as || echo $TARGET-as is not in the PATH
        ../gcc-"$CROSS_GCC_VERSION"/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-language=c,c++ --without-headers --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
        make all-gcc
        make all-target-libgcc
        make install-gcc
        make install-target-libgcc
    fi

    # Compile the x86_64 gcc cross-compiler
    if [ $CROSS_GNU_TOOLS_BUILD_OPTION == 64 ]
      then
        # Building binutils
        cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_amd64/build/
        ../binutils-"$CROSS_BINUTILS_VERSION"/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
        make
        make install

        # Building GCC
        cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_amd64/build/
        which -- $TARGET-as || echo $TARGET-as is not in the PATH
        ../gcc-"$CROSS_GCC_VERSION"/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-language=c,c++ --without-headers --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
        make all-gcc
        make all-target-libgcc
        make install-gcc
        make install-target-libgcc
    fi
fi




## CHECK OF INSTALLTION ##
echo "
----- QEMU -----"
qemu-system-i386 --version
qemu-system-x86_64 --version

if [ $OVMF_BUILD_OPTION == y ]
  then
    echo "
    ----- OVMF -----"
    ls "$TOOLCHAIN_SRC"/edk2/Build
fi

if [ $EDK2_TOOLS_BUILD_OPTION == y ]
  then
    echo "
    ----- EDK2 Build Tools -----"
    ls "$TOOLCHAIN_SRC"/edk2/
fi

# Extra
if [ $CROSS_GNU_TOOLS_BUILD_OPTION == 64 ]
  then
    echo "
    --- GCC Toolchain 64 bit ---"
    $TOOLCHAIN_PREFIX/bin/x86_64-elf-gcc --version
    find $PREFIX/lib -name 'libgcc.a'
  else
    if [ $CROSS_GNU_TOOLS_BUILD_OPTION == 32 ]
      then
        echo "
        --- GCC Toolchain 32 bit ---"
        $TOOLCHAIN_PREFIX/bin/i686-elf-gcc --version
    fi
fi