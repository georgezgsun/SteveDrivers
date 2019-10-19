
/*---------------------------
 * mode.h
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

#ifndef CAMERA_MODE_H_
#define CAMERA_MODE_H_

#include "01import.h"
/**************************************************/
/* image size under 1280 * 960 are SUBSAMPLING
 * image size upper 1280 * 960 are SCALING
 */
enum csi_downsize_mode {
	SUBSAMPLING,
	SCALING,
};

enum csi_mode {
	csi_mode_MIN = 0,
	csi_mode_VGA_640_480 = 0,
	csi_mode_NTSC_720_480 = 1,
	csi_mode_720P_1280_720 = 2,
	csi_mode_1080P_1920_1080 = 3,
	csi_mode_QSXGA_2592_1944 = 4,
	csi_mode_MAX = 5,
	csi_mode_INIT = 0xff, /*only for sensor init*/
};

/**************************************************/
#include "reg.h"
#include "rate.h"
struct csi_mode_info {
	enum csi_mode mode;
	enum csi_downsize_mode dn_mode;
	u32 width;
	u32 height;
};

extern int csi_mod_enum_framesizes(struct v4l2_subdev_frame_size_enum  *fse);
extern int csi_mod_enum_frameintervals(struct v4l2_subdev_frame_interval_enum *fie);

/**************************************************/
#endif 
/* end of mode.h */
