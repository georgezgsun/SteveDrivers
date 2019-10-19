
/*---------------------------
 * fmt.c
 *  Created on: May 1, 2019
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
#include "fmt_video.h"
/**************************************************/
/*! Description of video formats supported.
 *
 *  PAL:  raw=720x625, active=720x576.
 *  NTSC: raw=720x525, active=720x480.
 */
static video_fmt_t video_fmts[] = {
    {   /*! NTSC */
     .v4l2_id = V4L2_STD_NTSC,   .name = "NTSC",
     .raw_width = 720, .raw_height = 525,
     .active_width = 720, .active_height = 480,
   	 .frame_rate = 30,
     },
    {   /*! (B, G, H, I, N) PAL */
     .v4l2_id = V4L2_STD_PAL,    .name = "PAL",
     .raw_width = 720, .raw_height = 625,
     .active_width = 720, .active_height = 576,
   	 .frame_rate = 25,
     },
    {   /*! Unlocked standard */
     .v4l2_id = V4L2_STD_ALL,    .name = "Unlocked",
     .raw_width = 720, .raw_height = 525,
     .active_width = 720,   .active_height = 480,
   	 .frame_rate = 30,
     },
};
/**************************************************/
int	csi_fmt_get_raw_width(int i);
int	csi_fmt_get_raw_width(int i)
{
	return video_fmts[i].raw_width;
}
/**************************************************/
int	csi_fmt_get_raw_height(int i);
int	csi_fmt_get_raw_height(int i)
{
	return video_fmts[i].raw_height;
}
/**************************************************/
int	csi_fmt_get_active_width(int i);
int	csi_fmt_get_active_width(int i)
{
	return video_fmts[i].active_width;
}
/**************************************************/
int	csi_fmt_get_active_height(int i);
int	csi_fmt_get_active_height(int i)
{
	return video_fmts[i].active_height;
}

/**************************************************/
/* end of fmt.c */
