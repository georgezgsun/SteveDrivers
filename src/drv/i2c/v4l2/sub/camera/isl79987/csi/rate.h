
/*---------------------------
 * rate.h
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

#ifndef CSI_RATE_H_
#define CSI_RATE_H_


/**************************************************/
enum csi_frame_rate {
	csi_15_fps,
	csi_30_fps
};

extern enum csi_frame_rate csi_rate_find_frame_rate(struct v4l2_fract *timeperframe);
/**************************************************/
#endif 
/* end of rate.h */
