obj-m	+= test_dd.o

KDIR	:= /lib/modules/'uname -r'/build
KDIR	:= $(HOME)/pi/EmbeddedSystem/06.system_call/linux-rpi-5.10.y
CROSS	:= arm-linux-gnueabihf-

ALL: test_dd test_app

test_dd: test_dd.c
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C$(KDIR) M=$(shell pwd) modules
	# make ARCH=arm CROSS_COMPILE=$(CROSS) -C$(KDIR) SUBDIRS=$(PWD)

test_app: test_app.c
	$(CROSS)gcc -o $@ $<

clean:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C$(KDIR) M=$(sheel pwd) clean
	# make ARCH=arm CROSS_COMPILE=$(CROSS) -C$(KDIR) SUBDIRS=$(PWD) clean
	rm test_app