
/*---------------------------
 * probe.c
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
#include <linux/errno.h>
#include "./client/data.h"
/**************************************************/
extern int v4l2_register_subdev(struct i2c_client *client,struct i2c_client_data *sensor);
/**************************************************/
static int i2c_find_csi_mclk(struct device *dev,struct i2c_client_data *sensor);
static int i2c_find_csi_mclk(struct device *dev,struct i2c_client_data *sensor)
{
	/* Set initial values for the sensor struct. */
	sensor->sensor_clk = devm_clk_get(dev, "csi_mclk");
	if (IS_ERR(sensor->sensor_clk)) {
		/* assuming clock enabled by default */
		sensor->sensor_clk = NULL;
		return PTR_ERR(sensor->sensor_clk);
	}
	return 0;
}
int i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
int i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	struct i2c_client_data *sensor;
	struct device *dev;
	ret=0;
	dev = &client->dev;

	sensor=i2c_data_ini_client_data(client);
	if(sensor == NULL)	goto mem_err;
	dev_set_drvdata(dev,sensor);		//save driver data for remove use.

	ret=i2c_find_pinctl(dev,sensor);
	if(ret<0)	goto pinctl_err;
	ret=i2c_gpio_find_pin(dev,sensor);
	if(ret<0)	goto gpio_err;
	ret=i2c_find_csi_mclk(dev,sensor);
	if(ret<0)	goto mclk_err;
	ret=i2c_find_mclk(dev,sensor);
	if(ret<0)	goto mclk_err;
	ret = clk_prepare_enable(sensor->sensor_clk);
	if (ret < 0) goto mclk_err;
	ret=i2c_find_regulater(dev,sensor);
	if(ret<0)	goto rgl_err;
	ret=i2c_enable_regulater(sensor);
	if(ret<0)	goto rgl_err;

	i2c_gpio_reset_subdev(sensor);
	i2c_gpio_power_up(sensor);
	ret=v4l2_register_subdev(client,sensor);
	if(ret<0)	goto v4l2_err;

	os_print("I2C:register v4l2 successfully\n");
	return 0;

v4l2_err:
	i2c_gpio_power_down(sensor);
	i2c_disable_regulater(sensor);
	clk_disable_unprepare(sensor->sensor_clk);
	os_print("I2C:fail to register v4l2\n");
	return -ENODEV;
rgl_err:
	clk_disable_unprepare(sensor->sensor_clk);
	os_print("I2C:fail to enable regulator\n");
	return -ENODEV;
mclk_err:
	os_print("I2C:fail to find mclk\n");
	return -ENODEV;
gpio_err:
	os_print("I2C:fail to find gpio\n");
	return -ENODEV;
pinctl_err:
	os_print("I2C:fail to find pin ctl\n");
	return -ENODEV;
mem_err:
	os_print("I2C:fail to ini mem\n");
	return -ENOMEM;
}
/**************************************************/
/* end of probe.c */
