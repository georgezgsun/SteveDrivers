
/*---------------------------
 * mclk.c
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
#include <linux/clk.h>
#include <linux/of.h>
/**************************************************/
int i2c_find_mclk(struct device *dev,struct i2c_client_data *private);
int i2c_find_mclk(struct device *dev,struct i2c_client_data *private)
{
	int retval;

	retval = of_property_read_u32(dev->of_node, "mclk",
					&(private->mclk));
	if (retval) {
		os_print("I2C:mclk missing or invalid\n");
		return retval;
	}

	retval = of_property_read_u32(dev->of_node, "mclk_source",
					(u32 *) &(private->mclk_source));
	if (retval) {
		os_print("I2C:mclk_source missing or invalid\n");
		return retval;
	}

	os_print("I2C:mclk:%d, mclk_source:%d\n",private->mclk,private->mclk_source);
	return 0;
}

/**************************************************/
/* end of mclk.c */
