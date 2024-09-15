obj-m += a.o
KDIR:=/home/developer/EASY-EAI-kernel/kernel
PWD?=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order
