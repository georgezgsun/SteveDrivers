
/*---------------------------
 * remove.c
 *  Created on: Apr 23, 2019
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
#include <linux/i2c.h>
#include "./client/data.h"
/**************************************************/
extern int	v4l2_unregister_subdev(struct i2c_client *client,struct i2c_client_data *sensor);
extern void i2c_power_down(struct i2c_client_data *sensor,int enable);
extern int	i2c_disable_regulater(struct i2c_client_data *sensor);
int i2c_remove(struct i2c_client *client);
int i2c_remove(struct i2c_client *client)
{
	int ret;
	struct i2c_client_data *sensor;
	struct device *dev;

	ret=0;
	dev = &client->dev;

	sensor=dev_get_drvdata(dev);
	ret=v4l2_unregister_subdev(client,sensor);
	i2c_gpio_power_down(sensor);
	ret=i2c_disable_regulater(sensor);
	os_print("I2C:unregister v4l2 successfully\n");
	return ret;
}
/**************************************************/
/* end of remove.c */
