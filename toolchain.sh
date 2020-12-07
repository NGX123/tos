install_list_var="nasm binutils diffutils valgrind clang gcc qemu-system-x86"

## INSTALLATION CONFIGURATION ##
read -p "Package Manager(dnf, apt, macos): " pm_var
read -p "Should the x86_32/x86_64 cross-compiler be compiled(no/64/32): " x86_build_var
read -p "Should the UEFI be compiled(y/n): " uefi_build_var

## PACKAGE INSTALLATIONS ##
# DNF Installations
if [ $pm_var == dnf ]
  then
    # Install developer tools and headers
    sudo dnf -y install $install_list_var @development-tools kernel-headers kernel-devel edk2-ovmf

    # Install x86/x86_64 Cross-compiler build dependencies
    if [ $x86_build_var != no]
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
        sudo apt -y install build-essential uuid-dev iasl gcc-5 nasm python3-distutils
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
# Setup for compiling the x86_32 compiler
if [ $x86_build_var == 32 ]
  then
    # Create the nesecerry direcotries
    mkdir -p $HOME/src/cross-compiler/binutils2.30_i686/build/
    mkdir -p $HOME/src/cross-compiler/gcc9.3.0_i686/build/
    mkdir -p $HOME/opt/

    # Download and unpack source code
    cd $HOME/src/cross-compiler/binutils2.30_i686/
    wget https://ftp.gnu.org/gnu/binutils/binutils-2.30.tar.gz
    tar -xzf binutils-2.30.tar.gz
    rm binutils-2.30.tar.gz

    cd $HOME/src/cross-compiler/gcc9.3.0_i686/
    wget https://ftp.gnu.org/gnu/gcc/gcc-9.3.0/gcc-9.3.0.tar.gz
    tar -xzf gcc-9.3.0.tar.gz
    rm gcc-9.3.0.tar.gz

    # Declare the variables
    export PREFIX="$HOME/opt/"
    export TARGET=i686-elf
    export PATH="$PREFIX/bin:$PATH"
fi

# Setup for compiling the x86_64 compiler
if [ $x86_build_var == 64 ]
  then
    # Create the nesecerry direcotries
    mkdir -p $HOME/src/cross-compiler/binutils2.30_amd64/build/
    mkdir -p $HOME/src/cross-compiler/gcc9.3.0_amd64/build/
    mkdir -p $HOME/opt/

    # Download and unpack source code
    cd $HOME/src/cross-compiler/binutils2.30_amd64/
    wget https://ftp.gnu.org/gnu/binutils/binutils-2.30.tar.gz
    tar -xzf binutils-2.30.tar.gz
    rm binutils-2.30.tar.gz

    cd $HOME/src/cross-compiler/gcc9.3.0_amd64/
    wget https://ftp.gnu.org/gnu/gcc/gcc-9.3.0/gcc-9.3.0.tar.gz
    tar -xzf gcc-9.3.0.tar.gz
    rm gcc-9.3.0.tar.gz

    echo "MULTILIB_OPTIONS += mno-red-zone
MULTILIB_DIRNAMES += no-red-zone" > gcc/config/i386/t-x86_64-elf
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

    # Declare the variables
    export PREFIX="$HOME/opt/"
    export TARGET=x86_64-elf
    export PATH="$PREFIX/bin:$PATH"
fi

# Setup for compiling the OVMF UEFI
if [ $uefi_build_var == y ]
  then
    # Create the necessery directories
    mkdir -p $HOME/src/ovmf/

    # Download the source code
    git clone https://github.com/tianocore/edk2 $HOME/src/ovmf/
    cd $HOME/src/ovmf/
    git submodule update --init
fi



## BUILD PROCCESS ##
# Compile the x86_32 cross-compiler
if [ $x86_build_var == 32 ]
  then
    # Building binutils
    cd $HOME/src/cross-compiler/binutils2.30_i686/build/
    ../binutils-2.30/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
    make install

    # Building GCC
    cd $HOME/src/cross-compiler/gcc9.3.0_i686/build/
    which -- $TARGET-as || echo $TARGET-as is not in the PATH
    ../gcc-9.3.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-language=c,c++ --without-headers
    make all-gcc
    make all-target-libgcc
    make install-gcc
    make install-target-libgcc
fi

if [ $x86_build_var == 64 ]
  then
    # Building binutils
    cd $HOME/src/cross-compiler/binutils2.30_amd64/build/
    ../binutils-2.30/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
    make install

    # Building GCC
    cd $HOME/src/cross-compiler/gcc9.3.0_amd64/build/
    which -- $TARGET-as || echo $TARGET-as is not in the PATH
    ../gcc-9.3.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-language=c,c++ --without-headers
    make all-gcc
    make all-target-libgcc
    make install-gcc
    make install-target-libgcc
fi

# Compile the OVMF UEFI
if [ $uefi_build_var == y ]
  then
    cd $HOME/src/ovmf/
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
    cd Build/OvmfPkgX64/DEBUG_GCC5/FV
fi

# Seperate build instructions if the MacOS is used
if [ $pm_var == macos]
  if [ $x86_build_var == 32 ]
    then
      # Building binutils
      cd $HOME/src/cross-compiler/binutils2.30/build
      ../binutils-2.30/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
      make
      make install

      # Building GCC
      cd $HOME/src/cross-compiler/gcc9.3.0/build
      which -- $TARGET-as || echo $TARGET-as is not in the PATH
      ../gcc-9.3.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-language=c,c++ --without-headers --enable-multilib --with-libiconv-prefix=/usr/local/opt/libiconv/
      make all-gcc
      make all-target-libgcc
      make install-gcc
      make install-target-libgcc
  fi
fi




# CHECK OF INSTALLTION
qemu-system-i386 --version
qemu-system-x86_64 --version
if [ x86_build_var != no]
  then
    $TARGET-gcc --version
    if [ x86_build_var == 64 ]
      then
        find $PREFIX/lib -name 'libgcc.a'
    fi
fi
if [ uefi_build_var == y ]
  then
    ls $HOME/src/ovmf/Build
grub2-mkrescue --version
# If kernel headers installation on debian not work check "ls -l /usr/src/linux-headers-$(uname -r)"(if does not exist then there are no headers), insetad try to find the latest version if not installed