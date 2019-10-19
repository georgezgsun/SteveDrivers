
/*---------------------------
 * rate.c
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
#include "rate.h"

/**************************************************/
enum csi_frame_rate csi_rate_find_frame_rate(struct v4l2_fract *timeperframe);
enum csi_frame_rate csi_rate_find_frame_rate(struct v4l2_fract *timeperframe)
{
	enum csi_frame_rate rate;
	u32 tgt_fps;	/* target frames per secound */

	tgt_fps = timeperframe->denominator / timeperframe->numerator;

	if (tgt_fps == 30)
		rate = csi_30_fps;
	else if (tgt_fps == 15)
		rate = csi_15_fps;
	else
		rate = -EINVAL;

	return rate;
}
/**************************************************/
/* end of rate.c */
