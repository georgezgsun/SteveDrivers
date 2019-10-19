
/*---------------------------
 * register.c
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
extern int sub_core_s_power(struct v4l2_subdev *sd, int on);
static struct v4l2_subdev_core_ops sub_fun_core_ops = {
	.s_power	= sub_core_s_power,
};

/**************************************************/
extern int sub_video_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *param);
extern int sub_video_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *param);
extern int sub_video_s_stream(struct v4l2_subdev *sd, int enable);
static struct v4l2_subdev_video_ops sub_fun_video_ops = {
	.g_parm = sub_video_g_parm,
	.s_parm = sub_video_s_parm,
	.s_stream = sub_video_s_stream,
};

/**************************************************/
static struct v4l2_subdev_tuner_ops	sub_fun_tuner_ops = {

};


/**************************************************/
static struct v4l2_subdev_audio_ops sub_fun_audio_ops ={

};


/**************************************************/
static struct v4l2_subdev_vbi_ops sub_fun_vbi_ops = {

};

/**************************************************/
static struct v4l2_subdev_ir_ops sub_fun_ir_ops={

};

/**************************************************/
static struct v4l2_subdev_sensor_ops sub_fun_sensor_ops={

};


/**************************************************/
extern int sub_pad_enum_framesizes(struct v4l2_subdev *sd,
	       struct v4l2_subdev_pad_config *cfg,
	       struct v4l2_subdev_frame_size_enum *fse);
extern int sub_pad_enum_frameintervals(struct v4l2_subdev *sd,
		struct v4l2_subdev_pad_config *cfg,
		struct v4l2_subdev_frame_interval_enum *fie);
extern int sub_pad_enum_mbuscode(struct v4l2_subdev *sd,
	      struct v4l2_subdev_pad_config *cfg,
	      struct v4l2_subdev_mbus_code_enum *code);
extern int sub_pad_set_fmt(struct v4l2_subdev *sd,
	       struct v4l2_subdev_pad_config *cfg,
	       struct v4l2_subdev_format *format);
extern int sub_pad_get_fmt(struct v4l2_subdev *sd,
	       struct v4l2_subdev_pad_config *cfg,
	       struct v4l2_subdev_format *format);
static const struct v4l2_subdev_pad_ops sub_fun_pad_ops = {
	.enum_frame_size       = sub_pad_enum_framesizes,
	.enum_frame_interval   = sub_pad_enum_frameintervals,
	.enum_mbus_code        = sub_pad_enum_mbuscode,
	.set_fmt               = sub_pad_set_fmt,
	.get_fmt               = sub_pad_get_fmt,
};

/**************************************************/
static struct v4l2_subdev_ops sub_fun_ops = {
	.core	= &sub_fun_core_ops,
	.tuner  = &sub_fun_tuner_ops,
	.audio  = &sub_fun_audio_ops,
	.video	= &sub_fun_video_ops,
	.vbi	= &sub_fun_vbi_ops,
	.ir		= &sub_fun_ir_ops,
	.sensor = &sub_fun_sensor_ops,
	.pad	= &sub_fun_pad_ops,
};

/**************************************************/
static int media_entity_link_setup(struct media_entity *entity,
			   const struct media_pad *local,
			   const struct media_pad *remote, u32 flags)
{
	return 0;
}
static const struct media_entity_operations v4l2_sd_media_ops = {
	.link_setup = media_entity_link_setup,
};

/**************************************************/

extern void sub_ini_pix_size(struct i2c_client_data *sensor);
extern void sub_ini_strm_cap_fps(struct i2c_client_data *sensor);
static void v4l2_ini_subdev_data(struct i2c_client_data *sensor);
static void v4l2_ini_subdev_data(struct i2c_client_data *sensor)
{
	sensor->pix.pixelformat = V4L2_PIX_FMT_UYVY;
	sub_ini_pix_size(sensor);
	sensor->streamcap.capability = V4L2_MODE_HIGHQUALITY |
					   V4L2_CAP_TIMEPERFRAME;
	sensor->streamcap.capturemode = 0;
	sub_ini_strm_cap_fps(sensor);
}

/**************************************************/
static int v4l2_find_csi_id(struct device *dev,struct i2c_client_data *sensor);
static int v4l2_find_csi_id(struct device *dev,struct i2c_client_data *sensor)
{
	int retval;

	retval = of_property_read_u32(dev->of_node, "csi_id",
					&(sensor->csi));
	return retval;

}
/**************************************************/
static int v4l2_ini_entity_pads(struct v4l2_subdev *sd,struct i2c_client_data *sensor);
static int v4l2_ini_entity_pads(struct v4l2_subdev *sd,struct i2c_client_data *sensor)
{
	int retval;
	sd->flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	sd->entity.function = MEDIA_ENT_F_CAM_SENSOR;
	sd->entity.ops = &v4l2_sd_media_ops;

	sensor->pads[CAMERA_SENS_PAD_SOURCE].flags = MEDIA_PAD_FL_SOURCE;

	retval = media_entity_pads_init(&sd->entity, CAMERA_SENS_PADS_NUM,
							sensor->pads);
	return retval;
}
static int v4l2_clr_entity_pads(struct v4l2_subdev *sd,struct i2c_client_data *sensor);
static int v4l2_clr_entity_pads(struct v4l2_subdev *sd,struct i2c_client_data *sensor)
{
	media_entity_cleanup(&sd->entity);
	return 0;
}

extern int camera_check_chip_id(struct i2c_client_data *sensor);
extern int camera_check_slave_id(struct i2c_client_data *sensor);
extern int sub_init_camera(struct i2c_client_data *sensor);
extern int sub_stream_off(struct i2c_client_data *sensor);
int v4l2_register_subdev(struct i2c_client *client,struct i2c_client_data *sensor);
int v4l2_register_subdev(struct i2c_client *client,struct i2c_client_data *sensor)
{
	int ret;
	struct device *dev;
	struct v4l2_subdev *sd;
	os_print("V4L2:is going to register sub dev!\n");
	dev = &client->dev;

	v4l2_ini_subdev_data(sensor);

	ret=v4l2_find_csi_id(dev,sensor);
	if(ret<0) goto csi_err;

	ret=camera_check_slave_id(sensor);
	if(ret<0) goto i2c_err;

	/* Set mclk rate before clk on */
//	ret=v4l2_set_clk_rate(dev,sensor);
	if(ret<0) goto clk_err;
//	ret = clk_prepare_enable(sensor->sensor_clk);
	if (ret < 0) goto clk_err;

	ret=camera_check_chip_id(sensor);
	if(ret<0) goto chip_err;

	i2c_gpio_reset_subdev(sensor);
	ret=sub_init_camera(sensor);
	if (ret < 0) goto subdev_err;

	sd = &sensor->subdev;
	v4l2_i2c_subdev_init(sd, client, &sub_fun_ops);

	ret=v4l2_ini_entity_pads(sd,sensor);
	if (ret < 0) goto pads_err;

	sensor->subdev.grp_id = 678;
	ret = v4l2_async_register_subdev(sd);
	if (ret < 0) goto reg_err;

	sub_stream_off(sensor);
	os_print("V4L2:Camera ISL79987 is found\n");
	return ret;

reg_err:
	v4l2_clr_entity_pads(sd,sensor);
	i2c_gpio_power_down(sensor);
	os_print("V4L2:subdev register failed: %d\n",ret);
	return ret;
pads_err:
	i2c_gpio_power_down(sensor);
	os_print("V4L2:v4l2 media entity pads init fail: %d\n",ret);
	return -ENODEV;
subdev_err:
	i2c_gpio_power_down(sensor);
	os_print("V4L2:SUBDEV init fail: %d\n",ret);
	return -ENODEV;
chip_err:
	os_print("V4L2:chip check fail: %d\n",ret);
	return -ENODEV;
clk_err:
	os_print("V4L2:mclk init fail: %d\n",ret);
	return ret;
i2c_err:
	os_print("V4L2:chip i2c address error: %d\n",ret);
	return -ENODEV;
csi_err:
	os_print("V4L2:CSI fail: %d\n",ret);
	return -ENODEV;
}

/**************************************************/
/* end of probe.c */
