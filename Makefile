obj-m := par.o
KERNEL_SOURCE := /lib/modules/2.6.38-13-generic-pae/build/
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(PWD) modules
 
 clean:
	$(MAKE) -C $(KERNEL_SOURCE) M=$(PWD) clean

