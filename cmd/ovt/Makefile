ifneq ($(KERNELRELEASE),)


obj-m := ov5640_mipi_v4.o

#this driver is to use isl79987 configuration
ov5640_mipi_v4-objs =	\
	../../src/drv/ovt/ov5640_mipi_v4.o
	
else

-include ../arm32.mk

KDIR:= /lib/modules/$(KERNEL_NAME)/build

CDIR:= $(shell pwd)

all:

	make -C $(KDIR) M=$(CDIR) modules -I$(KERNEL_INCLUDE)

clean:
	rm -rf .*.omd *.o *.mod.o *.ko *.c .tmp_versions Module.symvers .Makefile.swp modules.order

endif