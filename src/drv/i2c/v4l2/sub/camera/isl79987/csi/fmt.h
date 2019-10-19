
/*---------------------------
 * fmt.h
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

#ifndef CSI_FMT_H_
#define CSI_FMT_H_

#include "../01import.h"
/**************************************************/
struct csi_datafmt {
	u32	code;
	enum v4l2_colorspace		colorspace;
};

extern const struct csi_datafmt *csi_fmt_find_datafmt(u32 code);
extern int csi_fmt_enum_mbus_code(struct v4l2_subdev_mbus_code_enum *code);
extern void csi_fmt_ini_mbus_code(struct v4l2_mbus_framefmt *mf);
/**************************************************/
#endif 
/* end of fmt.h */
