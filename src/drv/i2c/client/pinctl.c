
/*---------------------------
 * pinctl.c
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
#include <linux/pinctrl/consumer.h>
/**************************************************/
int i2c_find_pinctl(struct device *dev,struct i2c_client_data *private);
int i2c_find_pinctl(struct device *dev,struct i2c_client_data *private)
{
	struct pinctrl *pinctrl;

	pinctrl = devm_pinctrl_get_select_default(dev);
	if (IS_ERR(pinctrl)) {
		os_print("I2C:setup pinctrl failed\n");
		return PTR_ERR(pinctrl);
	}
	return 0;
}

/**************************************************/
/* end of pinctl.c */
