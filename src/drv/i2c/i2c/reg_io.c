
/*---------------------------
 * reg_io.c
 *  Created on: Apr 24, 2019
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
s32 i2c_write_reg(struct i2c_client *i2c_client, u8 reg, u8 val);
s32 i2c_write_reg(struct i2c_client *i2c_client, u8 reg, u8 val)
{
	u8 au8Buf[2] = {0};

	au8Buf[0] = reg;
	au8Buf[1] = val;

	if (i2c_master_send(i2c_client, au8Buf, 2) != 2) {
		os_print("I2C:Write reg error: reg=%x, val=%x\n", reg, val);
		return -1;
	}

	return 0;
}

/**************************************************/
s32 i2c_read_reg(struct i2c_client *i2c_client, u8 reg, u8 *val);
s32 i2c_read_reg(struct i2c_client *i2c_client, u8 reg, u8 *val)
{
	int ret;
	u8 au8RegBuf[2] = {0};
	u8 u8RdVal = 0;

	au8RegBuf[0] = reg ;

	if (i2c_master_send(i2c_client, au8RegBuf, 1) != 1) {
		os_print("I2C:Read reg error: reg=%x\n", reg);
		return -1;
	}

	if (i2c_master_recv(i2c_client, &u8RdVal, 1) != 1) {
		os_print("I2C:Read reg error: reg=%x, val=%x\n", reg, u8RdVal);
		return -1;
	}

	*val = u8RdVal;

	ret =u8RdVal;
	ret &= 0xff;
	return ret;
}

/**************************************************/
/* end of reg_io.c */
