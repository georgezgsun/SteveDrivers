
/*---------------------------
 * regulator.c
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
#include <linux/regulator/consumer.h>
/**************************************************/
int i2c_disable_regulater(struct i2c_client_data *private);
int i2c_disable_regulater(struct i2c_client_data *private)
{
	if (private->gpo_rgl)
		regulator_disable(private->gpo_rgl);
	if (private->analog_rgl)
		regulator_disable(private->analog_rgl);
	if (private->core_rgl)
		regulator_disable(private->core_rgl);
	if (private->io_rgl)
			regulator_disable(private->io_rgl);
	return 0;
}
/**************************************************/
int i2c_enable_regulater(struct i2c_client_data *private);
int i2c_enable_regulater(struct i2c_client_data *private)
{
	if (private->io_rgl)
		if (regulator_enable(private->io_rgl) != 0)
			return -EIO;
	if (private->core_rgl)
		if (regulator_enable(private->core_rgl) != 0)
			return -EIO;
	if (private->gpo_rgl)
		if (regulator_enable(private->gpo_rgl) != 0)
			return -EIO;
	if (private->analog_rgl)
		if (regulator_enable(private->analog_rgl) != 0)
			return -EIO;
	return 0;
}
/**************************************************/

int i2c_find_regulater(struct device *dev,struct i2c_client_data *private);
int i2c_find_regulater(struct device *dev,struct i2c_client_data *private)
{
	int ret = 0;

	private->io_rgl = devm_regulator_get(dev, "DOVDD");
	if (!IS_ERR(private->io_rgl)) {
		regulator_set_voltage(private->io_rgl,
				      CAMERA_VOLTAGE_DIGITAL_IO,
					  CAMERA_VOLTAGE_DIGITAL_IO);
		ret = regulator_enable(private->io_rgl);
		if (ret) {
			os_print("set io voltage failed\n");
			return ret;
		} else {
			os_print("set io voltage ok\n");
		}
	} else {
		private->io_rgl = NULL;
		os_print("cannot get io voltage\n");
	}

	private->core_rgl = devm_regulator_get(dev, "DVDD");
	if (!IS_ERR(private->core_rgl)) {
		regulator_set_voltage(private->core_rgl,
				CAMERA_VOLTAGE_DIGITAL_CORE,
				CAMERA_VOLTAGE_DIGITAL_CORE);
		ret = regulator_enable(private->core_rgl);
		if (ret) {
			os_print("set core voltage failed\n");
			return ret;
		} else {
			os_print("set core voltage ok\n");
		}
	} else {
		private->core_rgl = NULL;
		os_print("cannot get core voltage\n");
	}

	private->analog_rgl = devm_regulator_get(dev, "AVDD");
	if (!IS_ERR(private->analog_rgl)) {
		regulator_set_voltage(private->analog_rgl,
				CAMERA_VOLTAGE_ANALOG,
				CAMERA_VOLTAGE_ANALOG);
		ret = regulator_enable(private->analog_rgl);
		if (ret)
			os_print("set analog voltage failed\n");
		 else
			os_print("set analog voltage ok\n");
	} else {
		private->analog_rgl = NULL;
		os_print("cannot get analog voltage\n");
	}

	return ret;
}
/**************************************************/
/* end of regulator.c */
