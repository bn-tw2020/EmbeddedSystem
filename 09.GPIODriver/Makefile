obj-m	+= gpio_dev.o

# KDIR	:= /lib/modules/'uname -r'/build
KDIR	:= $(HOME)/pi/EmbeddedSystem/06.system_call/linux-rpi-5.10.y
CC	:= arm-linux-gnueabihf-gcc
OBJECT	:= gpio_app

default: 
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C$(KDIR) M=$(shell pwd) modules
	$(CC) -o $(OBJECT) $(OBJECT).c

test_app: test_app.c
	$(CROSS)gcc -o $@ $<

clean:
	make -C$(KDIR) M=$(shell pwd) clean
	rm $(OBJECT)
