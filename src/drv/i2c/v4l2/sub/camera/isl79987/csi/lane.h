
/*---------------------------
 * lane.h
 *  Created on: May 2, 2019
 *      Author: Steve Rui
 *---------------------------
 *
 *
 *
 *
 *
 *---------------------------
 */

#ifndef CSI_LANE_H_
#define LANE_H_

#include "../01import.h"
/**************************************************/

extern void csi_lane_write_channel(struct i2c_client_data *sensor,int channel,int lane);
/**************************************************/
#endif 
/* end of lane.h */
