#! /bin/bash


## Variables ##
script_start_time=$(date +%s)

CROSSPLATFORM_DEPENDENCIES="nasm binutils diffutils valgrind clang gcc qemu-system-x86 gnu-efi"
CROSS_GCC_VERSION="9.3.0"
CROSS_BINUTILS_VERSION="2.30"
TOOLCHAIN_PREFIX=$(realpath ../toolchain/)
MAKE_FOLDER=$(realpath ./make)

mkdir -p ../toolchain/src   # toolchain_src - Can't be decalred without making folder
TOOLCHAIN_SRC=$(realpath ../toolchain/src)

target_x86_32=i686-elf
target_x86_64=x86_64-elf

build_ovmf=n
build_gnu_tools=n
build_gnuefi=n


## Installation Config ##
read -r -p "Package Manager(dnf, apt, macos, other): " TOOLCHAIN_PM
read -r -p "Do you want to compile the EDK2 tools(y/n): " EDK2_TOOLS_BUILD_OPTION
read -r -p "Do you want to configure other options(y/n): " EXTRA_CONFIG_OPTION

# Extra configuration of the build
if [ "$EXTRA_CONFIG_OPTION" == y ]
  then
    # GCC Toolchain Build option
    read -r -p "Should the x86_32/x86_64 cross-compiler be compiled(no/all/64/32): " build_gnu_tools

    # OVMF Build option
    read -r -p "Should the UEFI be compiled(y/n): " build_ovmf

    # GNU-EFI Build option
    read -r -p "Should the GNU-EFI be compiled(y/n): " build_gnuefi

    # Configure the build directory
    read -r -p "Do you want to customize the build path(y/n): " PREFIX_OPTION
    if [ "$PREFIX_OPTION" == y ]
      then
        read -r -p "Enter a new build directory path: " TOOLCHAIN_PREFIX
    fi
fi


## Package installations ##
# DNF Installations
if [ "$TOOLCHAIN_PM" == dnf ]
  then
    # Install developer tools and headers
    sudo dnf -y install $CROSSPLATFORM_DEPENDENCIES @development-tools kernel-headers kernel-devel edk2-ovmf

    # Install x86/x86_64 Cross-compiler build dependencies
    if [ "$build_gnu_tools" != no ]
      then
        sudo dnf -y install gcc gcc-c++ make bison flex gmp-devel libmpc-devel mpfr-devel texinfo automake autoconf xorriso @development-tools
    fi

    # Install OVMF UEFI Dependencies
    if [ "$build_ovmf" == y ]
      then
        sudo dnf -y install @development-tools gcc-c++ iasl libuuid-devel nasm edk2-tools-python
    fi
fi

# APT Installations
if [ "$TOOLCHAIN_PM" == apt ]
  then
    # Install developer tools and headers
    sudo apt -y install $CROSSPLATFORM_DEPENDENCIES build-essential linux-headers-$(uname -r) ovmf

    # Install x86/x86_64 Cross-compiler build dependencies
    if [ "$build_gnu_tools" != no ]
      then
        sudo apt -y install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
    fi

    # Install OVMF UEFI build dependencies
    if [ "$build_ovmf" == y ]
      then
        sudo apt -y install build-essential uuid-dev iasl nasm python3-distutils
    fi
fi

# MacOS
if [ "$TOOLCHAIN_PM" == macos ]
  then
    # Packages
    brew install gdb nasm binutils diffutils coreutils

    # Dependencies
    brew install gmp mpfr libmpc libiconv
    brew install libiconv
    echo "There maybe problems during the compilation proccess due to wrong attributes, then remove -with-sysroot and add --enable-interwork to both gcc and binutils"
fi

# Other package managers
if [ "$TOOLCHAIN_PM" == other ]
  then
    echo "If you are here, your package manager is probably not in the list, so you need to make sure all of the libs are installed before preceding, here is the list: "
    echo "$CROSSPLATFORM_DEPENDENCIES"

    # GCC cross-compiler dependencies
    if [ "$build_gnu_tools" != no ]
      then
        echo "build-essentail(platform specific package), linux-headers, ovmf, bison, flex, gmp, libmpc, libmpfr, texinfo, xorriso, autoconf, automake"
    fi

    # OVMF dependencies
    if [ "$build_ovmf" == y ]
      then
        echo "build-essential uuid-dev iasl nasm python3-distutils"
    fi

    read -r -p "Press any button to continue, if all libs are installed"
fi


## Compilation setup ##
# Setup for compiling the OVMF UEFI
if [[ $build_ovmf == y || $EDK2_TOOLS_BUILD_OPTION == y ]]
  then
    # Create the necessery directories
    mkdir -p "$TOOLCHAIN_PREFIX"/edk2/

    # Download the source code
    git clone https://github.com/tianocore/edk2 "$TOOLCHAIN_PREFIX"/edk2/
    cd "$TOOLCHAIN_PREFIX"/edk2/ || exit
    git submodule update --init
fi


## Build proccess ##
# Compile the OVMF UEFI
if [ "$build_ovmf" == y ]
  then
    cd "$TOOLCHAIN_PREFIX"/edk2/ || exit
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
if [[ $EDK2_TOOLS_BUILD_OPTION == y && $build_ovmf != y ]]
  then
    cd "$TOOLCHAIN_PREFIX"/edk2/ || exit
    make -C BaseTools
    . edksetup.sh
fi


## Extra ##
# If kernel headers installation on debian not work check "ls -l /usr/src/linux-headers-$(uname -r)"(if does not exist then there are no headers), insetad try to find the latest version if not installed
# Setup for GNU-EFI toolkit compilation
if [ "$build_gnuefi" == y ]
  then
    mkdir -p "$TOOLCHAIN_PREFIX"/gnu-efi/
    git clone https://git.code.sf.net/p/gnu-efi/code "$TOOLCHAIN_PREFIX"/gnu-efi/
fi

# Setup for compiling the x86_32 compiler
if [[ $build_gnu_tools == 32 || $build_gnu_tools == all ]]
  then
    # Create the nesecerry direcotries
    mkdir -p "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_i686/build/
    mkdir -p "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_i686/build/
    mkdir -p "$TOOLCHAIN_PREFIX"

    # Download and unpack source code
    cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_i686/ || exit
    wget https://ftp.gnu.org/gnu/binutils/binutils-"$CROSS_BINUTILS_VERSION".tar.gz
    tar -xzf binutils-"$CROSS_BINUTILS_VERSION".tar.gz
    rm binutils-"$CROSS_BINUTILS_VERSION".tar.gz

    cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_i686/ || exit
    wget https://ftp.gnu.org/gnu/gcc/gcc-"$CROSS_GCC_VERSION"/gcc-"$CROSS_GCC_VERSION".tar.gz
    tar -xzf gcc-"$CROSS_GCC_VERSION".tar.gz
    rm gcc-"$CROSS_GCC_VERSION".tar.gz
fi

# Setup for compiling the x86_64 compiler
if [[ $build_gnu_tools == 64 || $build_gnu_tools == all ]]
  then
    # Create the nesecerry direcotries
    mkdir -p "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_amd64/build/
    mkdir -p "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_amd64/build/
    mkdir -p "$TOOLCHAIN_PREFIX"

    # Download and unpack source code
    cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_amd64/ || exit
    wget https://ftp.gnu.org/gnu/binutils/binutils-"$CROSS_BINUTILS_VERSION".tar.gz
    tar -xzf binutils-"$CROSS_BINUTILS_VERSION".tar.gz
    rm binutils-"$CROSS_BINUTILS_VERSION".tar.gz

    cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_amd64/ || exit
    wget https://ftp.gnu.org/gnu/gcc/gcc-"$CROSS_GCC_VERSION"/gcc-"$CROSS_GCC_VERSION".tar.gz
    tar -xzf gcc-"$CROSS_GCC_VERSION".tar.gz
    rm gcc-"$CROSS_GCC_VERSION".tar.gz

    echo "MULTILIB_OPTIONS += mno-red-zone
MULTILIB_DIRNAMES += no-red-zone" > gcc-"$CROSS_GCC_VERSION"/gcc/config/i386/t-x86_64-elf
    cat "$MAKE_FOLDER"/config.gcc > gcc-"$CROSS_GCC_VERSION"/gcc/config.gcc
fi

# Compile the GNU EFI toolkit
if [ "$build_gnuefi" == y ]
  then
    cd "$TOOLCHAIN_PREFIX"/gnu-efi/ || exit
    make
fi

# Compile the x86_32 cross-compiler
if [ "$TOOLCHAIN_PM" != macos ]
  then
    if [[ $build_gnu_tools == 32 || $build_gnu_tools == all ]]
      then
        # Building binutils
        cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_i686/build/ || exit
        ../binutils-"$CROSS_BINUTILS_VERSION"/configure --target=$target_x86_32 --prefix="$TOOLCHAIN_PREFIX" --with-sysroot --disable-nls --disable-werror
        make
        make install

        # Building GCC
        cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_i686/build/ || exit
        which -- $target_x86_32-as || echo $target_x86_32-as is not in the PATH
        ../gcc-"$CROSS_GCC_VERSION"/configure --target=$target_x86_32 --prefix="$TOOLCHAIN_PREFIX" --disable-nls --enable-language=c,c++ --without-headers
        make all-gcc
        make all-target-libgcc
        make install-gcc
        make install-target-libgcc
    fi

    # Compile the x86_64 cross-compiler
    if [[ $build_gnu_tools == 64 || $build_gnu_tools == all ]]
      then
        # Building binutils
        cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_amd64/build/ || exit
        ../binutils-"$CROSS_BINUTILS_VERSION"/configure --target=$target_x86_64 --prefix="$TOOLCHAIN_PREFIX" --with-sysroot --disable-nls --disable-werror
        make
        make install

        # Building GCC
        cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_amd64/build/ || exit
        which -- $target_x86_64-as || echo $target_x86_64-as is not in the PATH
        ../gcc-"$CROSS_GCC_VERSION"/configure --target=$target_x86_64 --prefix="$TOOLCHAIN_PREFIX" --disable-nls --enable-language=c,c++ --without-headers
        make all-gcc
        make all-target-libgcc
        make install-gcc
        make install-target-libgcc
    fi
fi

# Seperate build instructions if the MacOS is used
if [ "$TOOLCHAIN_PM" == macos ]
  then
    # Compile x86-32 gcc cross-compiler
    if [[ $build_gnu_tools == 32 || $build_gnu_tools == all ]]
      then
        # Building binutils
        cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_i686/build || exit
        ../binutils-"$CROSS_BINUTILS_VERSION"/configure --target=$target_x86_32 --prefix="$TOOLCHAIN_PREFIX" --with-sysroot --disable-nls --disable-werror --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
        make
        make install

        # Building GCC
        cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_i686/build || exit
        which -- $target_x86_32-as || echo $target_x86_32-as is not in the PATH
        ../gcc-"$CROSS_GCC_VERSION"/configure --target=$target_x86_32 --prefix="$TOOLCHAIN_PREFIX" --disable-nls --enable-language=c,c++ --without-headers --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
        make all-gcc
        make all-target-libgcc
        make install-gcc
        make install-target-libgcc
    fi

    # Compile the x86_64 gcc cross-compiler
    if [[ $build_gnu_tools == 64 || $build_gnu_tools == all ]]
      then
        # Building binutils
        cd "$TOOLCHAIN_SRC"/binutils"$CROSS_BINUTILS_VERSION"_amd64/build/ || exit
        ../binutils-"$CROSS_BINUTILS_VERSION"/configure --target=$target_x86_64 --prefix="$TOOLCHAIN_PREFIX" --with-sysroot --disable-nls --disable-werror --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
        make
        make install

        # Building GCC
        cd "$TOOLCHAIN_SRC"/gcc"$CROSS_GCC_VERSION"_amd64/build/ || exit
        which -- $target_x86_64-as || echo $target_x86_64-as is not in the PATH
        ../gcc-"$CROSS_GCC_VERSION"/configure --target=$target_x86_64 --prefix="$TOOLCHAIN_PREFIX" --disable-nls --enable-language=c,c++ --without-headers --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
        make all-gcc
        make all-target-libgcc
        make install-gcc
        make install-target-libgcc
    fi
fi


## Installation check ##
echo "
----- Execution Time -----"
script_end_time=$(date +%s)
script_execution_time="$((script_end_time-script_start_time))"
echo "Script took
Seconds: $script_execution_time
Minutes: $((script_execution_time / 60))"

if [ "$build_ovmf" == y ]
  then
    echo "
----- OVMF -----"
    ls "$TOOLCHAIN_PREFIX"/edk2/Build
fi

if [ "$build_gnuefi" == y ]
  then
    echo "
----- GNU-EFI Toolkit -----"
  ls "$TOOLCHAIN_PREFIX"/gnu-efi/x86_64
fi

if [ "$EDK2_TOOLS_BUILD_OPTION" == y ]
  then
    echo "
----- EDK2 Build Tools -----"
    ls "$TOOLCHAIN_PREFIX"/edk2/
fi

# Extra
if [[ $build_gnu_tools == 64 || $build_gnu_tools == all ]]
  then
    echo "
--- GCC Toolchain 64 bit ---"
    "$TOOLCHAIN_PREFIX"/bin/x86_64-elf-gcc --version
    find "$TOOLCHAIN_PREFIX"/lib -name 'libgcc.a'
fi
if [[ $build_gnu_tools == 32 || $build_gnu_tools == all ]]
  then
    echo "
--- GCC Toolchain 32 bit ---"
    "$TOOLCHAIN_PREFIX"/bin/i686-elf-gcc --version
fi