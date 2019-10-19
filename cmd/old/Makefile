ifneq ($(KERNELRELEASE),)

#This is for isl79987 old driver writen in 2016. 
#In order to use the old NXP'platform, 
#the driver needs API's provided by mxc_mipi_csi2.c(\drivers\mxc\mipi) to run at first.
obj-m := isl79987_mipi_old.o


#v4l2-int-device.h actually is located in drivers\media\platform\mxc\capture, 
#please copy it to \include\meadia
isl79987_mipi_old-objs =	\
	../../src/drv/old/isl79987_mipi_old.o

	
else

-include ../arm32.mk

KDIR:= /lib/modules/$(KERNEL_NAME)/build

CDIR:= $(shell pwd)

all:

	make -C $(KDIR) M=$(CDIR) modules -I$(KERNEL_INCLUDE)

clean:
	rm -rf .*.omd *.o *.mod.o *.ko *.c .tmp_versions Module.symvers .Makefile.swp modules.order

endif
