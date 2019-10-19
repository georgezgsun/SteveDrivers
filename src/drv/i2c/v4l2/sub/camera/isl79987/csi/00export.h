
/*---------------------------
 * 00export.h
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

#ifndef CSI_00EXPORT_H_
#define CSI_00EXPORT_H_

/**************************************************/
extern void csi_dec_ini_PAL(struct i2c_client_data *sensor);
extern void csi_dec_ini_NTSC(struct i2c_client_data *sensor);
extern void csi_dec_write_decorder(struct i2c_client_data *sensor);

extern int csi_input_is_ntsc(struct i2c_client_data *sensor, v4l2_std_id *std);

extern void csi_lane_write_channel(struct i2c_client_data *sensor,int channel,int lane);
/**************************************************/
#endif 
/* end of 00export.h */
