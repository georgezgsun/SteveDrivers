
/*---------------------------
 * mode.c
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
#include "reg.h"

#include "mod.h"
/**************************************************/
static struct csi_mode_info csi_mode_info_data[2][csi_mode_MAX + 1] = {
		{
			{csi_mode_VGA_640_480, -1, 0, 0},
			{csi_mode_NTSC_720_480, -1, 0, 0},
			{csi_mode_720P_1280_720, -1, 0, 0},
			{csi_mode_1080P_1920_1080, -1, 0, 0},
			{csi_mode_QSXGA_2592_1944, SCALING, 2592, 1944}
		},
		{
			{csi_mode_VGA_640_480, SUBSAMPLING, 640,  480,
			},
			{csi_mode_NTSC_720_480, SUBSAMPLING, 720, 480,
			},
			{csi_mode_720P_1280_720, SUBSAMPLING, 1280, 720,
			},
			{csi_mode_1080P_1920_1080, SCALING, 1920, 1080,
			},
			{csi_mode_QSXGA_2592_1944, -1, 0, 0},
		},
	};

/**************************************************/
#include <linux/kernel.h>
int csi_mod_enum_framesizes(struct v4l2_subdev_frame_size_enum  *fse);
int csi_mod_enum_framesizes(struct v4l2_subdev_frame_size_enum  *fse)
{
	if (fse->index > csi_mode_MAX)
		return -EINVAL;

	fse->max_width =
			max(csi_mode_info_data[0][fse->index].width,
			    csi_mode_info_data[1][fse->index].width);
	fse->min_width = fse->max_width;

	fse->max_height =
			max(csi_mode_info_data[0][fse->index].height,
			    csi_mode_info_data[1][fse->index].height);
	fse->min_height = fse->max_height;
	return 0;
}

/**************************************************/
#include "rate.h"
static int csi_framerates[] = {
	[csi_15_fps] = 15,
	[csi_30_fps] = 30,
};
int csi_mod_enum_frameintervals(struct v4l2_subdev_frame_interval_enum *fie);
int csi_mod_enum_frameintervals(struct v4l2_subdev_frame_interval_enum *fie)
{
	int i, j, count;
	if (fie->index < 0)
		return -EINVAL;
	if (fie->index > csi_mode_MAX)
		return -EINVAL;
	if (fie->width == 0)
		return -EINVAL;
	if (fie->height == 0)
		return -EINVAL;
	if (fie->code == 0)
		return -EINVAL;

	count = 0;
	for (i = 0; i < ARRAY_SIZE(csi_framerates); i++) {
		for (j = 0; j < (csi_mode_MAX + 1); j++) {
			if (fie->width == csi_mode_info_data[i][j].width
			 && fie->height == csi_mode_info_data[i][j].height) {
				count++;
			}
			if (fie->index == (count - 1)) {
				fie->interval.denominator = csi_framerates[i];
				return 0;
			}
		}
	}
	return -EINVAL;
}
/**************************************************/
/* end of mode.c */
