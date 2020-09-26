CC = gcc
CFLAGS = -Wall -Wextra -c 
CCFLAGS_WARNINGS_EXTRA = -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused #Wsign-conversion

# Cross-compiler
CC_OS = $(HOME)/opt/cross-compiler/bin/i686-elf-gcc
CFLAGS_OS = -I libs/headers -std=gnu99 -ffreestanding -nostdlib -O2 -Wall -Wextra -c

# Compile all
all:
	make sh
	make cat

# Compile shell
sh:
	$(CC) $(CCFLAGS_WARNINGS_EXTRA) $(CFLAGS) sh.c

# Compiler cat
cat:
	$(CC) $(CCFLAGS_WARNINGS_EXTRA) $(CFLAGS) cat.c

# Cleans objects after the 
clean:
	rm -rf *.o *.out