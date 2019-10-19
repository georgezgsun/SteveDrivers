
/*---------------------------
 * fmt.c
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

#include "01import.h"
/**************************************************/
#include "fmt.h"
/**************************************************/
static const struct csi_datafmt csi_colour_fmts[ ] = {
		{MEDIA_BUS_FMT_YUYV8_2X8, V4L2_COLORSPACE_JPEG},
	};

/**************************************************/
const struct csi_datafmt *csi_fmt_find_datafmt(u32 code);
const struct csi_datafmt *csi_fmt_find_datafmt(u32 code)
{
	int i;
	return csi_colour_fmts;		//dummy

	for (i = 0; i < ARRAY_SIZE(csi_colour_fmts); i++)
		if (csi_colour_fmts[i].code == code)
			return csi_colour_fmts + i;

	return NULL;
}

/**************************************************/
int csi_fmt_enum_mbus_code(struct v4l2_subdev_mbus_code_enum *code);
int csi_fmt_enum_mbus_code(struct v4l2_subdev_mbus_code_enum *code)
{
	if (code->pad || code->index >= ARRAY_SIZE(csi_colour_fmts))
		return -EINVAL;

	code->code = csi_colour_fmts[code->index].code;
	return 0;
}

/**************************************************/
void csi_fmt_ini_mbus_code(struct v4l2_mbus_framefmt *mf);
void csi_fmt_ini_mbus_code(struct v4l2_mbus_framefmt *mf)
{
	mf->code	= csi_colour_fmts[0].code;
	mf->colorspace	= csi_colour_fmts[0].colorspace;
}
/**************************************************/
/* end of fmt.c */
