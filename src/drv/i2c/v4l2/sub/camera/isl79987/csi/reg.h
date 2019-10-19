
/*---------------------------
 * reg.h
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

#ifndef CAMERA_REG_H_
#define CAMERA_REG_H_

#include "01import.h"
/**************************************************/
struct reg_value {
    u8 reg;
    u8 value;
};

extern int ReadTW88(struct i2c_client_data *sensor,u8 reg);
extern int WriteTW88(struct i2c_client_data *sensor,u8 reg, u8 val);
extern int WriteTW88S(struct i2c_client_data *sensor,struct reg_value *config, int size);

extern int	csi_write_reg(struct i2c_client_data *sensor,u16 reg, u8 val);
extern int	csi_read_reg(struct i2c_client_data *sensor,u16 reg, u8 *val);
/**************************************************/
#endif 
/* end of reg.h */
