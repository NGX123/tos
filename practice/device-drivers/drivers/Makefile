obj-m += myDriver.o

all:
	make -C /lib/modules/4.19.0-8-amd64/build	M=$(PWD)	modules
clean:
	make -C /lib/modules/4.19.0-8-amd64/build	M=$(PWD)	clean

# change kernel version to $(shell uname -r)