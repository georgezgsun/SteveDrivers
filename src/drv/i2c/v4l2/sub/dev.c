
/*---------------------------
 * dev.c
 *  Created on: Apr 25, 2019
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
#include "./camera/00export.h"
#include "./camera/camera.h"
/**************************************************/
#include <linux/kernel.h>
u32 sub_get_clk_rate(u32 mclk);
u32 sub_get_clk_rate(u32 mclk)
{
	u32 tgt_xclk;	/* target xclk */

	tgt_xclk =  mclk;
	tgt_xclk = min_t(u32, tgt_xclk, (u32)CAMERA_XCLK_MAX);
	tgt_xclk = max_t(u32, tgt_xclk, (u32)CAMERA_XCLK_MIN);

	return tgt_xclk;
}
/**************************************************/
int sub_init_camera(struct i2c_client_data *sensor);
int sub_init_camera(struct i2c_client_data *sensor)
{
	int ret;

	sensor->on = true;

	ret = camera_init_mode(sensor);
	return ret;
}
/**************************************************/
void sub_ini_pix_size(struct i2c_client_data *sensor);
void sub_ini_pix_size(struct i2c_client_data *sensor)
{

	sensor->pix.width = 640;
	sensor->pix.height =  480;
}
/**************************************************/
#include "./camera/fps.h"
void sub_ini_strm_cap_fps(struct i2c_client_data *sensor);
void sub_ini_strm_cap_fps(struct i2c_client_data *sensor)
{
	sensor->streamcap.timeperframe.denominator = DEFAULT_FPS;
	sensor->streamcap.timeperframe.numerator = 1;
}

/**************************************************/
int sub_stream_off(struct i2c_client_data *sensor);
int sub_stream_off(struct i2c_client_data *sensor)
{
	camera_stream_off(sensor);
	return 0;
}
/**************************************************/
int sub_stream_on(struct i2c_client_data *sensor);
int sub_stream_on(struct i2c_client_data *sensor)
{
	camera_stream_on(sensor);
	return 0;
}
/**************************************************/
/* end of dev.c */
