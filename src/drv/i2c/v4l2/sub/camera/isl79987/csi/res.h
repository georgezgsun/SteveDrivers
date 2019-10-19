
/*---------------------------
 * res.h
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

#ifndef CSI_RES_H_
#define CSI_RES_H_

#include "01import.h"
/**************************************************/
struct csi_res {
	int width;
	int height;
};

extern int csi_get_res_mode(int width, int height);
/**************************************************/
#endif 
/* end of res.h */
