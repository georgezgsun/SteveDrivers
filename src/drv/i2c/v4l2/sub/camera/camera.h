
/*---------------------------
 * camera.h
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

#ifndef CAMERA_CAMERA_H_
#define CAMERA_CAMERA_H_

#include "01import.h"
/**************************************************/
#include "00export.h"
/**************************************************/

extern void camera_stream_on(struct i2c_client_data *sensor);
extern void camera_stream_off(struct i2c_client_data *sensor);

extern int camera_init_mode(struct i2c_client_data *sensor);
extern int camera_check_chip_id(struct i2c_client_data *sensor);
extern int camera_check_slave_id(struct i2c_client_data *sensor);
/**************************************************/
#endif 
/* end of camera.h */
