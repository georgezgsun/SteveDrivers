
/*---------------------------
 * imput.c
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

#include "01import.h"
/**************************************************/
#include "input.h"
/**************************************************/
static struct v4l2_input isl79987_inputs[] = {
    { .index=0,  .name="CVBS1",
      .type = V4L2_INPUT_TYPE_CAMERA, .std = V4L2_STD_ALL,
      .capabilities = V4L2_IN_CAP_STD, .status= V4L2_IN_ST_NO_H_LOCK,},
    { .index=1,  .name="CVBS2",
      .type = V4L2_INPUT_TYPE_CAMERA, .std = V4L2_STD_ALL,
      .capabilities = V4L2_IN_CAP_STD, .status= V4L2_IN_ST_NO_H_LOCK,},
    { .index=2,  .name="CVBS3",
      .type = V4L2_INPUT_TYPE_CAMERA, .std = V4L2_STD_ALL,
      .capabilities = V4L2_IN_CAP_STD, .status= V4L2_IN_ST_NO_H_LOCK,},
    { .index=3,  .name="CVBS4",
      .type = V4L2_INPUT_TYPE_CAMERA, .std = V4L2_STD_ALL,
      .capabilities = V4L2_IN_CAP_STD, .status= V4L2_IN_ST_NO_H_LOCK,},
};
/**************************************************/
int csi_input_is_ntsc(struct i2c_client_data *sensor, v4l2_std_id *std);
int csi_input_is_ntsc(struct i2c_client_data *sensor, v4l2_std_id *std)
{
    isl79987_inputs[sensor->v_channel].status &= ~V4L2_IN_ST_NO_SIGNAL;
    isl79987_inputs[sensor->v_channel].status &= ~V4L2_IN_ST_NO_H_LOCK;
    *std = V4L2_STD_NTSC;
    sensor->std_id = V4L2_STD_NTSC;
    sensor->pix.width = 720;
    sensor->pix.height = 480; /*BT656:525*/
    sensor->streamcap.timeperframe.denominator = 30;
    sensor->pix.sizeimage = 720 * 2 * 480;

    return 0;
}
/**************************************************/
/* end of imput.c */
