
/*---------------------------
 * fmt.h
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

#ifndef CSI_FMT_VIDEO_H_
#define CSI_FMT_VIDEO_H_

#include "01import.h"
/**************************************************/
/*! List of input video formats supported.
 * The video formats is corresponding with v4l2 id in video_fmt_t
 */
typedef enum {
    isl79987_NTSC = 0,      /*!< Locked on (M) NTSC video signal. */
    isl79987_PAL,           /*!< (B, G, H, I, N)PAL video signal. */
    isl79987_NOT_LOCKED,    /*!< Not locked on a signal. */
} video_fmt_idx;
/*! Number of video standards supported (including 'not locked' signal). */
#define isl79987_STD_MAX      (isl79987_PAL + 1)

/*! Video format structure. */
typedef struct {
    int v4l2_id;        /*!< Video for linux ID. */
    char name[16];      /*!< Name (e.g., "NTSC", "PAL", etc.) */
    u16 raw_width;      /*!< Raw width. (total width) */
    u16 raw_height;     /*!< Raw height. (total width) */
    u16 active_width;   /*!< Active width. */
    u16 active_height;  /*!< Active height. */
	int frame_rate;		/*!< Frame rate. */
} video_fmt_t;
/**************************************************/
#endif 
/* end of fmt.h */
