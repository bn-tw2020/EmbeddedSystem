obj-m	+= vrd_dd.o

KDIR	:= $(HOME)/pi/EmbeddedSystem/06.system_call/linux-rpi-5.10.y
PWD		:= $(shell pwd)
CROSS	:= arm-linux-gnueabihf-

default: 
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KDIR) M=$(PWD) modules

clean:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C$(KDIR) M=$(PWD) clean
	# make -C $(KDIR) M=$(shell pwd) clean
