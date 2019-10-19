
/*---------------------------
 * isl79987.h
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

#ifndef ISL79987_ISL79987_H_
#define ISL79987_ISL79987_H_

#include "01import.h"
/**************************************************/
extern int isl79987_check_slave_id(struct i2c_client_data *sensor);
extern int isl79987_check_chip_id(struct i2c_client_data *sensor);
extern void ISL79987_Reset(struct i2c_client_data *sensor,int flag_set);

extern int isl79987_update_sensor_data(struct i2c_client_data *sensor, v4l2_std_id *std);
extern int isl79987_init_chip_registers(struct i2c_client_data *sensor);
/**************************************************/
#endif 
/* end of isl79987.h */
