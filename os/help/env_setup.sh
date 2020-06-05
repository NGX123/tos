# Install the dependencies
sudo dnf -y install gcc gcc-c++ make bison flex gmp-devel libmpc-devel mpfr-devel texinfo
sudo dnf -y install binutils @development-tools

# Create directories to store source and build directory
mkdir -p ~/Special/src/cross-compiler/binutils2.30/build
mkdir -p ~/Special/src/cross-compiler/gcc9.3.0/build
mkdir -p ~/Special/src/grub


# Download and unpack source code  
cd ~/Special/src/cross-compiler/binutils2.30/
wget https://ftp.gnu.org/gnu/binutils/binutils-2.30.tar.gz
tar -xzf binutils-2.30.tar.gz
rm binutils-2.30.tar.gz

cd ~/Special/src/cross-compiler/gcc9.3.0
wget https://ftp.gnu.org/gnu/gcc/gcc-9.3.0/gcc-9.3.0.tar.gz
tar -xzf gcc-9.3.0.tar.gz
rm gcc-9.3.0.tar.gz

cd ~/Special/src/grub
git clone https://git.savannah.gnu.org/git/grub.git



# Get ready for the build
mkdir -p ~/opt/cross-compiler

export PREFIX="$HOME/opt/cross-compiler"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"


mkdir -p ~/opt/grub



# Building binutils
cd $HOME/Special/src/cross-compiler/binutils2.30/build
../binutils-2.30/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make 
make install



# Building GCC
cd $HOME/Special/src/cross-compiler/gcc9.3.0/build
which -- $TARGET-as || echo $TARGET-as is not in the PATH

../gcc-9.3.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-language=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc



# Building GRUB
cd ~/Special/src/grub
./bootstrap
./configure --prefix="$HOME/opt/grub" --target=i386 --with-platform=efi
make
make install



#Check
qemu-system-i386 --version
$HOME/opt/cross-compiler/bin/$TARGET-gcc --version
$HOME/opt/grub/bin/grub-mkrescue