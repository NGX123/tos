# Installs programms for the osdev
read -p "Package Manager(dnf, apt, macos): " pm_var
read -p "Should the x86_32 cross-compiler be compiled(y/n): " x86_build_var
read -p "Should the x86_64 cross-compiler be compiled(y/n): " x86_64_build_var
read -p "Should the UEFI compiled or downloaded(no/download/install): " uefi_build_var

## PACKAGE INSTALLATIONS ##
# DNF Installations
if [ $pm_var == dnf ]
  then
    # Kernel headers(for linux device drivers)
    sudo dnf install kernel-headers kernel-devel

    # Packages
    sudo dnf -y install binutils diffutils @development-tools nasm valgrind

    # Install the dependencies for cross-compiler
    sudo dnf -y install gcc gcc-c++ make bison flex gmp-devel libmpc-devel mpfr-devel texinfo automake autoconf xorriso
fi

# APT Installations
if [ $pm_var == apt ]
  then
    # Kernel headers
    sudo apt -y install linux-headers-$(uname -r) # If does not work check "ls -l /usr/src/linux-headers-$(uname -r)"(if does not exist then there are no headers), insetad try to find the latest version if not installed

    # Packages
    sudo apt -y install build-essential nasm binutils diffutils valgrind

    # Dependencies
    sudo apt -y install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
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




## i386 CROSS COMPILER ##
# Create directories to store source and build directory
mkdir -p ~/src/cross-compiler/binutils2.30/build
mkdir -p ~/src/cross-compiler/gcc9.3.0/build

# Download and unpack source code
cd ~/src/cross-compiler/binutils2.30/
wget https://ftp.gnu.org/gnu/binutils/binutils-2.30.tar.gz
tar -xzf binutils-2.30.tar.gz
rm binutils-2.30.tar.gz

cd ~/src/cross-compiler/gcc9.3.0
wget https://ftp.gnu.org/gnu/gcc/gcc-9.3.0/gcc-9.3.0.tar.gz
tar -xzf gcc-9.3.0.tar.gz
rm gcc-9.3.0.tar.gz

# Get ready for the build
mkdir -p ~/opt/
export PREFIX="$HOME/opt/"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# Compilation instructions
if [ $pm_var != macos ]
  then
    # Building binutils
    cd $HOME/src/cross-compiler/binutils2.30/build
    ../binutils-2.30/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
    make install

    # Building GCC
    cd $HOME/src/cross-compiler/gcc9.3.0/build
    which -- $TARGET-as || echo $TARGET-as is not in the PATH
    ../gcc-9.3.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-language=c,c++ --without-headers
    make all-gcc
    make all-target-libgcc
    make install-gcc
    make install-target-libgcc
  else
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




# CHECK OF INSTALLTION
qemu-system-i386 --version
$TARGET-gcc --version
grub2-mkrescue --version
