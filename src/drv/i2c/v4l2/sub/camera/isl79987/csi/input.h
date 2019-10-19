
/*---------------------------
 * input.h
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

#ifndef CSI_INPUT_H_
#define CSI_INPUT_H_

#include "01import.h"
/**************************************************/
/***********************************************************************
 * mode info. only for decoder.
 ***********************************************************************/
typedef enum {
    INPUT_CVBS1 = 0,
    INPUT_CVBS2,
    INPUT_CVBS3,
    INPUT_CVBS4,
} ISL79987_INPUT_TYPE;

#define ISL79987_INPUT_MAX 4
/**************************************************/
#endif 
/* end of input.h */
