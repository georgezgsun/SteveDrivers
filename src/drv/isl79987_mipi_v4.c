
/*---------------------------
 * isl79987_mipi_v3.c
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
static const struct i2c_device_id drv_i2c_id[] = {
	{I2CDRV_MODULE_NAME, 0},
	{},
};
/**************************************************/
#include <linux/mod_devicetable.h>
static const struct of_device_id drv_i2c_of_match[] = {
	{
		.compatible = I2CDRV_COMPATIBLE_NAME,
	},
	{},			//must be a null
};
/**************************************************/
#include <linux/module.h>
MODULE_DEVICE_TABLE(i2c, drv_i2c_id);
MODULE_DEVICE_TABLE(of, drv_i2c_of_match);
/**************************************************/
extern int i2c_probe(struct i2c_client *adapter,
				const struct i2c_device_id *device_id);
extern int i2c_remove(struct i2c_client *adapter);
/**************************************************/
int drv_probe(struct i2c_client *adapter,
				const struct i2c_device_id *device_id)
{
	int ret;
	os_print("ISL: going to register %s\n",I2CDRV_MODULE_NAME);
	ret=i2c_probe(adapter,device_id);
	return(ret);
}

static int drv_remove(struct i2c_client *adapter)
{
	int ret;
	os_print("ISL: going to unregister %s\n",I2CDRV_MODULE_NAME);
	ret=i2c_remove(adapter);
	return(ret);
}

static struct i2c_driver drv_i2c_driver = {
	.driver = {
		  .owner = THIS_MODULE,
		  .name  = I2CDRV_MODULE_NAME,
		  .of_match_table = drv_i2c_of_match,
		  },
	.probe  = drv_probe,
	.remove = drv_remove,
	.id_table = drv_i2c_id,
};
/**************************************************/
#include <linux/init.h>
module_i2c_driver(drv_i2c_driver);
/**************************************************/
MODULE_AUTHOR(MODULE_AUTHOR_NAME);
MODULE_DESCRIPTION(MODULE_DESCRIP_NAME);
MODULE_LICENSE(MODULE_LICENSE_NAME);
MODULE_VERSION(MODULE_VERSION_NAME);
MODULE_ALIAS(MODULE_ALIAS_NAME);
/**************************************************/
/* end of isl79987_mipi_v3.c */
