
/*---------------------------
 * remove.c
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

#include "01import.h"
/**************************************************/

/**************************************************/
int v4l2_unregister_subdev(struct i2c_client *client,struct i2c_client_data *sensor);
int v4l2_unregister_subdev(struct i2c_client *client,struct i2c_client_data *sensor)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);

	v4l2_async_unregister_subdev(sd);

	clk_unprepare(sensor->sensor_clk);
	return 0;
}


/**************************************************/
/* end of remove.c */
