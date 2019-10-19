
/*---------------------------
 * res.c
 *  Created on: Apr 29, 2019
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
#include "res.h"
/**************************************************/
struct csi_res csi_valid_res[] = {
	[0] = {640, 480},
	[1] = {720, 480},
	[2] = {1280, 720},
	[3] = {1920, 1080},
	[4] = {2592, 1944},
};

/**************************************************/
int csi_get_res_mode(int width, int height);
int csi_get_res_mode(int width, int height)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(csi_valid_res); i++) {
		if ((csi_valid_res[i].width == width) &&
		     (csi_valid_res[i].height == height))
			return i;
	}

	return -1;
}
/**************************************************/
/* end of res.c */
