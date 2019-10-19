
/*---------------------------
 * reg.c
 *  Created on: Apr 30, 2019
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
#include "reg.h"
/**************************************************/
int ReadTW88(struct i2c_client_data *sensor,u8 reg);
int ReadTW88(struct i2c_client_data *sensor,u8 reg)
{
	int val;

	val = i2c_smbus_read_byte_data(sensor->i2c_client, reg);
	if (val < 0) {
		os_print("CSI:Read reg error: reg=%x\n", reg);
		return -1;
	}
	return val;
}

/**************************************************/
int WriteTW88(struct i2c_client_data *sensor,u8 reg, u8 val);
int WriteTW88(struct i2c_client_data *sensor,u8 reg, u8 val)
{
	int ret;

	ret = i2c_smbus_write_byte_data(sensor->i2c_client, reg, val);
	if (ret < 0) {
		os_print("CSI:Write reg error: reg=%x, val=%x\n", reg, val);
		return -1;
	}
	return 0;
}
/**************************************************/
int WriteTW88S(struct i2c_client_data *sensor,struct reg_value *config, int size);
int WriteTW88S(struct i2c_client_data *sensor,struct reg_value *config, int size)
{
    int i;
    for(i=0; i < size; i++)
        WriteTW88(sensor,config[i].reg, config[i].value);
    return 0;
}

/**************************************************/
int	csi_write_reg(struct i2c_client_data *sensor,u16 reg, u8 val);
int	csi_write_reg(struct i2c_client_data *sensor,u16 reg, u8 val)
{
	u8 au8Buf[3] = {0};

	au8Buf[0] = reg >> 8;
	au8Buf[1] = reg & 0xff;
	au8Buf[2] = val;

	if (i2c_master_send(sensor->i2c_client, au8Buf, 3) < 0) {
		os_print("CSI:Write reg error: reg=%x, val=%x\n", reg, val);
		return -1;
	}
	return 0;
}
/**************************************************/
int	csi_read_reg(struct i2c_client_data *sensor,u16 reg, u8 *val);
int	csi_read_reg(struct i2c_client_data *sensor,u16 reg, u8 *val)
{
	u8 au8RegBuf[2] = {0};
	u8 u8RdVal = 0;

	au8RegBuf[0] = reg >> 8;
	au8RegBuf[1] = reg & 0xff;

	if (i2c_master_send(sensor->i2c_client, au8RegBuf, 2) != 2) {
		os_print("CSI:Read reg error: reg=%x\n", reg);
		return -1;
	}

	if (i2c_master_recv(sensor->i2c_client, &u8RdVal, 1) != 1) {
		os_print("CSI:Read reg error: reg=%x, val=%x\n", reg, u8RdVal);
		return -1;
	}

	*val = u8RdVal;

	return u8RdVal;
}
/**************************************************/
#include <linux/delay.h>

/**************************************************/
/* end of reg.c */
