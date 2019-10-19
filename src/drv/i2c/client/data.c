
/*---------------------------
 * data.c
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
struct i2c_client_data *i2c_data_get_client_data(const struct i2c_client *client);
struct i2c_client_data *i2c_data_get_client_data(const struct i2c_client *client)
{

	return container_of(i2c_get_clientdata(client), struct i2c_client_data, subdev);
}

/**************************************************/
extern void i2c_gpio_reset_subdev(struct i2c_client_data *private);
struct i2c_client_data *i2c_data_ini_client_data(struct i2c_client *client);
struct i2c_client_data *i2c_data_ini_client_data(struct i2c_client *client)
{
	struct i2c_client_data  *client_priv_data;
	struct device *dev = &client->dev;

	client_priv_data = devm_kmalloc(dev, sizeof(*client_priv_data), GFP_KERNEL);
	if (!client_priv_data)
		return NULL;
	/* Set initial values for the private struct. */
	memset(client_priv_data, 0, sizeof(*client_priv_data));
	client_priv_data->i2c_client=client;
	client_priv_data->io_init=i2c_gpio_reset_subdev;

	client_priv_data->pix.pixelformat = V4L2_PIX_FMT_UYVY;
	client_priv_data->pix.width = 720;
	client_priv_data->pix.height = 480; /* for FrameMode */
	client_priv_data->pix.field = V4L2_FIELD_INTERLACED_TB;
	client_priv_data->pix.bytesperline = 720 * 2;
	client_priv_data->pix.sizeimage = 720 * 2 * 480;
	client_priv_data->pix.priv = 1;  /*NOTE: 1 means TV_in. 0 is camera */

	client_priv_data->is_mipi = 1;
	client_priv_data->streamcap.capturemode = 0;
	client_priv_data->streamcap.timeperframe.denominator = 30;
	client_priv_data->streamcap.timeperframe.numerator = 1;

    client_priv_data->streamcap.capability
        = V4L2_MODE_HIGHQUALITY | V4L2_CAP_TIMEPERFRAME;

    client_priv_data->on = false; /* _dev_init() will assign true*/
	client_priv_data->max_sensor = 1;
	client_priv_data->lanes = 1;
	client_priv_data->bclock = 216;

	client_priv_data->pseudo = 0;
	client_priv_data->histo = 0;
	client_priv_data->nvc = 0;

    //client_priv_data->use_counter = 0;
    client_priv_data->std_id = V4L2_STD_ALL;
    client_priv_data->prev_std_id = V4L2_STD_NTSC;

	client_priv_data->csi = 0;
	client_priv_data->v_channel = 0;
	client_priv_data->input_source = 0; /* CH1,Decoder0 */

	client_priv_data->AE_Target = 52;
	return(client_priv_data);
}

/**************************************************/
/* end of data.c */
