ifneq ($(KERNELRELEASE),)


obj-m := mxc_mipi_csi2_v4.o  mxc_v4l2_capture_v4.o

mxc_mipi_csi2_v4-objs =	\
	../../src/drv/mxc/v4l2-int-device.o \
	../../src/drv/mxc/mxc_mipi_csi2_v4.o
	
#this driver is to use isl79987 configuration
ov5640_mipi_v4-objs =	\
	../../src/drv/ovt/ov5640_mipi_v4.o

mxc_v4l2_capture_v4-objs =	\
	../../src/drv/mxc/ipu_bg_overlay_sdc.o \
	../../src/drv/mxc/ipu_csi_enc.o \
	../../src/drv/mxc/ipu_fg_overlay_sdc.o \
	../../src/drv/mxc/ipu_prp_enc.o \
	../../src/drv/mxc/ipu_prp_vf_sdc.o \
	../../src/drv/mxc/ipu_prp_vf_sdc_bg.o \
	../../src/drv/mxc/ipu_still.o \
	../../src/drv/mxc/mxc_v4l2_capture_v4.o
	
else

-include ../arm32.mk

KDIR:= /lib/modules/$(KERNEL_NAME)/build

CDIR:= $(shell pwd)

all:

	make -C $(KDIR) M=$(CDIR) modules -I$(KERNEL_INCLUDE)

clean:
	rm -rf .*.omd *.o *.mod.o *.ko *.c .tmp_versions Module.symvers .Makefile.swp modules.order

endif