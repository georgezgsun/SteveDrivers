
/*---------------------------
 * gpio.c
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
#include <linux/of_gpio.h>
#include <linux/pinctrl/consumer.h>
#include <linux/delay.h>
/**************************************************/
void i2c_gpio_power(struct i2c_client_data *sensor, int enable);
void i2c_gpio_power(struct i2c_client_data *sensor, int enable)
{
	if (sensor->pwn_gpio < 0)
		return;

	if (!enable)
		gpio_set_value_cansleep(sensor->pwn_gpio, 0);
	else
		gpio_set_value_cansleep(sensor->pwn_gpio, 1);

	msleep(2);
}

void i2c_gpio_power_down(struct i2c_client_data *sensor);
void i2c_gpio_power_down(struct i2c_client_data *sensor)
{
	int enable;

	enable=1;
	if (sensor->pwn_gpio < 0)
		return;

	if (!enable)
		gpio_set_value_cansleep(sensor->pwn_gpio, 0);
	else
		gpio_set_value_cansleep(sensor->pwn_gpio, 1);

	msleep(2);
}
/**************************************************/
void i2c_gpio_power_up(struct i2c_client_data *sensor);
void i2c_gpio_power_up(struct i2c_client_data *sensor)
{
	int enable;

	enable=0;
	if (sensor->pwn_gpio < 0)
		return;

	if (!enable)
		gpio_set_value_cansleep(sensor->pwn_gpio, 0);
	else
		gpio_set_value_cansleep(sensor->pwn_gpio, 1);

	msleep(2);
}

/**************************************************/
//Hardware reset sub device by power off and on, include camera and display.
void i2c_gpio_reset_subdev(struct i2c_client_data *sensor);
void i2c_gpio_reset_subdev(struct i2c_client_data *sensor)
{
	if (sensor->rst_gpio < 0 || sensor->pwn_gpio < 0)
		return;

	/* camera reset */
	gpio_set_value(sensor->rst_gpio, 1);

	/* camera power dowmn */
	gpio_set_value(sensor->pwn_gpio, 1);
	msleep(5);

	gpio_set_value(sensor->rst_gpio, 0);
	msleep(1);

	gpio_set_value(sensor->pwn_gpio, 0);
	msleep(5);

	gpio_set_value(sensor->rst_gpio, 1);
	msleep(5);
}
/**************************************************/
static int i2c_find_pwn_pin(struct device *dev,struct i2c_client_data *private);
static int i2c_find_pwn_pin(struct device *dev,struct i2c_client_data *private)
{
	int retval;

	/* request power down pin */
	private->pwn_gpio = of_get_named_gpio(dev->of_node, "pwn-gpios", 0);
	if (!gpio_is_valid(private->pwn_gpio))
		os_print("No sensor pwdn pin available");
	else
	{
		retval = devm_gpio_request_one(dev, private->pwn_gpio,
			GPIOF_OUT_INIT_HIGH, CAMERA_GPIO_POWER_DOWN_NAME);
		if (retval < 0)
		{
			os_print("Failed to set power pin\n");
			os_print("retval=%d\n", retval);
		return retval;
		}
	}
	return 0;
}
/**************************************************/
static int i2c_find_rst_pin(struct device *dev,struct i2c_client_data *private);
static int i2c_find_rst_pin(struct device *dev,struct i2c_client_data *private)
{
	int retval;

	private->rst_gpio = of_get_named_gpio(dev->of_node, "rst-gpios", 0);
	if (!gpio_is_valid(private->rst_gpio))
		os_print("No sensor reset pin available");
	else {
		retval = devm_gpio_request_one(dev, private->rst_gpio,
				GPIOF_OUT_INIT_HIGH, CAMERA_GPIO_RESET_NAME);
		if (retval < 0) {
			os_print("Failed to set reset pin\n");
			return retval;
		}
	}
	return 0;
}

/**************************************************/
int i2c_gpio_find_pin(struct device *dev,struct i2c_client_data *private);
int i2c_gpio_find_pin(struct device *dev,struct i2c_client_data *private)
{
	int ret,ret1;
	ret=0;
	ret1=i2c_find_rst_pin(dev,private);
	ret |=ret1;
	ret=i2c_find_pwn_pin(dev,private);
	ret |=ret1;
	return ret;
}
/**************************************************/
/* end of gpio.c */
