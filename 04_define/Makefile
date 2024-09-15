# KBUILD_CFLAGS_MODULE += -D宏
# KBUILD_CFLAGS_MODULE += -D宏=3 相当于C语言 #define DEBUG 3
KBUILD_CFLAGS_MODULE += -DDEBUG=3 
# Makefile也兼容EXTRA_CFLAGS += -DDEBUG
# EXTRA_CFLAGS += -DDEBUG

obj-m += define.o
# KDIR:=/home/developer/EASY-EAI-kernel/kernel
# 修改为ubuntu上的内核
KDIR:=/lib/modules/5.4.0-150-generic/build
PWD?=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers *.order
