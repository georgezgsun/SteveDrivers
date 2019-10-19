
/*---------------------------
 * hus.h
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

#ifndef CAMERA_HUS_H_
#define CAMERA_HUS_H_

#include "01import.h"
/**************************************************/
struct csi_hs_info {
	u32 width;
	u32 height;
	u32 frame_rate;
	u32 val;
};

extern uint16_t csi_hus_find_configure(struct i2c_client_data *sensor);
/**************************************************/
#endif 
/* end of hus.h */
