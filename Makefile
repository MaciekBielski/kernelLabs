# TODO: config and build needed for that:
# KERNELDIR ?= /home/mba/Code/ubuntu-focal
KERNELDIR ?= /lib/modules/5.4.0-43-generic/build

obj-m += gismo/

all default: modules
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) $@

