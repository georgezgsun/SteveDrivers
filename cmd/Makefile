ifneq ($(KERNELRELEASE),)


#Since we have ov5640_mipi_v4.c at first, this is called v4 in order to distinguished with the old one
obj-m := isl79987_mipi_v4.o
isl79987_mipi_v4-objs =	\
	../src/drv/i2c/v4l2/sub/camera/isl79987/csi/fmt_video.o \
	../src/drv/i2c/v4l2/sub/camera/isl79987/csi/fmt.o \
	../src/drv/i2c/v4l2/sub/camera/isl79987/csi/hus.o \
	../src/drv/i2c/v4l2/sub/camera/isl79987/csi/input.o \
	../src/drv/i2c/v4l2/sub/camera/isl79987/csi/lane.o \
	../src/drv/i2c/v4l2/sub/camera/isl79987/csi/mod.o \
	../src/drv/i2c/v4l2/sub/camera/isl79987/csi/rate.o \
	../src/drv/i2c/v4l2/sub/camera/isl79987/csi/reg.o \
	../src/drv/i2c/v4l2/sub/camera/isl79987/csi/res.o \
	../src/drv/i2c/v4l2/sub/camera/isl79987/csi/decoder.o \
	../src/drv/i2c/v4l2/sub/camera/isl79987/isl79987.o \
	../src/drv/i2c/v4l2/sub/camera/camera.o \
	../src/drv/i2c/v4l2/sub/ops.o \
	../src/drv/i2c/v4l2/sub/dev.o \
	../src/drv/i2c/v4l2/register.o \
	../src/drv/i2c/v4l2/unregister.o \
	../src/drv/i2c/client/gpio.o \
	../src/drv/i2c/client/mclk.o \
	../src/drv/i2c/client/pinctl.o \
	../src/drv/i2c/client/regulator.o \
	../src/drv/i2c/client/data.o \
	../src/drv/i2c/i2c/reg_io.o \
	../src/drv/i2c/probe.o \
	../src/drv/i2c/remove.o \
	../src/drv/isl79987_mipi_v4.o
	
else

-include arm32.mk

KDIR:= /lib/modules/$(KERNEL_NAME)/build

CDIR:= $(shell pwd)

all:

	make -C $(KDIR) M=$(CDIR) modules -I$(KERNEL_INCLUDE)
	
clean:
	rm -rf .*.omd *.o *.mod.o *.ko *.c .tmp_versions Module.symvers .Makefile.swp modules.order

endif
