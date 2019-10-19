
/*---------------------------
 * ops.c
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
#include "./camera/00export.h"
#include "./camera/camera.h"

/**************************************************/
//static struct v4l2_subdev_core_ops v4l2_subdev_core_ops = {
//	.s_power	= sub_s_power,
//};

/**************************************************/
int sub_core_s_power(struct v4l2_subdev *sd, int on);
int sub_core_s_power(struct v4l2_subdev *sd, int on)
{
	struct i2c_client *client;
	struct i2c_client_data *sensor;
	client = v4l2_get_subdevdata(sd);
	sensor=i2c_data_get_client_data(client);

	os_print("I2C sub ops: %s\n",__func__);
//	camera_s_power(sensor, on);

	if (on && !sensor->on)
		i2c_enable_regulater(sensor);
	else
		i2c_disable_regulater(sensor);

	sensor->on = on;
	return 0;
}
/**************************************************/
//static struct v4l2_subdev_video_ops v4l2_subdev_video_ops = {
//	.g_parm = sub_g_parm,
//	.s_parm = sub_s_parm,
//	.s_stream = sub_s_stream,
//};
/**************************************************/
int sub_video_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *a);
int sub_video_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *a)
{

	struct i2c_client *client;
	struct i2c_client_data *sensor;
	struct v4l2_captureparm *cparm;
	int ret = 0;

	client = v4l2_get_subdevdata(sd);
	sensor=i2c_data_get_client_data(client);
	cparm= &a->parm.capture;

	os_print("I2C sub ops: %s\n",__func__);
	switch (a->type) {
	/* This is the only case currently handled. */
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
	case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
		memset(a, 0, sizeof(*a));
		a->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		cparm->capability = sensor->streamcap.capability;
		cparm->timeperframe = sensor->streamcap.timeperframe;
		cparm->capturemode = sensor->streamcap.capturemode;
		ret = 0;
		break;

	/* These are all the possible cases. */
	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
	case V4L2_BUF_TYPE_VIDEO_OVERLAY:
	case V4L2_BUF_TYPE_VBI_CAPTURE:
	case V4L2_BUF_TYPE_VBI_OUTPUT:
	case V4L2_BUF_TYPE_SLICED_VBI_CAPTURE:
	case V4L2_BUF_TYPE_SLICED_VBI_OUTPUT:
		ret = -EINVAL;
		break;

	default:
		os_print("I2C Ops: type is unknown - %d\n", a->type);
		ret = -EINVAL;
		break;
	}

	return ret;
}

/**************************************************/
#include "./camera/fps.h"
int sub_video_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *a);
int sub_video_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *a)
{
	struct i2c_client *client;
	struct i2c_client_data *sensor;
	struct v4l2_fract *timeperframe;
	enum csi_mode mode,orig_mode;
	enum csi_frame_rate frame_rate;
	u32 tgt_fps;	/* target frames per secound */
	int ret = 0;

	os_print("I2C sub ops: %s %d\n",__func__,__LINE__);
	client = v4l2_get_subdevdata(sd);
	sensor=i2c_data_get_client_data(client);
	timeperframe = &a->parm.capture.timeperframe;
	mode = a->parm.capture.capturemode;
	os_print("I2C sub ops: %s %d\n",__func__,__LINE__);
	switch (a->type) {
	/* This is the only case currently handled. */
	case V4L2_BUF_TYPE_VIDEO_CAPTURE:
	case V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
		/* Check that the new frame rate is allowed. */
		if ((timeperframe->numerator == 0) ||
		    (timeperframe->denominator == 0)) {
			timeperframe->denominator = DEFAULT_FPS;
			timeperframe->numerator = 1;
		}

		tgt_fps = timeperframe->denominator /
			  timeperframe->numerator;

		if (tgt_fps > MAX_FPS) {
			timeperframe->denominator = MAX_FPS;
			timeperframe->numerator = 1;
		} else if (tgt_fps < MIN_FPS) {
			timeperframe->denominator = MIN_FPS;
			timeperframe->numerator = 1;
		}

		/* Actual frame rate we use */
		tgt_fps = timeperframe->denominator /
			  timeperframe->numerator;

		if (tgt_fps == 15)
			frame_rate = csi_15_fps;
		else if (tgt_fps == 30)
			frame_rate = csi_30_fps;
		else {
			os_print("SUB:The camera frame rate is not supported!\n");
			return -EINVAL;
		}

		if (mode > csi_mode_MAX || mode < csi_mode_MIN) {
			os_print("SUB:The camera mode[%d] is not supported!\n", mode);
			return -EINVAL;
		}

		orig_mode = sensor->streamcap.capturemode;
		ret = camera_init_mode(sensor);

		if (ret < 0)
			return ret;
		sensor->streamcap.capturemode = (u32)a->parm.capture.capturemode;
		sensor->streamcap.timeperframe = *timeperframe;
		break;

	/* These are all the possible cases. */
	case V4L2_BUF_TYPE_VIDEO_OUTPUT:
	case V4L2_BUF_TYPE_VIDEO_OVERLAY:
	case V4L2_BUF_TYPE_VBI_CAPTURE:
	case V4L2_BUF_TYPE_VBI_OUTPUT:
	case V4L2_BUF_TYPE_SLICED_VBI_CAPTURE:
	case V4L2_BUF_TYPE_SLICED_VBI_OUTPUT:
		os_print("SUB:type is not V4L2_BUF_TYPE_VIDEO_CAPTURE but %d\n",a->type);
		ret = -EINVAL;
		break;

	default:
		os_print("SUB:type is unknown - %d\n", a->type);
		ret = -EINVAL;
		break;
	}
	os_print("I2C sub ops: %s %d\n",__func__,__LINE__);
	return ret;
}

/**************************************************/
int sub_video_s_stream(struct v4l2_subdev *sd, int enable);
int sub_video_s_stream(struct v4l2_subdev *sd, int enable)
{

	struct i2c_client *client;
	struct i2c_client_data *sensor;

	os_print("I2C sub ops: %s\n",__func__);
	client = v4l2_get_subdevdata(sd);
	sensor = i2c_data_get_client_data(client);

	if (enable)
		camera_stream_on(sensor);
	else
		camera_stream_off(sensor);
	return 0;		//dummy
}
/**************************************************/
//static const struct v4l2_subdev_pad_ops v4l2_subdev_pad_ops = {
//	.enum_frame_size       = sub_enum_framesizes,
//	.enum_frame_interval   = sub_enum_frameintervals,
//	.enum_mbus_code        = sub_enum_code,
//	.set_fmt               = sub_set_fmt,
//	.get_fmt               = sub_get_fmt,
//};
/**************************************************/
int sub_pad_enum_framesizes(struct v4l2_subdev *sd,
		struct v4l2_subdev_pad_config *cfg,
		struct v4l2_subdev_frame_size_enum  *fse);
int sub_pad_enum_framesizes(struct v4l2_subdev *sd,
		struct v4l2_subdev_pad_config *cfg,
		struct v4l2_subdev_frame_size_enum  *fse)
{
	os_print("I2C sub ops: %s\n",__func__);

	return csi_mod_enum_framesizes(fse);

}
/**************************************************/
int sub_pad_enum_frameintervals(struct v4l2_subdev *sd,
		struct v4l2_subdev_pad_config *cfg,
		struct v4l2_subdev_frame_interval_enum *fie);
int sub_pad_enum_frameintervals(struct v4l2_subdev *sd,
		struct v4l2_subdev_pad_config *cfg,
		struct v4l2_subdev_frame_interval_enum *fie)
{
	os_print("I2C sub ops: %s\n",__func__);

	fie->interval.numerator = 1;
	return csi_mod_enum_frameintervals(fie);
}

/**************************************************/
int sub_pad_enum_mbuscode(struct v4l2_subdev *sd,
	      struct v4l2_subdev_pad_config *cfg,
	      struct v4l2_subdev_mbus_code_enum *code);
int sub_pad_enum_mbuscode(struct v4l2_subdev *sd,
	      struct v4l2_subdev_pad_config *cfg,
	      struct v4l2_subdev_mbus_code_enum *code)
{
	os_print("I2C sub ops: %s\n",__func__);

	return csi_fmt_enum_mbus_code(code);
}
/**************************************************/
int sub_pad_set_fmt(struct v4l2_subdev *sd,
	       struct v4l2_subdev_pad_config *cfg,
	       struct v4l2_subdev_format *format);
int sub_pad_set_fmt(struct v4l2_subdev *sd,
	       struct v4l2_subdev_pad_config *cfg,
	       struct v4l2_subdev_format *format)
{
	struct i2c_client *client;
	struct i2c_client_data *sensor;
	struct v4l2_mbus_framefmt *mf;
	const struct csi_datafmt *fmt;
	int capturemode;

	os_print("I2C sub ops: %s\n",__func__);
	if (format->which == V4L2_SUBDEV_FORMAT_TRY)
		return 0;

	client = v4l2_get_subdevdata(sd);
	sensor = i2c_data_get_client_data(client);
	mf = &format->format;

	mf->field	= V4L2_FIELD_NONE;

	fmt=csi_fmt_find_datafmt(mf->code);
	if (!fmt)
		csi_fmt_ini_mbus_code(mf);

	sensor->fmt = fmt;
	capturemode = csi_get_res_mode(mf->width, mf->height);
	if (capturemode >= 0) {
		sensor->streamcap.capturemode = capturemode;
		sensor->pix.width = mf->width;
		sensor->pix.height = mf->height;
		return 0;
	}

	os_print("SUB:Set format failed %d, %d\n", fmt->code, fmt->colorspace);
	return -EINVAL;
}
/**************************************************/
int sub_pad_get_fmt(struct v4l2_subdev *sd,
	       struct v4l2_subdev_pad_config *cfg,
	       struct v4l2_subdev_format *format);
int sub_pad_get_fmt(struct v4l2_subdev *sd,
	       struct v4l2_subdev_pad_config *cfg,
	       struct v4l2_subdev_format *format)
{
	struct i2c_client *client;
	struct i2c_client_data *sensor;
	struct v4l2_mbus_framefmt *mf;
	const struct csi_datafmt *fmt;

	os_print("I2C sub ops: %s\n",__func__);
	client = v4l2_get_subdevdata(sd);
	sensor=i2c_data_get_client_data(client);
	mf = &format->format;
	fmt = sensor->fmt;

	if (format->pad)
		return -EINVAL;

	memset(mf, 0, sizeof(struct v4l2_mbus_framefmt));

	mf->code	= fmt->code;
	mf->colorspace	= fmt->colorspace;
	mf->field	= V4L2_FIELD_NONE;

	mf->width	= sensor->pix.width;
	mf->height	= sensor->pix.height;

	os_print("SUB:%s code=0x%x, w/h=(%d,%d), colorspace=%d, field=%d\n",
		__func__, mf->code, mf->width, mf->height, mf->colorspace, mf->field);

	return 0;
}

/**************************************************/
/* end of ops.c */
