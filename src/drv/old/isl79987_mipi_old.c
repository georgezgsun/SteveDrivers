/*
 * Copyright (c) 2015-2017 Intersil Corporation. All Rights Reserved.
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*!
 * @file isl79987_mipi.c
 *
 * @brief Intersil isl79987 4-ch decoder for mipi-csi2.
 *
 * @ingroup TVIN
 *
 *
 * CONFIG_MXC_TVIN_ISL79987_MIPI for mipi-csi2.
 */
/* 
* Change History
* ==============
* 170411     BrianKang      use dev_inited_counter.
* 170408 1.4 BrianKang		add use_counter.
* 170203 1.3 BrianKang		add Default
* 170117 1.2 BrianKang		use FrameMode. Assign some default to co-work with mxc_v4l2_tvin.out
* 161109 1.1 BrianKang      use TC value. Add raw_data[112].
*                           use CONFIG_MXC_IPU_4CH_CAPTURE
* 160819 1.0 BrianKang
* 160407 0.2 BrianKang      for isl79987
* 151006 0.1 BrianKang      add querystd
* 150914 0.0 BrianKang		Kernel 3.10.53
*/

/* 9987 raw_data format-----------------------
    raw_data[100..1003]="9997"
	raw_data[104] format version
	raw_data[105] 0:normal, 1:dummy, 2:reader
	raw_data[106] max_sensor. default is 4.
	raw_data[107] mipi data lanes. default is 2
	raw_data[108] pseudo 0:standard, 1,2 or 4.
	raw_data[109] 1:histogram data on pseudo
	raw_data[110] bclock high byte
	raw_data[111] bclock low byte
	raw_data[112] VC mode on pseudo
		R506[3]
		R504[7:0]
					0
					1,2,3,4 
-------------------------------------------
*/

 
#define DEBUG
#define CONFIG_PATCH_INTERSIL

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/of_device.h>
#include <linux/i2c.h>
#include <linux/of_gpio.h>
#include <linux/pinctrl/consumer.h>
#include <linux/regulator/consumer.h>
#include <linux/fsl_devices.h>
#include <linux/mipi_csi2.h>
#include <media/v4l2-chip-ident.h>
//#include <media/v4l2-int-device.h>
#include "../mxc/v4l2-int-device.h"
#include "../mxc/mxc_v4l2_capture.h"
#include "../mxc/mxc_mipi_csi2.h"

#define  os_print(fmt,...)  printk(KERN_ERR fmt,##__VA_ARGS__)
/*!
 * Maintains the information on the current state of the sesnor.
 */
#define SENSOR_NUM 4
int dev_inited_counter;
static struct sensor_data isl79987_data[SENSOR_NUM];


/*!
* ISL79987 I2C ID (7bit) 
*/
#define ISL79987_I2C_ID        0x44

/*!
* decoder registers 
*/
#define isl79987_IA_BRIGHTNESS_Y      0x10  /* Brightness */
#define isl79987_IA_CONTRAST_Y        0x11  /* Contrast */
#define isl79987_IA_SHARPNESS         0x12  /* Sharpness */
#define isl79987_IA_CONTRAST_CB       0x13  /* Saturation Cb */
#define isl79987_IA_CONTRAST_CR       0x14  /* Saturation Cr */
#define isl79987_IA_HUE               0x15  /* Hue */
#define isl79987_DEC_STANDARD         0x1C  /* DEC STD select */


/*!
 *  isl79987 initialization register structure
 *  1st value = Register Address
 *  2nd value = Register Value
 */
struct reg_value {
    u8 reg;
    u8 value;
};

/*!
 * default isl79987 register table.
 *
 * current target is 4ch 2 mipi lanes.
 *
 * describe important registers.
 * R501[5] 1:FrameMode
*/


/* default is 4Ch2Lane */
static struct reg_value isl79987_default[]={
{0xFF,0x00},{0x03,0x00},{0x0D,0xC9},{0x0E,0xC9}, 
            {0x10,0x01},{0x11,0x03},{0x12,0x00},{0x13,0x00},{0x14,0x00}, 
{0xFF,0x05},{0x00,0x02},{0x01,0x85},{0x02,0xa0},{0x03,0x18},
            {0x04,0xE4},{0x05,0x40},{0x06,0x40}, 
            {0x10,0x05},{0x11,0xa0},
            {0x20,0x00},{0x21,0x0C},{0x22,0x00},{0x23,0x00}, 
            {0x24,0x00},{0x25,0xF0},{0x26,0x00},{0x27,0x00},
            {0x2A,0x00},{0x2B,0x19},
            {0x2C,0x18},{0x2D,0xF1},{0x2E,0x00},{0x2F,0xF1},
            {0x30,0x00},{0x31,0x00},{0x32,0x00},{0x33,0xC0},
            {0x34,0x18},{0x35,0x00},{0x36,0x00}
};

static struct reg_value isl79987_decoder[]={
    {0xff,0x0f},{0x08,0x14},{0x2F,0xE6},{0x33,0x85},{0x3d,0x08},{0xE7,0x00}
};

static struct reg_value isl79987_4Ch2Lane[]={
{0xFF,0x00},{0x07,0x12},{0x08,0x1f},{0x09,0x43},{0x0A,0x4f},{0x0B,0x41},
{0xFF,0x05},{0x00,0x02},{0x01,0x25},{0x02,0xA0},{0x03,0x10},
            {0x04,0xE4},{0x05,0x00},{0x06,0x00},{0x07,0x2B},
            {0x08,0x02},{0x09,0x00},{0x0A,0x62},{0x0B,0x02},
            {0x0C,0x36},{0x0D,0x00},{0x0E,0x6C},{0x0F,0x00},
            {0x10,0x05},{0x11,0xA0},{0x12,0x77},{0x13,0x17},
            {0x14,0x08},{0x15,0x38},{0x16,0x14},{0x17,0xF6},
            {0x18,0x00},{0x19,0x17},{0x1A,0x0A},{0x1B,0x71},
            {0x1C,0x7A},{0x1D,0x0F},{0x1E,0x8C},
            {0x23,0x0A},{0x26,0x08},
            {0x28,0x01},{0x29,0x0E},{0x2A,0x00},{0x2B,0x00},
            {0x38,0x03},{0x39,0xC0},{0x3A,0x06},{0x3B,0xB3},
            {0x3C,0x00},{0x3D,0xF1},    
};

static struct reg_value isl79987_4Ch1Lane[]={
{0xFF, 0x0},{0x07,0x12},{0x08,0x1f},{0x09,0x43},{0x0A,0x4f},{0x0B,0x40},
{0xFF, 0x5},{0x00,0x01},{0x01,0x25},{0x02,0xA0},{0x03,0x10},
            {0x04,0xE4},{0x05,0x00},{0x06,0x00},{0x07,0x2B},
            {0x08,0x00},{0x09,0x00},{0x0A,0x62},{0x0B,0x02},
            {0x0C,0x36},{0x0D,0x00},{0x0E,0x6C},{0x0F,0x00},
            {0x10,0x05},{0x11,0xA0},{0x12,0x78},{0x13,0x17},
            {0x14,0x07},{0x15,0x36},{0x16,0x10},{0x17,0xF6},
            {0x18,0x00},{0x19,0x17},{0x1A,0x0A},{0x1B,0x71},
            {0x1C,0x7A},{0x1D,0x0F},{0x1E,0x8C},
            {0x23,0x0A},{0x26,0x07},
            {0x28,0x01},{0x29,0x0E},{0x2A,0x00},{0x2B,0x00},
            {0x38,0x03},{0x39,0xC0},{0x3A,0x06},{0x3B,0xB3},
            {0x3C,0x00},{0x3D,0xF1}
};
static struct reg_value isl79987_2Ch2Lane[]={
{0xFF, 0x0},{0x07,0x11},{0x08,0x1f},{0x09,0x47},{0x0A,0x4f},{0x0B,0x42},
{0xFF, 0x5},{0x00,0x02},{0x01,0x25},{0x02,0xA0},{0x03,0x10},
            {0x04,0xE4},{0x05,0x00},{0x06,0x00},{0x07,0x24},
            {0x08,0x02},{0x09,0x00},{0x0A,0x62},{0x0B,0x02},
            {0x0C,0x36},{0x0D,0x00},{0x0E,0x36},{0x0F,0x00},
            {0x10,0x05},{0x11,0xA0},{0x12,0x34},{0x13,0x0F},
            {0x14,0x06},{0x15,0x24},{0x16,0x11},{0x17,0x70},
            {0x18,0x00},{0x19,0x17},{0x1A,0x06},{0x1B,0x31},
            {0x1C,0x46},{0x1D,0x08},{0x1E,0x57},
            {0x23,0x06},{0x26,0x06},
            {0x28,0x01},{0x29,0x69},{0x2A,0x00},{0x2B,0x00},
            {0x38,0x01},{0x39,0xE0},{0x3A,0x06},{0x3B,0xB3},
            {0x3C,0x00},{0x3D,0xF1}
};
static struct reg_value isl79987_2Ch1Lane[]={
{0xFF, 0x0},{0x07,0x11},{0x08,0x1f},{0x09,0x47},{0x0A,0x4f},{0x0B,0x41},
{0xFF, 0x5},{0x00,0x01},{0x01,0x25},{0x02,0xA0},{0x03,0x10},
            {0x04,0xE4},{0x05,0x00},{0x06,0x00},{0x07,0x1B},
            {0x08,0x02},{0x09,0x00},{0x0A,0x62},{0x0B,0x02},
            {0x0C,0x36},{0x0D,0x00},{0x0E,0x36},{0x0F,0x00},
            {0x10,0x05},{0x11,0xA0},{0x12,0x34},{0x13,0x07},
            {0x14,0x02},{0x15,0x1E},{0x16,0x0A},{0x17,0x70},
            {0x18,0x00},{0x19,0x17},{0x1A,0x06},{0x1B,0x31},
            {0x1C,0x43},{0x1D,0x08},{0x1E,0x77},
            {0x23,0x03},{0x26,0x02},
            {0x28,0x00},{0x29,0xB4},{0x2A,0x00},{0x2B,0x00},
            {0x38,0x01},{0x39,0xE0},{0x3A,0x06},{0x3B,0xB3},
            {0x3C,0x00},{0x3D,0xF1}
};
static struct reg_value isl79987_1Ch1Lane[]={
{0xFF, 0x0},{0x07,0x00}, {0x08,0x1f},{0x09,0x4f},{0x0A,0x4f},{0x0B,0x42},
{0xFF, 0x5},{0x00,0x01},{0x01,0x25},{0x02,0xA0},{0x03,0x10},
            {0x04,0xE4},{0x05,0x00},{0x06,0x00},{0x07,0x17},
            {0x08,0x00},{0x09,0x00},{0x0A,0x62},{0x0B,0x02},
            {0x0C,0x36},{0x0D,0x00},{0x0E,0x1B},{0x0F,0x00},
            {0x10,0x05},{0x11,0xA0},{0x12,0x12},{0x13,0x05},
            {0x14,0x02},{0x15,0x0E},{0x16,0x08},{0x17,0x37},
            {0x18,0x00},{0x19,0x00},{0x1A,0x02},{0x1B,0x11},
            {0x1C,0x22},{0x1D,0x03},{0x1E,0x22},
            {0x23,0x02},{0x26,0x02},
            {0x28,0x01},{0x29,0x0E},{0x2A,0x00},{0x2B,0x00},
            {0x38,0x00},{0x39,0xF0},{0x3A,0x06},{0x3B,0xB3},
            {0x3C,0x00},{0x3D,0xF1}
};

struct reg_value isl79987_Decoder_NTSC[] = {
    {0x07,0x02},{0x08,0x12},{0x09,0xf0},{0x0a,0x0F},{0x0b,0xd0}
};
struct reg_value isl79987_Decoder_PAL[] = {
    {0x07,0x12},{0x08,0x18},{0x09,0x20},{0x0a,0x08},{0x0b,0xd0}
};  


/*
 * define prototype.
 */

static int ioctl_dev_init(struct v4l2_int_device *s);
static int ioctl_g_std(struct v4l2_int_device *s, v4l2_std_id *std);
static int ioctl_querystd(struct v4l2_int_device *s, v4l2_std_id *std);

/*! Read one register from a ISL79987 i2c slave device.
 *
 *  @param *reg		register in the device we wish to access.
 *
 *  @return		       0 if success, an error code otherwise.
 *
 *  	int val;

	val = i2c_smbus_read_byte_data(isl79987_data[0].i2c_client, reg);
	if (val < 0) {
		dev_info(&isl79987_data[0].i2c_client->dev,
			"%s:read reg error: reg=%2x\n", __func__, reg);
		return -1;
	}
	return val;
 */
static int ReadTW88(u8 reg)
{
	int ret;
	u8 au8RegBuf[2] = {0};
	u8 u8RdVal = 0;

	au8RegBuf[0] = reg ;

	if (i2c_master_send(isl79987_data[0].i2c_client, au8RegBuf, 1) != 1) {
		os_print("I2C:Read reg send error: reg=%x\n", reg);
		return -1;
	}

	if (i2c_master_recv(isl79987_data[0].i2c_client, &u8RdVal, 1) != 1) {
		os_print("I2C:Read reg recv error: reg=%x, val=%x\n", reg, u8RdVal);
		return -1;
	}

	ret =u8RdVal;
	ret &= 0xff;
	return ret;
}

/*! Write one register of a ISL79987 i2c slave device.
 *
 *  @param *reg		register in the device we wish to access.
 *
 *  @return		       0 if success, an error code otherwise.
 *
 *
 *  	s32 ret;

	ret = i2c_smbus_write_byte_data(isl79987_data[0].i2c_client, reg, val);
	if (ret < 0) {
		dev_info(&isl79987_data[0].i2c_client->dev,
			"%s:write reg error:reg=%2x,val=%2x\n", __func__,
			reg, val);
		return -1;
	}
	return 0;
 */
static int WriteTW88(u8 reg, u8 val)
{
	u8 au8Buf[2] = {0};

	au8Buf[0] = reg;
	au8Buf[1] = val;

	if (i2c_master_send(isl79987_data[0].i2c_client, au8Buf, 2) != 2) {
//		os_print("I2C:Write reg error: reg=%x, val=%x\n", reg, val);
		return -1;
	}
	return 0;
}

/*!
 * write arrary register table
 */
static int WriteTW88S(struct reg_value *config, int size)
{
    int i;
    for(i=0; i < size; i++)
        WriteTW88(config[i].reg, config[i].value);
    return 0;
}

/*!
 * reset MIPI
 */
void ISL79987_Reset(int flag_set) 
{
    WriteTW88(0xff, 0); 
    if(flag_set==1)
        WriteTW88(0x02, 0x1F);  /* set Reset */
    else if(flag_set==2)
        WriteTW88(0x02, 0x10); /* clear Reset_CH1 ~ Reset_CH4 */            
    else
        WriteTW88(0x02, 0x00); /* clear reset */
}



/*! List of input video formats supported. 
 * The video formats is corresponding with v4l2 id in video_fmt_t
 */
typedef enum {
    isl79987_NTSC = 0,      /*!< Locked on (M) NTSC video signal. */
    isl79987_PAL,           /*!< (B, G, H, I, N)PAL video signal. */
    isl79987_NOT_LOCKED,    /*!< Not locked on a signal. */
} video_fmt_idx;
/*! Number of video standards supported (including 'not locked' signal). */
#define isl79987_STD_MAX      (isl79987_PAL + 1)

/*! Video format structure. */
typedef struct {
    int v4l2_id;        /*!< Video for linux ID. */
    char name[16];      /*!< Name (e.g., "NTSC", "PAL", etc.) */
    u16 raw_width;      /*!< Raw width. (total width) */
    u16 raw_height;     /*!< Raw height. (total width) */
    u16 active_width;   /*!< Active width. */
    u16 active_height;  /*!< Active height. */
	int frame_rate;		/*!< Frame rate. */
} video_fmt_t;

/*! Description of video formats supported.
 *
 *  PAL:  raw=720x625, active=720x576.
 *  NTSC: raw=720x525, active=720x480.
 */
static video_fmt_t video_fmts[] = {
    {   /*! NTSC */
     .v4l2_id = V4L2_STD_NTSC,   .name = "NTSC",
     .raw_width = 720, .raw_height = 525,       
     .active_width = 720, .active_height = 480, 
   	 .frame_rate = 30, 
     },
    {   /*! (B, G, H, I, N) PAL */
     .v4l2_id = V4L2_STD_PAL,    .name = "PAL",
     .raw_width = 720, .raw_height = 625,
     .active_width = 720, .active_height = 576,
   	 .frame_rate = 25, 
     },
    {   /*! Unlocked standard */
     .v4l2_id = V4L2_STD_ALL,    .name = "Unlocked",
     .raw_width = 720, .raw_height = 525,
     .active_width = 720,   .active_height = 480,
   	 .frame_rate = 30, 
     },
};


/*! @brief This mutex is used to provide mutual exclusion.
 *
 *  Create a mutex that can be used to provide mutually exclusive
 *  read/write access to the globally accessible data structures
 *  and variables that were defined above.
 */
static DEFINE_MUTEX(mutex);

/***********************************************************************
 * mode info. only for decoder.
 ***********************************************************************/
typedef enum {
    INPUT_CVBS1 = 0,
    INPUT_CVBS2, 
    INPUT_CVBS3,
    INPUT_CVBS4, 
} ISL79987_INPUT_TYPE;

#define ISL79987_INPUT_MAX 4
static struct v4l2_input isl79987_inputs[] = {
    { .index=0,  .name="CVBS1", 
      .type = V4L2_INPUT_TYPE_CAMERA, .std = V4L2_STD_ALL, 
      .capabilities = V4L2_IN_CAP_STD, .status= V4L2_IN_ST_NO_H_LOCK,},
    { .index=1,  .name="CVBS2",
      .type = V4L2_INPUT_TYPE_CAMERA, .std = V4L2_STD_ALL, 
      .capabilities = V4L2_IN_CAP_STD, .status= V4L2_IN_ST_NO_H_LOCK,},
    { .index=2,  .name="CVBS3",
      .type = V4L2_INPUT_TYPE_CAMERA, .std = V4L2_STD_ALL, 
      .capabilities = V4L2_IN_CAP_STD, .status= V4L2_IN_ST_NO_H_LOCK,},
    { .index=3,  .name="CVBS4",
      .type = V4L2_INPUT_TYPE_CAMERA, .std = V4L2_STD_ALL, 
      .capabilities = V4L2_IN_CAP_STD, .status= V4L2_IN_ST_NO_H_LOCK,},
};


/*! 
 * check mipi sensor & status
 */
static int mxc_wait_mipi_csi2_ready(u32 msec_wait4stable)
{
    void *mipi_csi2_info; 
    u32 mipi_reg;
    unsigned int i;

    mipi_csi2_info = mipi_csi2_get_info();
    if(mipi_csi2_info == 0) {
        pr_err("%s: Fail to get mipi_csi2_info!\n", __func__);
        return -1;
    }

    msleep(msec_wait4stable);
    
    /* wait for mipi sensor ready */
    /* wait until the clock lane is not an untra low power state */
    i = 0;
    while(1) {
        mipi_reg = mipi2_csi2_dphy_status(mipi_csi2_info);
#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
        if((mipi_reg & 0x0700) != 0x0200)
#else
        if(mipi_reg != 0x200)
#endif
            break;
        i++;
        if(i == 100) {
            pr_err("mipi csi2 can not receive sensor clk!\n");
            return -1;
        }
        msleep(10);
    }
    /* wait for mipi stable */
    i = 0;
    while(1) {
        mipi_reg = mipi2_csi2_get_error1(mipi_csi2_info);
        if(mipi_reg == 0)
            break;
        i++;
        if(i == 10) {
            pr_err("mipi csi2 can not receive data correctly!\n");
            return -1;
        }
        msleep(10);
    }
    return 0;
}

/*! 
 * update sensor data
 * caller ioctl_g_std, ioctl_querystd
 */
static int isl79987_update_sensor_data(struct v4l2_int_device *s, v4l2_std_id *std)
{
    struct sensor_data *sensor = s->priv;
    int ret;

    if(std==V4L2_STD_UNKNOWN)
        return -EINVAL;

    sensor->std_id = *std;

    if(*std == V4L2_STD_PAL) {
        ISL79987_Reset(1);
        WriteTW88(0xFF,sensor->v_channel+1);
        WriteTW88S(isl79987_Decoder_PAL, ARRAY_SIZE(isl79987_Decoder_PAL));
        WriteTW88(0xFF, 0x05);
        WriteTW88(0x0F, ReadTW88(0x0F) |  0x04);            
        WriteTW88(0x3C, 0x01);            
        WriteTW88(0x3D, 0x21);
        ISL79987_Reset(0);

        sensor->pix.width = 720;
        sensor->pix.height = 576;
        sensor->streamcap.timeperframe.denominator = 25;
        sensor->pix.sizeimage = 720 * 2 * 576;
    }
    else {
        ISL79987_Reset(1);
        WriteTW88(0xFF,sensor->v_channel+1);
        WriteTW88S(isl79987_Decoder_NTSC, ARRAY_SIZE(isl79987_Decoder_NTSC));
        WriteTW88(0xFF, 0x05);
        WriteTW88(0x0F, ReadTW88(0x0F) & ~0x04);
        WriteTW88(0x3C, 0x00);            
        WriteTW88(0x3D, 0xF1);
        ISL79987_Reset(0);

        sensor->pix.width = 720;
        sensor->pix.height = 480;
        sensor->streamcap.timeperframe.denominator = 30;
        sensor->pix.sizeimage = 720 * 2 * 480;  
    }
    
    ret = mxc_wait_mipi_csi2_ready(100);
    if(ret)
        pr_err("%s failed check_csi2_rx_mipi_dphy\n",__func__);

    return ret;
}

//===========================================================


/*
extern parameters:
    sensor->is_dummy
    sensor->max_sensor  mipi_chs
    sensor->lanes       mipi_lanes. if we change, 
            we need mipi_csi2_set_lanes_by_num(), mipi_csi2_reset().
    sensor->pseudo  0,1,2,4 PseudoFrame mode.
    sensor->histo       enable histogram data line when it is PseudoFrame mode
    sensor->nvc         virtual channel on PseudoFrame mode
*/
int isl79987_init_chip_registers(struct sensor_data *sensor)
{
	int lines;

    if(sensor==NULL) {
        pr_info("download isl79987 default regs.");
        WriteTW88(0xff, 5);         /* clear PowerDown */
        WriteTW88(0x00, ReadTW88(0x00) & ~0x80);         
        ISL79987_Reset(1);          /* set Reset all */                      
        WriteTW88S(isl79987_default, ARRAY_SIZE(isl79987_default));        
        ISL79987_Reset(2);          /* clear Reset_CH1 ~ Reset_CH4 */        
        WriteTW88S(isl79987_decoder, ARRAY_SIZE(isl79987_decoder));
        WriteTW88S(isl79987_4Ch2Lane, ARRAY_SIZE(isl79987_4Ch2Lane));
        ISL79987_Reset(0);          /* clear Reset */ 
        return 0;
    }
    if(sensor->is_dummy)
        return 1;

    ISL79987_Reset(1);          /* set Reset all */
                  
    WriteTW88S(isl79987_default, ARRAY_SIZE(isl79987_default));
    
    ISL79987_Reset(2);          /* clear Reset_CH1 ~ Reset_CH4 */
    
    WriteTW88S(isl79987_decoder, ARRAY_SIZE(isl79987_decoder));
    if(sensor->max_sensor==1) {
        WriteTW88S(isl79987_1Ch1Lane, ARRAY_SIZE(isl79987_1Ch1Lane));
    }
    else if(sensor->max_sensor==2) {
        if(sensor->lanes==1)
            WriteTW88S(isl79987_2Ch1Lane, ARRAY_SIZE(isl79987_2Ch1Lane));
        else
            WriteTW88S(isl79987_2Ch2Lane, ARRAY_SIZE(isl79987_2Ch2Lane));
    }
    else {
        if(sensor->lanes==1)
            WriteTW88S(isl79987_4Ch1Lane, ARRAY_SIZE(isl79987_4Ch1Lane));
        else
            WriteTW88S(isl79987_4Ch2Lane, ARRAY_SIZE(isl79987_4Ch2Lane));
    }    
                                
    if(sensor->pseudo) {
		/* first, remove FrameMode; REG501[5] */
        WriteTW88(0xFF, 5);
		WriteTW88(0x01, ReadTW88(0x01) & ~0x20);

        lines = sensor->pseudo;
        if(sensor->std_id==V4L2_STD_PAL)    lines *= 288;
        else                                lines *= 240;
        if(sensor->histo) { /* ?1 */
            lines += sensor->pseudo;
            WriteTW88(0xff, 0xf);
            WriteTW88(0xE7, 0x01);
        }

        WriteTW88(0xFF, 5);
        WriteTW88(0x06, 0x61);
        switch(sensor->nvc) {
        case 0:
            WriteTW88(0x04, 0x00); /* or use REG506[3]=1 */
            break;
        case 1:
            WriteTW88(0x04, 0x55);
            break;
        case 2:
            WriteTW88(0x04, 0xAA);
            break;
        case 3:
            WriteTW88(0x04, 0xFF);
            break;
        default:
            WriteTW88(0x04, 0x00);
            break;
        }

        WriteTW88(0x38, lines >> 8);
        WriteTW88(0x39, lines);        
    }
    
    
    if (sensor->pix.pixelformat == V4L2_PIX_FMT_RGB565) {
        WriteTW88(0xFF, sensor->v_channel+1);
        WriteTW88(0x3D, ReadTW88(0x3D) |  0x01);
        WriteTW88(0xFF, 0x05);
        WriteTW88(0x05, ReadTW88(0x05) |  0x10);  
    }        
    else {
        WriteTW88(0xFF, sensor->v_channel+1);
        WriteTW88(0x3D, ReadTW88(0x3D) & ~0x01);
        WriteTW88(0xFF, 0x05);
        WriteTW88(0x05, ReadTW88(0x05) & ~0x10);  
    }    
    
    ISL79987_Reset(0);  /* clear Reset */        

    return 0;
}

/*
* extern:
*   sensor->lanes
*   sensor->bclock
*   sensor->max_sensor
*/
static int mxc_init_mipi_csi2(struct sensor_data *sensor)
{
	int retval = 0;
	void *mipi_csi2_info;
	int i, lanes;
	unsigned int datatype;

#if defined(CONFIG_MXC_IPU_4CH_CAPTURE) 
    pr_err("%s %d counter:%d\n",__func__,  sensor->input_source,dev_inited_counter); 
#else
    pr_err("%s %d\n",__func__,  sensor->input_source);                    
#endif    
    
    
	/* initial mipi dphy */
	mipi_csi2_info = mipi_csi2_get_info();
	if (!mipi_csi2_info) {
		pr_err("%s: Fail to get mipi_csi2_info!\n", __func__);
		return -1;
	}

	if (!mipi2_csi2_get_status(mipi_csi2_info))
		mipi2_csi2_enable(mipi_csi2_info);

	if (!mipi2_csi2_get_status(mipi_csi2_info)) {
		pr_err("%s: Can not enable mipi csi2 driver!\n",__func__);
		return -1;
	}

    lanes = mipi2_csi2_set_lanes(mipi_csi2_info,sensor->lanes);
    lanes += 1;
    if(lanes != sensor->lanes) {
        pr_err("ISL79987 doesn't support lanes = %d.\n", sensor->lanes);
		return -1;
    }

	/* Only reset MIPI CSI2 HW at sensor initialize */
	/* 13.5MHz pixel clock (720*480@30fps) * 16 bits per pixel (YUV422) 
	   = 216Mbps mipi data rate for each camera */
	mipi2_csi2_reset(mipi_csi2_info);

    if(sensor->pix.pixelformat == V4L2_PIX_FMT_RGB565)
        datatype = MIPI_DT_RGB565;
    else
        datatype = MIPI_DT_YUV422;

#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
	for (i=0; i<SENSOR_NUM; i++)
		mipi2_csi2_set_datatype(mipi_csi2_info, i, datatype);
#else
	mipi2_csi2_set_datatype(mipi_csi2_info, datatype);
#endif

    retval = mxc_wait_mipi_csi2_ready(10);

	return retval;
}



/*--------------------------------------------------------------------------*/
/*---------------- IOCTL functions from v4l2_int_ioctl_desc ---------------_*/
/*--------------------------------------------------------------------------*/


/*!
 * ioctl_enum_fmt_cap - V4L2 sensor interface handler for VIDIOC_ENUM_FMT
 * @s: pointer to standard V4L2 device structure
 * @fmt: pointer to standard V4L2 fmt description structure
 *
 * Return 0.
 */
static int ioctl_enum_fmt_cap(struct v4l2_int_device *s,
			      struct v4l2_fmtdesc *fmt)
{
	//struct sensor_data *sensor = s->priv;

    os_print("ISL:ops %s.\n",__func__);

	if (fmt->index > 1) /* only 1 pixelformat support so far */
		return -EINVAL;

    if(fmt->index==1) {
        fmt->pixelformat = V4L2_PIX_FMT_RGB565;
        strcpy(fmt->description, "RGBP");
    }
    else {
        fmt->pixelformat = V4L2_PIX_FMT_UYVY;
        strcpy(fmt->description, "UYUV");
    }

	return 0;
}


/*!
 * ioctl_g_fmt_cap - V4L2 sensor interface handler for ioctl_g_fmt_cap
 * @s: pointer to standard V4L2 device structure
 * @f: pointer to standard V4L2 v4l2_format structure
 *
 * Returns the sensor's current pixel format in the v4l2_format
 * parameter.
 */
static int ioctl_g_fmt_cap(struct v4l2_int_device *s, struct v4l2_format *f)
{
	struct sensor_data *sensor = s->priv;
    os_print("ISL:ops %s.\n",__func__);
	switch(f->type) {
    case V4L2_BUF_TYPE_VIDEO_CAPTURE:
        f->fmt.pix = sensor->pix;
        break;

	/* Freescale Buggy Code.*/
	case V4L2_BUF_TYPE_PRIVATE: {
		v4l2_std_id std;
		ioctl_g_std(s, &std);
		f->fmt.pix.pixelformat = (u32)std;
		}
		break;
		
	default:
	    pr_err("ioctl_g_fmt_cap: unknown f-type:%x", f->type);
		return -1;
	}
    return 0;
}

static int ioctl_s_fmt_cap(struct v4l2_int_device *s, struct v4l2_format *f)
{
    struct sensor_data *sensor = s->priv;
    void *mipi_csi2_info;
#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
    int i;
#endif
    int ret;
    os_print("ISL:ops %s.\n",__func__);
    if(f->type != V4L2_BUF_TYPE_VIDEO_CAPTURE)
        return -EINVAL;

    if(f->fmt.pix.field == V4L2_FIELD_ANY)
        f->fmt.pix.field = V4L2_FIELD_INTERLACED_TB;
    
	if(sensor->pix.width != f->fmt.pix.width)
		pr_err("%s width %x->%x\n", __func__, 
		    sensor->pix.width, f->fmt.pix.width);
	if(sensor->pix.height != f->fmt.pix.height)
		pr_err("%s height %x->%x\n", __func__, 
		    sensor->pix.height, f->fmt.pix.height);
	if(sensor->pix.pixelformat != f->fmt.pix.pixelformat)
		pr_err("%s pixelformat %x->%x\n", __func__, 
		    sensor->pix.pixelformat, f->fmt.pix.pixelformat);
	if(sensor->pix.field != f->fmt.pix.field)
		pr_err("%s field %x->%x\n", __func__, 
		    sensor->pix.field, f->fmt.pix.field);

	if(sensor->pix.pixelformat != f->fmt.pix.pixelformat) {
		pr_err("%s pixelformat %x->%x\n", __func__, 
		    sensor->pix.pixelformat, f->fmt.pix.pixelformat);
        mipi_csi2_info = mipi_csi2_get_info();
        if(mipi_csi2_info==0) {
            pr_err("%s mipi_csi2_info fail\n",__func__);
            return -1;
        }
		ISL79987_Reset(1);  /* set Reset all */
        if (sensor->pix.pixelformat == V4L2_PIX_FMT_UYVY) {
            WriteTW88(0xFF, sensor->v_channel+1);
            WriteTW88(0x3D, ReadTW88(0x3D) & ~0x01);
            WriteTW88(0xFF, 0x05);
            WriteTW88(0x05, ReadTW88(0x05) & ~0x10);  
#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
            for(i=0; i < 4; i++)
                mipi2_csi2_set_datatype(mipi_csi2_info, i, MIPI_DT_YUV422);
#else
            mipi2_csi2_set_datatype(mipi_csi2_info, MIPI_DT_YUV422);
#endif
        }            
        else if(sensor->pix.pixelformat == V4L2_PIX_FMT_RGB565) {
            WriteTW88(0xFF, sensor->v_channel+1);
            WriteTW88(0x3D, ReadTW88(0x3D) |  0x01);
            WriteTW88(0xFF, 0x05);
            WriteTW88(0x05, ReadTW88(0x05) |  0x10);            
#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
            for(i=0; i < 4; i++)
                mipi2_csi2_set_datatype(mipi_csi2_info, i, MIPI_DT_RGB565);
#else
            mipi2_csi2_set_datatype(mipi_csi2_info, MIPI_DT_RGB565);
#endif
        }
	    else {
		    pr_err("currently this sensor format can not be supported!\n");
		}
		ISL79987_Reset(0);  /* clear Reset */ 

        /*check mipi sensor & status*/
        ret = mxc_wait_mipi_csi2_ready(10);
        if(ret)
            pr_err("%s failed mxc_wait_mipi_csi2_ready\n",__func__);
	
    }		

    sensor->pix = f->fmt.pix;
    
    return 0;
}

static int ioctl_try_fmt_cap(struct v4l2_int_device *s, struct v4l2_format *f)
{
	struct sensor_data *sensor = s->priv;

    os_print("ISL:ops %s.\n",__func__);
    if(f->fmt.pix.field == V4L2_FIELD_ANY) {
        pr_err("%s update field ANY->_TB\n",__func__);
        f->fmt.pix.field = V4L2_FIELD_INTERLACED_TB;
    }
    if(f->fmt.pix.field != sensor->pix.field) {
        pr_err("%s update field %d->%d\n",__func__,
            sensor->pix.field,f->fmt.pix.field);
        sensor->pix.field = f->fmt.pix.field;                
    }
            
    if(f->fmt.pix.pixelformat != sensor->pix.pixelformat) {
        pr_err("%s update pixelformat %x->%x\n",__func__,
            sensor->pix.pixelformat,f->fmt.pix.pixelformat);
        sensor->pix.pixelformat = f->fmt.pix.pixelformat;                
    }
    if(sensor->pix.width != f->fmt.pix.width) {
        pr_err("%s update width %d->%d\n",__func__,
            sensor->pix.width,f->fmt.pix.width);
        sensor->pix.width = f->fmt.pix.width;
    }
    if(sensor->pix.height != f->fmt.pix.height) {
        pr_err("%s update height %d->%d\n",__func__,
            sensor->pix.height,f->fmt.pix.height);
        sensor->pix.height = f->fmt.pix.height;
    }

    pr_err("%s->ioctl_dev_init()\n",__func__);
	ioctl_dev_init(s);
	return 0;
}

/* supported controls */
/* This hasn't been fully implemented yet.
 * This is how it should work, though. 
 * REG28A
 */

static struct v4l2_queryctrl isl79987_qctrl[] = {
    {
    .id = V4L2_CID_BRIGHTNESS, .type = V4L2_CTRL_TYPE_INTEGER, 
    .name = "Brightness",
    .minimum = -128, .maximum = 127, .step = 1, .default_value = 0x00,  
    .flags = 0,
    }, {
    .id = V4L2_CID_CONTRAST, .type = V4L2_CTRL_TYPE_INTEGER, 
    .name = "Contrast",
    .minimum = 0x00,   .maximum = 0xFF, .step = 1, .default_value = 0x80,  
    .flags = 0,
    }, {
    .id = V4L2_CID_SATURATION, .type = V4L2_CTRL_TYPE_INTEGER, 
    .name = "Saturation",
    .minimum = 0, .maximum = 255, .step = 0x1, .default_value = 128,
    .flags = 0,
    }, {
    .id = V4L2_CID_HUE, .type = V4L2_CTRL_TYPE_INTEGER, 
    .name = "Hue",
    .minimum = 0, .maximum = 63, .step = 0x1, .default_value = 32,
    .flags = 0,
    }, {
    .id = V4L2_CID_BLACK_LEVEL, .type = V4L2_CTRL_TYPE_BOOLEAN,
    .name = "Black Level",
    .minimum = 0, .maximum = 1, .step = 0, .default_value = 0,
    .flags = 0,
    }, {
    .id = V4L2_CID_AUTO_WHITE_BALANCE, .type = V4L2_CTRL_TYPE_BOOLEAN,
    .name = "Auto Contrast Adjust", /*Auto White Balance */
    .minimum = 0, .maximum = 1, .step = 0, .default_value = 0,
    .flags = 0,
    },{
    .id = V4L2_CID_SHARPNESS, .type = V4L2_CTRL_TYPE_INTEGER,
    .name = "Sharpness",
    .minimum = 0, .maximum = 15, .step = 0x1, .default_value = 1,
    .flags = 0,
    }
};


/*!
 * ioctl_queryctrl - V4L2 sensor interface handler for VIDIOC_QUERYCTRL ioctl
 * @s: pointer to standard V4L2 device structure
 * @qc: standard V4L2 VIDIOC_QUERYCTRL ioctl structure
 *
 * If the requested control is supported, returns the control information
 * from the video_control[] array.  Otherwise, returns -EINVAL if the
 * control is not supported.
 */
static int ioctl_queryctrl(struct v4l2_int_device *s, struct v4l2_queryctrl *qc)
{
    int i;
    os_print("ISL:ops %s.\n",__func__);
    for (i = 0; i < ARRAY_SIZE(isl79987_qctrl); i++) {
        if (qc->id && qc->id == isl79987_qctrl[i].id) {
            memcpy(qc, &(isl79987_qctrl[i]),  sizeof(*qc));
            return 0;
        }
    }
    return -EINVAL;
}



/*!
 * ioctl_g_ctrl - V4L2 sensor interface handler for VIDIOC_G_CTRL ioctl
 * @s: pointer to standard V4L2 device structure
 * @vc: standard V4L2 VIDIOC_G_CTRL ioctl structure
 *
 * If the requested control is supported, returns the control's current
 * value from the video_control[] array.  Otherwise, returns -EINVAL
 * if the control is not supported.
 */
static int ioctl_g_ctrl(struct v4l2_int_device *s, struct v4l2_control *vc)
{
    struct sensor_data *sensor = s->priv;
    u8 tmp;
    int i;
    os_print("ISL:ops %s.\n",__func__);
    switch (vc->id) {
    case V4L2_CID_BRIGHTNESS:
        WriteTW88(0xFF,sensor->v_channel+1);
        tmp = ReadTW88((u8)isl79987_IA_BRIGHTNESS_Y);
        if(tmp > 0x7F) {
            i = 0x100;
            i -= tmp;
            sensor->brightness = -(i);
            vc->value = -(i);
        }
        else {
            sensor->brightness = tmp;
            vc->value = tmp;
        }
        break;
    case V4L2_CID_CONTRAST:
        WriteTW88(0xFF,sensor->v_channel+1);
        tmp = ReadTW88((u8)isl79987_IA_CONTRAST_Y);
        sensor->contrast =tmp;
        vc->value = tmp;
        break;
    case V4L2_CID_SATURATION:
        WriteTW88(0xFF,sensor->v_channel+1);
        tmp = ReadTW88((u8)isl79987_IA_CONTRAST_CB);
        sensor->saturation = tmp;
        vc->value = tmp;
        break;
    case V4L2_CID_HUE:
        WriteTW88(0xFF,sensor->v_channel+1);
        tmp = ReadTW88((u8)isl79987_IA_HUE);
        /*convert HW to SW*/
        if(tmp == 32)       tmp = 32;  //32    => 32
        else if(tmp < 32)   tmp += 33; //0..31 => 33..63
        else                tmp -= 33; //33..63=> 0..30
        sensor->hue = tmp;
        vc->value = tmp;
        break;
    case V4L2_CID_BLACK_LEVEL:
        WriteTW88(0xFF,sensor->v_channel+1);
        tmp = ReadTW88(0x0C);
        if(tmp & 0x10)  vc->value = 1;
        else            vc->value = 0;
        break;
    case V4L2_CID_AUTO_WHITE_BALANCE:
        WriteTW88(0xFF,sensor->v_channel+1);
        tmp = ReadTW88(0x80);
        vc->value = tmp & 0x01;
        break;
    case V4L2_CID_SHARPNESS:
        WriteTW88(0xFF,sensor->v_channel+1);
        tmp = ReadTW88(0x12);
        tmp &= 0x0F;
        vc->value = tmp;
        break;
    case V4L2_CID_DO_WHITE_BALANCE:
    case V4L2_CID_RED_BALANCE:
    case V4L2_CID_BLUE_BALANCE:
    case V4L2_CID_GAMMA:    
    case V4L2_CID_EXPOSURE: 
    case V4L2_CID_AUTOGAIN: 
    case V4L2_CID_GAIN:
    case V4L2_CID_HFLIP:
    case V4L2_CID_VFLIP:
    default:
        pr_debug("%s unknown id:%x\n",__func__,vc->id);
        return -EINVAL;
    }
    return 0;
}

/*!
 * ioctl_s_ctrl - V4L2 sensor interface handler for VIDIOC_S_CTRL ioctl
 * @s: pointer to standard V4L2 device structure
 * @vc: standard V4L2 VIDIOC_S_CTRL ioctl structure
 *
 * If the requested control is supported, sets the control's current
 * value in HW (and updates the video_control[] array).  Otherwise,
 * returns -EINVAL if the control is not supported.
 */
static int ioctl_s_ctrl(struct v4l2_int_device *s, struct v4l2_control *vc)
{
    struct sensor_data *sensor = s->priv;
    u8 tmp;
    os_print("ISL:ops %s.\n",__func__);
    switch (vc->id) {
    case V4L2_CID_BRIGHTNESS:
        WriteTW88(0xFF,sensor->v_channel+1);
        WriteTW88((u8)isl79987_IA_BRIGHTNESS_Y,vc->value);
        sensor->brightness = vc->value;
        break;
    case V4L2_CID_CONTRAST:
        WriteTW88(0xFF,sensor->v_channel+1);
        WriteTW88((u8)isl79987_IA_CONTRAST_Y,vc->value);
        sensor->contrast = vc->value;
        break;
    case V4L2_CID_SATURATION:
        WriteTW88(0xFF,sensor->v_channel+1);
        WriteTW88((u8)isl79987_IA_CONTRAST_CB,vc->value);
        WriteTW88((u8)isl79987_IA_CONTRAST_CR,vc->value);
        sensor->saturation = vc->value;
        break;
    case V4L2_CID_HUE:
        tmp = vc->value;
        /*convert SW to HW*/
        if(tmp==32)       tmp = 32;  //32    =>32
        else if(tmp < 32) tmp += 33; //0..31 =>33..63
        else              tmp -= 33; //33..63=>0..31 
        WriteTW88(0xFF,sensor->v_channel+1);
        WriteTW88((u8)isl79987_IA_HUE, tmp);
        sensor->hue = vc->value;
        break;
    case V4L2_CID_BLACK_LEVEL:
        WriteTW88(0xFF,sensor->v_channel+1);
        tmp = ReadTW88(0x0C);
        if(vc->value) tmp |= 0x10;
        else            tmp &= ~0x10;
        WriteTW88(0x0C, tmp);
        break;
    case V4L2_CID_AUTO_WHITE_BALANCE:
        WriteTW88(0xFF,sensor->v_channel+1);
        tmp = ReadTW88(0x80);
        if(vc->value) tmp |= 0x81;
        else    tmp &= ~0x81;
        WriteTW88(0x80, tmp);
        break;
    case V4L2_CID_SHARPNESS:
        WriteTW88(0xFF,sensor->v_channel+1);
        tmp = ReadTW88(0x12);
        tmp &= 0xF0;
        tmp |= vc->value;
        WriteTW88(0x12,tmp);
        break;
    case V4L2_CID_DO_WHITE_BALANCE:
    case V4L2_CID_RED_BALANCE:
    case V4L2_CID_BLUE_BALANCE:
    case V4L2_CID_GAMMA:    
    case V4L2_CID_EXPOSURE: 
    case V4L2_CID_AUTOGAIN: 
    case V4L2_CID_GAIN:
    case V4L2_CID_HFLIP:
    case V4L2_CID_VFLIP:
    default:
        pr_debug("%s unknown id:%x\n",__func__,vc->id);
        return -EINVAL;
    }
    return 0;
}


/*!
 * ioctl_g_parm - V4L2 sensor interface handler for VIDIOC_G_PARM ioctl
 * @s: pointer to standard V4L2 device structure
 * @a: pointer to standard V4L2 VIDIOC_G_PARM ioctl structure
 *
 * Returns the sensor's video CAPTURE parameters.
 */
static int ioctl_g_parm(struct v4l2_int_device *s, struct v4l2_streamparm *a)
{
	struct sensor_data *sensor = s->priv;
	struct v4l2_captureparm *cparm = &a->parm.capture;
	int ret = 0;
	int raw_data_flag=0;
    os_print("ISL:ops %s.\n",__func__);
	switch (a->type) {
	/* This is the only case currently handled. */
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
	     
        //pr_err("%s raw_data %x:%x:%x:%x\n",__func__,
        //    a->parm.raw_data[100],
        //    a->parm.raw_data[101],
        //    a->parm.raw_data[102],
        //    a->parm.raw_data[103]); 
	    
	    if(a->parm.raw_data[100]=='9'
        && a->parm.raw_data[101]=='9'
	    && a->parm.raw_data[102]=='8'
	    && a->parm.raw_data[103]=='7') {
            //pr_debug("%s has raw_data[]!!\n",__func__);
            raw_data_flag=1;        	
	    }
		memset(a, 0, sizeof(*a));
		a->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		cparm->capability = sensor->streamcap.capability;
		cparm->timeperframe = sensor->streamcap.timeperframe;
		cparm->capturemode = sensor->streamcap.capturemode;
		ret = 0;
		break;

	/* These are all the possible cases. */
	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
	case V4L2_BUF_TYPE_VIDEO_OVERLAY:
	case V4L2_BUF_TYPE_VBI_CAPTURE:
	case V4L2_BUF_TYPE_VBI_OUTPUT:
	case V4L2_BUF_TYPE_SLICED_VBI_CAPTURE:
	case V4L2_BUF_TYPE_SLICED_VBI_OUTPUT:
		ret = -EINVAL;
		break;

	default:
		pr_debug("   type is unknown - %d\n", a->type);
		ret = -EINVAL;
		break;
	}

    if(ret)
        return ret;

    if(raw_data_flag) {
	    a->parm.raw_data[100]='9';
        a->parm.raw_data[101]='9';
	    a->parm.raw_data[102]='8';
	    a->parm.raw_data[103]='7';
        a->parm.raw_data[104]=0; 
        a->parm.raw_data[105]=sensor->is_mipi;
        a->parm.raw_data[106]=sensor->max_sensor;
        a->parm.raw_data[107]=sensor->lanes;
        a->parm.raw_data[108]=sensor->pseudo;
        a->parm.raw_data[109]=sensor->histo;
        a->parm.raw_data[110]=(u8)(sensor->bclock>>8);
        a->parm.raw_data[111]=(u8)sensor->bclock;
        a->parm.raw_data[112]=sensor->nvc;
        pr_debug("%s sendback raw_data[]!!\n",__func__);
    }
    
	return ret;
}

/*!
 * ioctl_s_parm - V4L2 sensor interface handler for VIDIOC_S_PARM ioctl
 * @s: pointer to standard V4L2 device structure
 * @a: pointer to standard V4L2 VIDIOC_S_PARM ioctl structure
 *
 * Configures the sensor to use the input parameters, if possible.  If
 * not possible, reverts to the old parameters and returns the
 * appropriate error code.
 */
/*! freescale code
 *   do nothing. It just resutn 0.
*/
static int ioctl_s_parm(struct v4l2_int_device *s, struct v4l2_streamparm *a)
{
	struct sensor_data *sensor = s->priv;
	struct v4l2_captureparm *cparm = &a->parm.capture;
	//struct v4l2_fract *timeperframe = &a->parm.capture.timeperframe;
	int ret = 0;
	unsigned int mode;
    os_print("ISL:ops %s.\n",__func__);
    if(a->type != V4L2_BUF_TYPE_VIDEO_CAPTURE) {
 	    switch (a->type) {
      	/* These are all the possible cases. */
	    case V4L2_BUF_TYPE_VIDEO_OUTPUT:
	    case V4L2_BUF_TYPE_VIDEO_OVERLAY:
	    case V4L2_BUF_TYPE_VBI_CAPTURE:
	    case V4L2_BUF_TYPE_VBI_OUTPUT:
	    case V4L2_BUF_TYPE_SLICED_VBI_CAPTURE:
	    case V4L2_BUF_TYPE_SLICED_VBI_OUTPUT:
		    pr_debug("   type is not " \
			    "V4L2_BUF_TYPE_VIDEO_CAPTURE but %d\n",
			    a->type);
		    ret = -EINVAL;
		    break;
	    default:
		    pr_debug("   type is unknown - %d\n", a->type);
		    ret = -EINVAL;
		    break;
	    }
        return ret;
    }

    sensor->streamcap.capturemode = cparm->capturemode;
    if(sensor->streamcap.capturemode) {
        mode = sensor->streamcap.capturemode;
        pr_debug("%s capturemode:0x%x\n", __func__,mode);
        if(mode & 0x10)    sensor->pseudo=1;
        else               sensor->pseudo=0;
        if(mode & 0x08)    sensor->histo=1;
        else               sensor->histo=0;
        if(mode & 0x01)    sensor->lanes=1;
        else               sensor->lanes=2; /*note*/
        mode >>= 1;
        mode &= 0x03; /* note */        
        if     (mode==1)    sensor->max_sensor = 1;
        else if(mode==2)    sensor->max_sensor = 2;
        else /*note */      sensor->max_sensor = 4;
        
        //skip--------------
        //sensor->is_mipi=
        //sensor->is_dummy=
        //sensor->bclock=
        //sensor->nvc=
        //--------------------
        //try 0x10....for 4ch2lane pseudo
        //try 0x18....for 4ch2lane pseudo with histogram
        isl79987_init_chip_registers(sensor);

        mutex_lock(&mutex);
        dev_inited_counter = 0; /* clear */ 
        mutex_unlock(&mutex);                  
        ret = mxc_init_mipi_csi2(sensor);		    

        return ret;          
    }        

	if(a->parm.raw_data[100]=='9'
    && a->parm.raw_data[101]=='9'
	&& a->parm.raw_data[102]=='8'
	&& a->parm.raw_data[103]=='7'
	) {
		pr_debug("%s raw_data!!\n",__func__);
	    sensor->is_mipi = 1;  /* of course ! */
		sensor->is_dummy = a->parm.raw_data[105];
	    sensor->max_sensor = a->parm.raw_data[106];
	    sensor->lanes = a->parm.raw_data[107];
		sensor->pseudo= a->parm.raw_data[108]; /* 0,1,4*/
		sensor->histo = a->parm.raw_data[109]; /* 0,1*/
	    sensor->bclock = a->parm.raw_data[110];
	    sensor->bclock <<= 8;
	    sensor->bclock |= a->parm.raw_data[111];
		sensor->nvc = a->parm.raw_data[112];

		pr_debug("9987 receives max_sensor:%d lanes:%d bclock:%d\n", 
		    sensor->max_sensor,
		    sensor->lanes,
		    sensor->bclock);

		pr_debug("9987 receives pseudo:%d histo:%d\n", 
		    sensor->pseudo,
		    sensor->histo);

        /* BKNote 170411.
            Do not execute raw_date[] s_parm
            if you are not try to do something.
            Normally, it have to be execute by iv4l2_set.
        */            

        isl79987_init_chip_registers(sensor);

        if(dev_inited_counter)
            /* it can generate video noisy */
            pr_debug("dev_inited_counter was %d\n",dev_inited_counter);

        mutex_lock(&mutex);
        dev_inited_counter = 0; /* clear */ 
        mutex_unlock(&mutex);                  
        ret = mxc_init_mipi_csi2(sensor);		    
	}

	return ret;
}

/*
 * static
 */
static int you_are_ntsc(struct v4l2_int_device *s, v4l2_std_id *std)
{
    struct sensor_data *sensor = s->priv;
    int ret;
    
    isl79987_inputs[sensor->v_channel].status &= ~V4L2_IN_ST_NO_SIGNAL;
    isl79987_inputs[sensor->v_channel].status &= ~V4L2_IN_ST_NO_H_LOCK;
    *std = V4L2_STD_NTSC;
    sensor->std_id = V4L2_STD_NTSC;
    sensor->pix.width = 720;
    sensor->pix.height = 480; /*BT656:525*/
    sensor->streamcap.timeperframe.denominator = 30;
    sensor->pix.sizeimage = 720 * 2 * 480;  

    ret = mxc_wait_mipi_csi2_ready(100);
    if(ret)
        pr_err("%s failed mxc_wait_mipi_csi2_ready\n",__func__);
    return ret;        
}


/*
 * ioctl_querystd - V4L2 decoder interface handler for VIDIOC_QUERYSTD ioctl
 * @s: pointer to standard V4L2 device structure
 * @std_id: standard V4L2 std_id ioctl enum
 * Returns the current standard detected by tw9992. 
 * If no active input is detected, returns -EINVAL
*/
static int ioctl_querystd(struct v4l2_int_device *s, v4l2_std_id *std)
{
    struct sensor_data *sensor = s->priv;
    volatile u8 status_reg;       /*0x03*/
    volatile u8 standard_reg;     /*0x1C*/
    volatile u8 macrovision_reg;  /*0x30*/
    int i,count;
    int ret;
    os_print("ISL:ops %s.\n",__func__);
    if(sensor->is_dummy)
        return you_are_ntsc(s,std);
    
    *std = V4L2_STD_UNKNOWN;
    
    /* select Page register */
    WriteTW88(0xFF,sensor->v_channel+1);
    
    /* check VDLOSS, LOCK */
    i=100;
    while (--i) {
        status_reg = ReadTW88(0x03);
        if (( status_reg & 0xE8 ) == 0x68 )
            break; 
        msleep(10);
    }
    if(i==0) {
        /* failed */
        if(status_reg & 0x80) {
            pr_err("%s detect VDLOSS\n",__func__);
            isl79987_inputs[sensor->v_channel].status |= V4L2_IN_ST_NO_SIGNAL;
        }            
        else {
            pr_err("%s fail LOCK R03=%x\n",__func__, status_reg);
            isl79987_inputs[sensor->v_channel].status |= V4L2_IN_ST_NO_H_LOCK;
        }            
        return -EINVAL; 
    }
    pr_debug("%s use %d0ms\n",__func__, 99-i);    
    isl79987_inputs[sensor->v_channel].status &= ~V4L2_IN_ST_NO_SIGNAL;
    isl79987_inputs[sensor->v_channel].status &= ~V4L2_IN_ST_NO_H_LOCK;

    /* check Standard */
    i=100;
    count = 0;
    while (--i) {
        standard_reg = ReadTW88(0x1C);
        if (( standard_reg & 0x80 ) == 0 ) {
            status_reg = ReadTW88(0x03);
            macrovision_reg = ReadTW88(0x30);
            if((status_reg & 0x68) && ((macrovision_reg & 0xE0)==0)) {
                if(count > 4)
                    break;  
                count++;
            }
        }
        msleep(5);
    }
    if(i==0) {
        /*failed. if status_reg was 0x41 */
        if((status_reg & 0xC1) == 0x41)
            ; 
        else
            standard_reg = 0x80;                    
    }

    if(standard_reg & 0x80) {
        /* failed */
        pr_err(" NoSTD\n");
        /* use V4L2_IN_ST_NO_STD_LOCK */
        isl79987_inputs[sensor->v_channel].status |= V4L2_IN_ST_NO_H_LOCK;
        return -EINVAL;
    }
    /* use V4L2_IN_ST_NO_STD_LOCK */
    isl79987_inputs[sensor->v_channel].status &= ~V4L2_IN_ST_NO_H_LOCK;

    standard_reg >>= 4;

    if(standard_reg==1 /* PAL(B,D,G,H,I)*/
    || standard_reg==2 /* SECAM */
    || standard_reg==5 /* PAL(CN)*/
    ) {
        *std = V4L2_STD_PAL;
    }
    else {
        *std = V4L2_STD_NTSC;
    }

    pr_debug("%s detect std:%Lx\n",__func__,*std);
    
    if(sensor->std_id != *std) {        
        ret=isl79987_update_sensor_data(s,std);
        if(ret)
            pr_err("%s failed isl79987_update_sensor_data\n",__func__);
    }
    return 0;
}

/*
use ioctl_querystd to get more accurate result.
*/
static int ioctl_g_std(struct v4l2_int_device *s, v4l2_std_id *std)
{
    struct sensor_data *sensor = s->priv;
    int ret;
    u8 reg;
    os_print("ISL:ops %s.\n",__func__);
    if(sensor->is_dummy)
        return you_are_ntsc(s,std);

    *std = V4L2_STD_UNKNOWN;
    
    /* check VDLOSS, LOCK */
    WriteTW88(0xFF, sensor->v_channel+1);
    reg = ReadTW88(0x03);
    if(reg & 0x80) {  /* VDLOSS */
        pr_err("%s detect VDLOSS\n",__func__);
        isl79987_inputs[sensor->v_channel].status |= V4L2_IN_ST_NO_SIGNAL;
        return -EINVAL;
    }
    if((reg & 0x68) != 0x68) {  /* Lock */
        pr_err("%s fail Lock. R03=%x\n",__func__,reg);
        isl79987_inputs[sensor->v_channel].status |= V4L2_IN_ST_NO_H_LOCK;
        return -EINVAL;
    }
    isl79987_inputs[sensor->v_channel].status &= ~V4L2_IN_ST_NO_SIGNAL;
    isl79987_inputs[sensor->v_channel].status &= ~V4L2_IN_ST_NO_H_LOCK;
    
    /* Read the isl79987_DEC_STANDARD to get the detect output video standard */
    reg = ReadTW88((u8)isl79987_DEC_STANDARD);
    reg &= 0x70;

    if (reg == 0x10 || reg == 0x20 || reg == 0x50) {
        *std = V4L2_STD_PAL;
    } else if (reg == 0x00 || reg == 0x30 || reg == 0x40 || reg == 0x60) {
        *std = V4L2_STD_NTSC;
    } else {
        *std = V4L2_STD_ALL;
        pr_debug("Got invalid video standard!\n");
    }

    pr_debug("%s detect std:%Lx\n",__func__,*std);

    ret=0;
    /* This assumes autodetect which this device uses. */
    /* mipi returns active, bt656 returns raw(period) */
    if(sensor->std_id != *std) {

        ret=isl79987_update_sensor_data(s,std);
        if(ret)
            pr_err("%s failed isl79987_update_sensor_data\n",__func__);
    }

    return 0;
}

/**
 * ioctl_s_std - V4L2 decoder interface handler for VIDIOC_S_STD ioctl
 * @s: pointer to standard V4L2 device structure
 * @std_id: standard V4L2 v4l2_std_id ioctl enum
 *
 * The decoder can not set std_id, use querystd and g_set.
 */
static int ioctl_s_std(struct v4l2_int_device *s, v4l2_std_id std)
{
    os_print("ISL:ops %s.\n",__func__);
    return 0;
}



/*--------------------------------------------------------------------------*/
/*---------------- private IOCTL functions                  ----------------*/
/*--------------------------------------------------------------------------*/


/*!
 * ioctl_dev_init - V4L2 sensor interface handler for vidioc_int_dev_init_num
 * @s: pointer to standard V4L2 device structure
 *
 * Initialise the device when slave attaches to the master.
 */
static int ioctl_dev_init(struct v4l2_int_device *s)
{
	struct sensor_data *sensor = s->priv;
	int ret = 0;
#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
	int counter;

    mutex_lock(&mutex);
    dev_inited_counter++;
    counter = dev_inited_counter;
    mutex_unlock(&mutex);

    pr_debug("isl79987 %s %d counter:%d\n",__func__, sensor->input_source, counter);
#else
    pr_debug("isl79987 %s %d\n",__func__, sensor->input_source);
#endif
    os_print("ISL:ops %s.\n",__func__);
	sensor->on = true;

#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
    if(counter > 1) { 
        pr_debug("    skip init_mipi_csi2_rx()\n");
        return ret;
    }      
#endif	
	ret = mxc_init_mipi_csi2(sensor);

	return ret;
}

/*!
 * ioctl_dev_exit - V4L2 sensor interface handler for vidioc_int_dev_exit_num
 * @s: pointer to standard V4L2 device structure
 *
 * Delinitialise the device when slave detaches to the master.
 */
static int ioctl_dev_exit(struct v4l2_int_device *s)
{
	struct sensor_data *sensor = s->priv;
	void *mipi_csi2_info;
	int counter;
    os_print("ISL:ops %s.\n",__func__);
	counter=0;
    pr_debug("isl79987 %s %d\n",__func__, sensor->input_source);
#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
    mutex_lock(&mutex);
    if(dev_inited_counter)
        dev_inited_counter--;
    counter = dev_inited_counter;        
    mutex_unlock(&mutex);
#endif
    if(counter == 0) {
		mipi_csi2_info = mipi_csi2_get_info();

		/* disable mipi csi2 */
		if (mipi_csi2_info)
			if (mipi2_csi2_get_status(mipi_csi2_info))
				mipi2_csi2_disable(mipi_csi2_info);
    }    
    return 0;
}


/*!
 * ioctl_s_power - V4L2 sensor interface handler for VIDIOC_S_POWER ioctl
 * @s: pointer to standard V4L2 device structure
 * @on: indicates power mode (on or off)
 *
 * Turns the power on or off, depending on the value of on and returns the
 * appropriate error code.
 */
static int ioctl_s_power(struct v4l2_int_device *s, int on)
{
	struct sensor_data *sensor = s->priv;
    os_print("ISL:ops %s.\n",__func__);
    pr_debug("isl79987 %s %d on:%d\n",__func__, sensor->input_source, on);
	sensor->on = on;

	return 0;
}

static int ioctl_g_priv(struct v4l2_int_device *s, void *p)
{
	struct sensor_data *sensor = s->priv;
    os_print("ISL:ops %s.\n",__func__);
    p = (void *)sensor;
    return 0;
}

static int ioctl_g_ifparm(struct v4l2_int_device *s, struct v4l2_ifparm *p)
{
	struct sensor_data *sensor = s->priv;
    os_print("ISL:ops %s.\n",__func__);
	if (s == NULL) {
		pr_err("   ERROR!! no slave device set!\n");
		return -1;
	}

    /* isl79987 is mipi, not BT656. but send 0 to make it work. */
	memset(p, 0, sizeof(*p));
	p->u.bt656.clock_curr = sensor->mclk;  
	pr_debug("   clock_curr=mclk=%d\n", sensor->mclk);
	p->if_type = V4L2_IF_TYPE_BT656;
	p->u.bt656.mode = V4L2_IF_TYPE_BT656_MODE_NOBT_8BIT;
	p->u.bt656.bt_sync_correct = 1;  /* Indicate external vsync */
	
	return 0;
}



/*!
 * ioctl_enum_framesizes - V4L2 sensor interface handler for
 *			   VIDIOC_ENUM_FRAMESIZES ioctl
 * @s: pointer to standard V4L2 device structure
 * @fsize: standard V4L2 VIDIOC_ENUM_FRAMESIZES ioctl structure
 *
 * Return 0 if successful, otherwise -EINVAL.
 */
static int ioctl_enum_framesizes(struct v4l2_int_device *s,
				 struct v4l2_frmsizeenum *fsize)
{
	//struct sensor_data *sensor = s->priv;
	//int index;
    os_print("ISL:ops %s.\n",__func__);
	if (fsize->index > 2) /*0:NTSC, 1:PAL 2:Unlock */
		return -EINVAL;
					
    fsize->type = V4L2_FRMSIZE_TYPE_DISCRETE;
	fsize->discrete.width  = video_fmts[fsize->index].active_width;
	fsize->discrete.height = video_fmts[fsize->index].active_height;
#if 0 //skip..
    //pixel_format;
#endif    	
	return 0;
}

/*!
 * ioctl_enum_frameintervals - V4L2 sensor interface handler for
 *			       VIDIOC_ENUM_FRAMEINTERVALS ioctl
 * @s: pointer to standard V4L2 device structure
 * @fival: standard V4L2 VIDIOC_ENUM_FRAMEINTERVALS ioctl structure
 *
 * Return 0 if successful, otherwise -EINVAL.
 */
static int ioctl_enum_frameintervals(struct v4l2_int_device *s, struct v4l2_frmivalenum *frmi)
{
    os_print("ISL:ops %s.\n",__func__);
    if (frmi->index > 2) /* 0:NTSC, 1:PAL, 2:Unlock */
        return -EINVAL;

    frmi->type = V4L2_FRMSIZE_TYPE_DISCRETE;
    frmi->discrete.numerator = 1;
    frmi->discrete.denominator = video_fmts[frmi->index].frame_rate;
#if 0 //skip..
    //pixel_format;
    //width
    //height;
#endif    

    return 0;
}


static int ioctl_reset(struct v4l2_int_device *s)
{
	struct sensor_data *sensor = s->priv;
    os_print("ISL:ops %s.\n",__func__);
    pr_debug("isl79987 %s %d\n",__func__, sensor->input_source);
    return 0;
}


/*!
 * ioctl_init - V4L2 sensor interface handler for VIDIOC_INT_INIT
 * @s: pointer to standard V4L2 device structure
 */
static int ioctl_init(struct v4l2_int_device *s)
{
	struct sensor_data *sensor = s->priv;
    os_print("ISL:ops %s.\n",__func__);
    pr_debug("isl79987 %s %d\n",__func__, sensor->input_source);
	return 0;
}


/*!
 * ioctl_g_chip_ident - V4L2 sensor interface handler for
 *			VIDIOC_DBG_G_CHIP_IDENT ioctl
 * @s: pointer to standard V4L2 device structure
 * @id: pointer to int
 *
 * Return 0.
 */
static int ioctl_g_chip_ident(struct v4l2_int_device *s, int *id)
{
    os_print("ISL:ops %s.\n",__func__);

	((struct v4l2_dbg_chip_ident *)id)->match.type =
					V4L2_CHIP_MATCH_I2C_DRIVER;

	if(isl79987_data[0].pix.priv == 0)
	    strcpy(((struct v4l2_dbg_chip_ident *)id)->match.name,
		    "ovisl79987_mipi_decoder");
    else
	    strcpy(((struct v4l2_dbg_chip_ident *)id)->match.name,
		    "advisl79987_mipi_decoder");

	return 0;
}


static int ioctl_enuminput(struct v4l2_int_device *s, struct v4l2_input *vin)
{
    os_print("ISL:ops %s.\n",__func__);

    if(vin->index > ISL79987_INPUT_MAX) 
        return -EINVAL;

    memcpy(vin,&isl79987_inputs[vin->index], sizeof(struct v4l2_input));
    return 0;
}


/* location:
static int ioctl_g_input(struct v4l2_int_device *s, unsigned int *inp)
static int ioctl_s_input(struct v4l2_int_device *s, unsigned int inp)
*/


static int ioctl_dbg_g_register(struct v4l2_int_device *s, struct v4l2_dbg_register *reg)
{
    os_print("ISL:ops %s.\n",__func__);

    if(reg->match.addr != ISL79987_I2C_ID)
        return -EINVAL;

    reg->val = ReadTW88((u8)reg->reg);
    return 0;        
}
static int ioctl_dbg_s_register(struct v4l2_int_device *s, struct v4l2_dbg_register *reg)
{
    os_print("ISL:ops %s.\n",__func__);

    if(reg->match.addr != ISL79987_I2C_ID)
        return -EINVAL;

    WriteTW88((u8)reg->reg, (u8)reg->val);
    return 0;
}

/*!
 * This structure defines all the ioctls for this module and links them to the
 * enumeration.
 */
 /* BK160819: i think, we better follow up IOCTL CODES */
static struct v4l2_int_ioctl_desc isl79987_ioctl_desc[] = {
          /* "Proper" V4L ioctls, as in struct video_device. */
/*V:2  */ {vidioc_int_enum_fmt_cap_num,	  (v4l2_int_ioctl_func *)ioctl_enum_fmt_cap}, 
/*V:4  */ {vidioc_int_g_fmt_cap_num,      (v4l2_int_ioctl_func *)ioctl_g_fmt_cap},    
/*V:5  */ {vidioc_int_s_fmt_cap_num,      (v4l2_int_ioctl_func *)ioctl_s_fmt_cap},    
/*V:21 */ {vidioc_int_g_parm_num,         (v4l2_int_ioctl_func *)ioctl_g_parm},       
/*V:22 */ {vidioc_int_s_parm_num,         (v4l2_int_ioctl_func *)ioctl_s_parm},       
/*V:23 */ //{vidioc_int_g_std_num,          (v4l2_int_ioctl_func *)ioctl_g_std},
/*V:24 */ {vidioc_int_s_std_num,          (v4l2_int_ioctl_func *)ioctl_s_std},        
/*V:26 */ //{vidioc_int_enuminput_num,      (v4l2_int_ioctl_func *)ioctl_enuminput},
/*V:27 */ {vidioc_int_g_ctrl_num,         (v4l2_int_ioctl_func *)ioctl_g_ctrl},       
/*V:28 */ {vidioc_int_s_ctrl_num,         (v4l2_int_ioctl_func *)ioctl_s_ctrl},       	
/*V:36 */ {vidioc_int_queryctrl_num,      (v4l2_int_ioctl_func *)ioctl_queryctrl},    
/*V:38 */ /*{vidioc_int_g_input_num,      (v4l2_int_ioctl_func *)ioctl_g_input},*/
/*V:39 */ /*{vidioc_int_s_input_num,      (v4l2_int_ioctl_func *)ioctl_s_input},*/

/*V:58 */ /*{vidioc_int_cropcap_num,      (v4l2_int_ioctl_func *)ioctl_cropcap},*/    
/*V:59 */ /*{vidioc_int_g_crop_num,       (v4l2_int_ioctl_func *)ioctl_g_crop},*/     
/*V:60 */ /*{vidioc_int_s_crop_num,       (v4l2_int_ioctl_func *)ioctl_s_crop},*/     
/*V:63 */ {vidioc_int_querystd_num,       (v4l2_int_ioctl_func *)ioctl_querystd},      
/*V:64 */ {vidioc_int_try_fmt_cap_num,	  (v4l2_int_ioctl_func *)ioctl_try_fmt_cap},  
	
/*V:79 */ //{vidioc_int_dbg_s_register_num, (v4l2_int_ioctl_func *)ioctl_dbg_s_register},
/*V:80 */ //{vidioc_int_dbg_g_register_num, (v4l2_int_ioctl_func *)ioctl_dbg_g_register},
/*V:102*/ {vidioc_int_g_chip_ident_num,	(v4l2_int_ioctl_func *)ioctl_g_chip_ident},     
/*V:111*/ /*{vidioc_int_g_video_routing_num,(v4l2_int_ioctl_func *)ioctl_g_video_routing},*/ 
/*V:112*/ /*{vidioc_int_s_video_routing_num,(v4l2_int_ioctl_func *)ioctl_s_video_routing},*/  

          /* Strictly internal ioctls. */ 
          {vidioc_int_dev_init_num,       (v4l2_int_ioctl_func *) ioctl_dev_init},
          {vidioc_int_dev_exit_num,                               ioctl_dev_exit},
          {vidioc_int_s_power_num,        (v4l2_int_ioctl_func *) ioctl_s_power},
          /* 
           * Get slave private data, e.g. platform-specific slave
           * configuration used by the master.
          */
          {vidioc_int_g_priv_num,         (v4l2_int_ioctl_func *)ioctl_g_priv},
	      {vidioc_int_g_ifparm_num,       (v4l2_int_ioctl_func *)ioctl_g_ifparm},
          /*{vidioc_int_g_needs_reset_num,	(v4l2_int_ioctl_func *) ioctl_g_needs_reset}, */
/*V:74 */ {vidioc_int_enum_framesizes_num,(v4l2_int_ioctl_func *)ioctl_enum_framesizes},
/*V:75 */ {vidioc_int_enum_frameintervals_num,(v4l2_int_ioctl_func *)ioctl_enum_frameintervals},

          {vidioc_int_reset_num,          (v4l2_int_ioctl_func *)ioctl_reset},
	      {vidioc_int_init_num,           (v4l2_int_ioctl_func *)ioctl_init},

          /*
           * Start of private ioctls for Intersil.
           * add at the end of vidioc_int_priv_start_num = 2000,
           */
};

static struct v4l2_int_slave isl79987_slave[SENSOR_NUM] = {
	{
	.ioctls = isl79987_ioctl_desc,
	.num_ioctls = ARRAY_SIZE(isl79987_ioctl_desc),
	},

	{
	.ioctls = isl79987_ioctl_desc,
	.num_ioctls = ARRAY_SIZE(isl79987_ioctl_desc),
	},

	{
	.ioctls = isl79987_ioctl_desc,
	.num_ioctls = ARRAY_SIZE(isl79987_ioctl_desc),
	},

	{
	.ioctls = isl79987_ioctl_desc,
	.num_ioctls = ARRAY_SIZE(isl79987_ioctl_desc),
	}
};

static struct v4l2_int_device isl79987_int_device[SENSOR_NUM] = {
	{
		.module = THIS_MODULE,
		.name = "isl79987",
		.type = v4l2_int_type_slave,
		.u = {
			.slave = &isl79987_slave[0],
		},
	}, 
	{
		.module = THIS_MODULE,
		.name = "isl79987",
		.type = v4l2_int_type_slave,
		.u = {
			.slave = &isl79987_slave[1],
		},
	}, 
	{
		.module = THIS_MODULE,
		.name = "isl79987",
		.type = v4l2_int_type_slave,
		.u = {
			.slave = &isl79987_slave[2],
		},
	}, 
	{
		.module = THIS_MODULE,
		.name = "isl79987",
		.type = v4l2_int_type_slave,
		.u = {
			.slave = &isl79987_slave[3],
		},
	}
};


/*!
 * isl79987 I2C probe function
 *
 * @param adapter            struct i2c_adapter *
 * @return  Error code indicating success or failure
 */
static int isl79987_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	int retval;
	int dummy;
	
    pr_err("In isl79987_probe\n");
    dev_inited_counter=0;

    /* Check if the adapter supports the needed features */
    if (!i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_BYTE_DATA)) {        
		pr_info("**Intersil isl79987_probe failed**");
        return -ENODEV;
    }
    v4l_info(client, "chip found @ 0x%x (%s)\n",
        client->addr << 1, client->adapter->name);

	/* Set initial values for the sensor struct. */
	memset(&isl79987_data[0], 0, sizeof(isl79987_data[0]));
	
	/* read property from device tree.
	   I am not using, but, keep it to do...
        isl79987_data[0].mclk = 27000000;
        isl79987_data[0].mclk_source = 0;
        isl79987_data[0].csi_id = 0;
        isl79987_data[0].is_dummy = 0;
    */

	isl79987_data[0].sensor_clk = devm_clk_get(dev, "csi_mclk");
	if (IS_ERR(isl79987_data[0].sensor_clk)) {
		isl79987_data[0].sensor_clk = NULL;
		dev_err(dev, "clock-frequency missing or invalid\n");
		return PTR_ERR(isl79987_data[0].sensor_clk);
	}

	retval = of_property_read_u32(dev->of_node, "mclk",
					&(isl79987_data[0].mclk));
	if (retval) {
		dev_err(dev, "mclk missing or invalid\n");
		return retval;
	}

	retval = of_property_read_u32(dev->of_node, "mclk_source",
					(u32 *) &(isl79987_data[0].mclk_source));
	if (retval) {
		dev_err(dev, "mclk_source missing or invalid\n");
		return retval;
	}
    
	retval = of_property_read_u32(dev->of_node, "csi_id",
					&(isl79987_data[0].csi));
	if (retval) {
		dev_err(dev, "csi id missing or invalid\n");
		return retval;
	}

	retval = of_property_read_u32(dev->of_node, "dummy",
					&dummy);
	if (retval)
		isl79987_data[0].is_dummy = 0;
	else {
	    if(dummy) 
    	    isl79987_data[0].is_dummy = 1;    
    	else
    	    isl79987_data[0].is_dummy = 0;    
    }

	clk_prepare_enable(isl79987_data[0].sensor_clk);

	isl79987_data[0].i2c_client = client;
	isl79987_data[0].is_mipi = 1;
	isl79987_data[0].pix.pixelformat = V4L2_PIX_FMT_UYVY;
	isl79987_data[0].pix.width = 720;
    isl79987_data[0].pix.height = 480; /* for FrameMode */
	isl79987_data[0].pix.field = V4L2_FIELD_INTERLACED_TB;
    isl79987_data[0].pix.bytesperline = 720 * 2;
    isl79987_data[0].pix.sizeimage = 720 * 2 * 480;
	isl79987_data[0].pix.priv = 1;  /*NOTE: 1 means TV_in. 0 is camera */ 
	
	isl79987_data[0].streamcap.capturemode = 0;
	isl79987_data[0].streamcap.timeperframe.denominator = 30;
	isl79987_data[0].streamcap.timeperframe.numerator = 1;
	
    isl79987_data[0].streamcap.capability 
        = V4L2_MODE_HIGHQUALITY | V4L2_CAP_TIMEPERFRAME;

    isl79987_data[0].on = false; /* _dev_init() will assign true*/
	isl79987_data[0].max_sensor = 4;
	isl79987_data[0].lanes = 2;
	isl79987_data[0].bclock = 216;

	isl79987_data[0].pseudo = 0;
	isl79987_data[0].histo = 0;
	isl79987_data[0].nvc = 0;

    //isl79987_data[0].use_counter = 0;
    isl79987_data[0].std_id = V4L2_STD_ALL;
    isl79987_data[0].prev_std_id = V4L2_STD_NTSC;

#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
	isl79987_data[0].ipu_id = 0;
#endif
	isl79987_data[0].csi = 0;
	isl79987_data[0].v_channel = 0;
	isl79987_data[0].input_source = 0; /* CH1,Decoder0 */

#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
	memcpy(&isl79987_data[1], &isl79987_data[0], sizeof(struct sensor_data));
	memcpy(&isl79987_data[2], &isl79987_data[0], sizeof(struct sensor_data));
	memcpy(&isl79987_data[3], &isl79987_data[0], sizeof(struct sensor_data));

	isl79987_data[1].i2c_client = NULL;
	isl79987_data[2].i2c_client = NULL;
	isl79987_data[3].i2c_client = NULL;

	isl79987_data[1].ipu_id = 0;
	isl79987_data[1].csi = 1;
	isl79987_data[1].v_channel = 1;
	isl79987_data[1].input_source = 1;  /* CH2,Decoder1 */

	isl79987_data[2].ipu_id = 1;
	isl79987_data[2].csi = 0;
	isl79987_data[2].v_channel = 2;
	isl79987_data[2].input_source = 2;  /* CH3,Decoder2 */

	isl79987_data[3].ipu_id = 1;
	isl79987_data[3].csi = 1;
	isl79987_data[3].v_channel = 3;
	isl79987_data[3].input_source = 3;  /* CH4,Decoder3 */
#endif
    /* check CHIP ID */
    if(isl79987_data[0].is_dummy) {
        pr_info("Renesas isl79987 dummy driver\n");
    }
    else {
	    WriteTW88(0xFF, 0x00);
	    retval = ReadTW88(0x00);
	    retval = 0x87;
	    if (retval != 0x87) {
		    pr_warning("isl79987 is not found, chip id reg 0x00 = 0x%x.\n", retval);
#if 1 /* dummy mode */
            dummy= 1;
            pr_info(" ***enable dummy to debug*** \n");
			isl79987_data[0].is_dummy = 1;
#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
			isl79987_data[1].is_dummy = 1;
			isl79987_data[2].is_dummy = 1;
			isl79987_data[3].is_dummy = 1;
#endif
#else //..dummy		    
		    clk_disable_unprepare(isl79987_data[0].sensor_clk);
		    return -ENODEV;
#endif //..dummy
	    }
	    else {
            pr_info("Renesas isl79987 id:%2X ",retval);
            retval = ReadTW88(0x01);
            pr_info("rev:%2X is found.\n", retval);
	    }
    }	   
 
	/* v4l2 register */
	isl79987_int_device[0].priv = &isl79987_data[0];
#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
	isl79987_int_device[1].priv = &isl79987_data[1];
	isl79987_int_device[2].priv = &isl79987_data[2];
	isl79987_int_device[3].priv = &isl79987_data[3];
#endif
	retval  = v4l2_int_device_register(&isl79987_int_device[0]);
#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)
	retval |= v4l2_int_device_register(&isl79987_int_device[1]);
	retval |= v4l2_int_device_register(&isl79987_int_device[2]);
	retval |= v4l2_int_device_register(&isl79987_int_device[3]);
#endif
	clk_disable_unprepare(isl79987_data[0].sensor_clk);
    if(retval)
        goto err;

    /* initialize chip with SW default value */
    if(isl79987_data[0].is_dummy==0)
        isl79987_init_chip_registers(NULL);

    pr_debug( "%s finished.\n",__func__);
err:
    /* add regulator code */
	return retval;
}


/*!
 * isl79987 I2C detach function
 *
 * @param client            struct i2c_client *
 * @return  Error code indicating success or failure
 */
static int isl79987_remove(struct i2c_client *client)
{
#if defined(CONFIG_MXC_IPU_4CH_CAPTURE)	
	v4l2_int_device_unregister(&isl79987_int_device[3]);
	v4l2_int_device_unregister(&isl79987_int_device[2]);
	v4l2_int_device_unregister(&isl79987_int_device[1]);
#endif
	v4l2_int_device_unregister(&isl79987_int_device[0]);

    /* poweroff_regulator */
	/*  add regulator code */
	return 0;
}

/*!
 * for platform device driver
 *
 * On L3.10.53, use arch/arm/boot/dts/imx6q-wandboard.dts.
 * for example
 *  &i2c2 {
 *      isl79987_mipi: isl79987_mipi @44 {
            ...
        }
 *  }
 */

#define	I2CDRV_MODULE_NAME		"isl79987_mipi"
#define	I2CDRV_COMPATIBLE_NAME	"isl,isl79987_mipi"

static const struct i2c_device_id isl79987_id[] = {
	{I2CDRV_MODULE_NAME, 0},
	{},
};

MODULE_DEVICE_TABLE(i2c, isl79987_id);

#include <linux/mod_devicetable.h>
static const struct of_device_id drv_i2c_of_match[] = {
	{
		.compatible = I2CDRV_COMPATIBLE_NAME,
	},
	{},			//must be a null
};

MODULE_DEVICE_TABLE(of, drv_i2c_of_match);

static struct i2c_driver isl79987_i2c_driver = {
	.driver = {
		  .owner = THIS_MODULE,
		  .name  = I2CDRV_MODULE_NAME,
		  .of_match_table = drv_i2c_of_match,
		  },
	.probe  = isl79987_probe,
	.remove = isl79987_remove,
	.id_table = isl79987_id,
};


/*!
 * isl79987 init function
 *
 * @return  Error code indicating success or failure
 */

static __init int isl79987_module_init(void)
{
	u8 err;

	err = i2c_add_driver(&isl79987_i2c_driver);
	if (err != 0)
		pr_err("%s:driver registration failed, error=%d\n",	__func__, err);

	return err;
}
/*!
 * ISL79987 cleanup function
 *
 * @return  Error code indicating success or failure
 */

static void __exit isl79987_module_exit(void)
{
	i2c_del_driver(&isl79987_i2c_driver);
}

module_init(isl79987_module_init);
module_exit(isl79987_module_exit);

MODULE_AUTHOR("Brian Kang bkang@intersil.com");
MODULE_DESCRIPTION("ISL79987 Video Decoder Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.4");
MODULE_ALIAS("CSI");
