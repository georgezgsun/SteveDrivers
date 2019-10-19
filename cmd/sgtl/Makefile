ifneq ($(KERNELRELEASE),)

#obj-m := reg_dev.o
#reg_dev-objs =../../src/drv/platform/device/device.o \
	../../src/drv/platform/device/module.o
obj-m := sgtl5000.o

#This for imx8's mipi-csi2 interface in SOC
#For more debug information, we need to change this driver
sgtl5000-objs =	\
	../../src/drv/sgtl/sgtl5000.o
	
	
else

-include ../arm32.mk

KDIR:= /lib/modules/$(KERNEL_NAME)/build

CDIR:= $(shell pwd)

all:

	make -C $(KDIR) M=$(CDIR) modules -I$(KERNEL_INCLUDE)

clean:
	rm -rf .*.omd *.o *.mod.o *.ko *.c .tmp_versions Module.symvers .Makefile.swp modules.order

endif
