
/*---------------------------
 * camera.c
 *  Created on: Apr 29, 2019
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
#define	custom_camera_name		"isl79987"
#include "00export.h"

/**************************************************/
void camera_stream_off(struct i2c_client_data *sensor);
void camera_stream_off(struct i2c_client_data *sensor)
{
}
/**************************************************/
void camera_stream_on(struct i2c_client_data *sensor);
void camera_stream_on(struct i2c_client_data *sensor)
{

}

/**************************************************/
int camera_check_slave_id(struct i2c_client_data *sensor);
int camera_check_slave_id(struct i2c_client_data *sensor)
{
	int ret;

	ret=isl79987_check_slave_id(sensor);

	return ret;
}
/**************************************************/
int camera_check_chip_id(struct i2c_client_data *sensor);
int camera_check_chip_id(struct i2c_client_data *sensor)
{
	int ret;

	ret=isl79987_check_chip_id(sensor);

	return ret;
}

/**************************************************/
int camera_init_mode(struct i2c_client_data *sensor);
int camera_init_mode(struct i2c_client_data *sensor)
{
	int ret;
	ret=isl79987_init_chip_registers(sensor);
	return ret;
}

/**************************************************/
/* end of camera.c */
