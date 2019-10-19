
/*---------------------------
 * isl79987.c
 *  Created on: Apr 26, 2019
 *      Author: Steve Rui
 *---------------------------
 *
 *
 *
 *
 *
 *---------------------------
 */

#include "01import.h"
/**************************************************/
#include "00export.h"

/*!
* ISL79987 I2C Slave address (7bit)
*/
#define ISL79987_I2C_ID        0x44

/**************************************************/
//check I2C slave id and communication
int isl79987_check_slave_id(struct i2c_client_data *sensor);
int isl79987_check_slave_id(struct i2c_client_data *sensor)
{
	int ret,ret1;
	u8 val;
	ret=WriteTW88(sensor, 0xff,	0x00);
	ret1=ReadTW88(sensor,0x00);

	ret=i2c_write_reg(sensor->i2c_client,0xff,00);
	ret1=i2c_read_reg(sensor->i2c_client,00,&val);

	ret |= ret1;
	if(ret<0)
	{
		os_print("CAMERA: I2C slave device wrong!\n");
		return -ENODEV;
	}
	return 0;
}
/**************************************************/
int isl79987_check_chip_id(struct i2c_client_data *sensor);
int isl79987_check_chip_id(struct i2c_client_data *sensor)
{
	int ret;
	u8 chip_id;

	WriteTW88(sensor, 0xff,	0x00);
	ret = ReadTW88(sensor,0x00);
	if ( ret <0 ) return -ENODEV;
	chip_id = ret&0xFF;
	if ( chip_id != 0x87 ) return -ENODEV;
	return 0;
}
/**************************************************/
/*!
 * reset MIPI
 */
void ISL79987_Reset(struct i2c_client_data *sensor,int flag_set)
{
    WriteTW88(sensor,0xff, 0);
    if(flag_set==1)
        WriteTW88(sensor,0x02, 0x1F);  /* set Reset */
    else if(flag_set==2)
        WriteTW88(sensor,0x02, 0x10); /* clear Reset_CH1 ~ Reset_CH4 */
    else
        WriteTW88(sensor,0x02, 0x00); /* clear reset */
}

/**************************************************/
/*!
 * update sensor data
 * caller ioctl_g_std, ioctl_querystd
 */
int isl79987_update_sensor_data(struct i2c_client_data *sensor, v4l2_std_id *std);
int isl79987_update_sensor_data(struct i2c_client_data *sensor, v4l2_std_id *std)
{
    if(std==V4L2_STD_UNKNOWN)
        return -EINVAL;

    sensor->std_id = *std;

    if(*std == V4L2_STD_PAL) {
        ISL79987_Reset(sensor, 1);
        csi_dec_ini_PAL(sensor);
        ISL79987_Reset(sensor, 0);

        sensor->pix.width = 720;
        sensor->pix.height = 576;
        sensor->streamcap.timeperframe.denominator = 25;
        sensor->pix.sizeimage = 720 * 2 * 576;
    }
    else {
        ISL79987_Reset(sensor, 1);
        csi_dec_ini_NTSC(sensor);
        ISL79987_Reset(sensor, 0);

        sensor->pix.width = 720;
        sensor->pix.height = 480;
        sensor->streamcap.timeperframe.denominator = 30;
        sensor->pix.sizeimage = 720 * 2 * 480;
    }
     return 0;
}

/**************************************************/
static	void init_pseudo(struct i2c_client_data *sensor);
static	void init_pseudo(struct i2c_client_data *sensor)
{
	int lines;

    if(sensor->pseudo)
    {
		/* first, remove FrameMode; REG501[5] */
        WriteTW88(sensor,0xFF, 5);
		WriteTW88(sensor,0x01, ReadTW88(sensor,0x01) & ~0x20);

        lines = sensor->pseudo;
        if(sensor->std_id==V4L2_STD_PAL)    lines *= 288;
        else                                lines *= 240;
        if(sensor->histo) { /* ?1 */
            lines += sensor->pseudo;
            WriteTW88(sensor,0xff, 0xf);
            WriteTW88(sensor,0xE7, 0x01);
        }

        WriteTW88(sensor,0xFF, 5);
        WriteTW88(sensor,0x06, 0x61);
        switch(sensor->nvc) {
        case 0:
            WriteTW88(sensor,0x04, 0x00); /* or use REG506[3]=1 */
            break;
        case 1:
            WriteTW88(sensor,0x04, 0x55);
            break;
        case 2:
            WriteTW88(sensor,0x04, 0xAA);
            break;
        case 3:
            WriteTW88(sensor,0x04, 0xFF);
            break;
        default:
            WriteTW88(sensor,0x04, 0x00);
            break;
        }

        WriteTW88(sensor,0x38, lines >> 8);
        WriteTW88(sensor,0x39, lines);
    }
}
/**************************************************/
static void init_pixelformat(struct i2c_client_data *sensor);
static void init_pixelformat(struct i2c_client_data *sensor)
{
	   if (sensor->pix.pixelformat == V4L2_PIX_FMT_RGB565) {
	        WriteTW88(sensor,0xFF, sensor->v_channel+1);
	        WriteTW88(sensor,0x3D, ReadTW88(sensor,0x3D) |  0x01);
	        WriteTW88(sensor,0xFF, 0x05);
	        WriteTW88(sensor,0x05, ReadTW88(sensor,0x05) |  0x10);
	    }
	    else {
	        WriteTW88(sensor,0xFF, sensor->v_channel+1);
	        WriteTW88(sensor,0x3D, ReadTW88(sensor,0x3D) & ~0x01);
	        WriteTW88(sensor,0xFF, 0x05);
	        WriteTW88(sensor,0x05, ReadTW88(sensor,0x05) & ~0x10);
	    }

}
/**************************************************/
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
int isl79987_init_chip_registers(struct i2c_client_data *sensor);
int isl79987_init_chip_registers(struct i2c_client_data *sensor)
{
    if(sensor==NULL) {
        os_print("ISL:download isl79987 default regs.");
        WriteTW88(sensor,0xff, 5);         /* clear PowerDown */
        WriteTW88(sensor,0x00, ReadTW88(sensor,0x00) & ~0x80);
        ISL79987_Reset(sensor,1);          /* set Reset all */
        csi_lane_write_channel(sensor,0,0);		//default
        ISL79987_Reset(sensor,2);          /* clear Reset_CH1 ~ Reset_CH4 */
        csi_dec_write_decorder(sensor);
        csi_lane_write_channel(sensor,4,2);
        ISL79987_Reset(sensor,0);          /* clear Reset */
        return 0;
    }
    if(sensor->is_dummy)
        return 1;

    ISL79987_Reset(sensor,1);          /* set Reset all */
    csi_lane_write_channel(sensor,0,0);		//default
    ISL79987_Reset(sensor,2);          /* clear Reset_CH1 ~ Reset_CH4 */
    csi_dec_write_decorder(sensor);
    csi_lane_write_channel(sensor,sensor->max_sensor,sensor->lanes);
    init_pseudo(sensor);
    init_pixelformat(sensor);
    ISL79987_Reset(sensor,0);  /* clear Reset */
    return 0;
}

/**************************************************/
/* end of isl79987.c */
