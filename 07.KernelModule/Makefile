obj-m	+= hello_m.o

KDIR	:= $(HOME)/pi/EmbeddedSystem/06.system_call/linux-rpi-5.10.y
CROSS	:= arm-linux-gnueabihf-

default:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C$(KDIR) M=$(shell pwd) modules
#	make ARCH=arm CROSS_COMPILE=$(CROSS) -C$(KDIR) SUBDIRS=$(PWD)

clean:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C$(KDIR) M=$(sheel pwd) clean
#	make ARCH=arm CROSS_COMPILE=$(CROSS) -C$(KDIR) SUBDIRS=$(PWD) clean
