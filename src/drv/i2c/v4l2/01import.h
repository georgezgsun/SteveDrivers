
/*---------------------------
 * 01import.h
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

#ifndef SRC_DRV_I2C_V4L2_01IMPORT_H_
#define SRC_DRV_I2C_V4L2_01IMPORT_H_

#include "../01import.h"
/**************************************************/
#include <linux/v4l2-mediabus.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ctrls.h>
extern	s32 i2c_write_reg(struct i2c_client *i2c_client, u8 reg, u8 val);
extern	s32 i2c_read_reg(struct i2c_client *i2c_client, u8 reg, u8 *val);
/**************************************************/
#endif 
/* end of 01import.h */
