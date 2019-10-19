
/*---------------------------
 * hus.c
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
#include "hus.h"
/**************************************************/
static struct csi_hs_info hs_setting[] = {
	{2592, 1944, 30, 0x0B},
	{2592, 1944, 15, 0x10},

	{1920, 1080, 30, 0x0B},
	{1920, 1080, 15, 0x10},

	{1280, 720,  30, 0x11},
	{1280, 720,  15, 0x16},

	{720,  480,  30, 0x1E},
	{720,  480,  15, 0x23},

	{640,  480,  30, 0x1E},
	{640,  480,  15, 0x23},

	{320,  240,  30, 0x1E},
	{320,  240,  15, 0x23},
};

/**************************************************/
uint16_t csi_hus_find_configure(struct i2c_client_data *sensor);
uint16_t csi_hus_find_configure(struct i2c_client_data *sensor)
{
	struct v4l2_fract *timeperframe = &sensor->streamcap.timeperframe;
	struct v4l2_pix_format *pix = &sensor->pix;
	u32 frame_rate = timeperframe->denominator / timeperframe->numerator;
	int i;

	for (i = 0; i < ARRAY_SIZE(hs_setting); i++) {
		if (hs_setting[i].width == pix->width &&
			hs_setting[i].height == pix->height &&
			hs_setting[i].frame_rate == frame_rate)
		  return hs_setting[i].val;
	}

	if (i == ARRAY_SIZE(hs_setting))
		os_print("%s can not find hs configure\n", __func__);

	return -EINVAL;
}
/**************************************************/
/* end of hus.c */
