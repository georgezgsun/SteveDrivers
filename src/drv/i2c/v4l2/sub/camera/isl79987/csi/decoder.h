
/*---------------------------
 * decoder.h
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

#ifndef CSI_DECODER_H_
#define CSI_DECODER_H_

#include "../01import.h"
/**************************************************/
extern void csi_dec_ini_PAL(struct i2c_client_data *sensor);
extern void csi_dec_ini_NTSC(struct i2c_client_data *sensor);
extern void csi_dec_write_decorder(struct i2c_client_data *sensor);

/**************************************************/
#endif 
/* end of decoder.h */
