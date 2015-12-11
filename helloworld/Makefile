obj-m += hello.o
KERNEL_INCLUDE := -I/usr/include/
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules


clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
