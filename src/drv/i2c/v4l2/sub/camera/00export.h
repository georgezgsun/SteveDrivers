
/*---------------------------
 * 00export.h
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

#ifndef CAMERA_00EXPORT_H_
#define CAMERA_00EXPORT_H_

#include "./isl79987/00export.h"
/**************************************************/
extern int camera_s_power(struct v4l2_subdev *sd, int on);

extern int camera_set_fmt(struct v4l2_subdev *sd,
			  struct v4l2_subdev_pad_config *cfg,
			  struct v4l2_subdev_format *format);
extern int camera_get_fmt(struct v4l2_subdev *sd,
			  struct v4l2_subdev_pad_config *cfg,
			  struct v4l2_subdev_format *format);
extern int camera_enum_code(struct v4l2_subdev *sd,
			    struct v4l2_subdev_pad_config *cfg,
			    struct v4l2_subdev_mbus_code_enum *code);
extern int camera_enum_framesizes(struct v4l2_subdev *sd,
			       struct v4l2_subdev_pad_config *cfg,
			       struct v4l2_subdev_frame_size_enum *fse);
extern int camera_enum_frameintervals(struct v4l2_subdev *sd,
		struct v4l2_subdev_pad_config *cfg,
		struct v4l2_subdev_frame_interval_enum *fie);

extern int camera_g_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *a);
extern int camera_s_parm(struct v4l2_subdev *sd, struct v4l2_streamparm *a);
extern int camera_s_stream(struct v4l2_subdev *sd, int enable);
/**************************************************/
#endif 
/* end of 00export.h */
