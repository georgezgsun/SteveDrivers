
/*---------------------------
 * data.h
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

#ifndef I2C_DATA_H_
#define I2C_DATA_H_

#include "01import.h"
/**************************************************/
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/regulator/consumer.h>

#include <linux/clk.h>
#include <linux/v4l2-mediabus.h>
#include <media/v4l2-device.h>
#include <media/v4l2-ctrls.h>

#include "../v4l2/sub/camera/fmt.h"
#include "../v4l2/sub/camera/pad.h"
#include "../v4l2/sub/camera/gpio.h"
#include "../v4l2/sub/camera/voltage.h"
#include "../v4l2/sub/camera/mclk.h"
struct i2c_client_data {


	//i2c_client
	struct i2c_client *i2c_client;
	void (*io_init)(struct i2c_client_data *);
	//i2c_gpio
	int pwn_gpio, rst_gpio;
	//i2c_clk
	u32 mclk;
	u8 mclk_source;
	//regulator
	struct regulator *io_rgl;
	struct regulator *core_rgl;
	struct regulator *analog_rgl;
	struct regulator *gpo_rgl;
	//i2c on/off
	bool on;

	//v4l2 data
	struct v4l2_subdev		subdev;
	struct v4l2_pix_format  pix;
	struct v4l2_captureparm streamcap;

	struct clk 	*sensor_clk;

	//
	struct media_pad pads[CAMERA_SENS_PADS_NUM];

	/* control settings */
	int brightness;
	int hue;
	int contrast;
	int saturation;
	int red;
	int green;
	int blue;
	int ae_mode;
	int csi;
	////////////////////self defined for ISL79987
	bool is_dummy; /* for dummy */
	bool is_mipi; /*BKNote: I need it */
    int input_source;   //related CH number [1..4] on ISL79987
                        //related CH number [1..4] on TW9992
                        //related CH1,CH2,DTVA1,... TW8844
	int v_channel;      //it is real virtual channel
	int use_counter;
	//bool is_mipi_interlaced; /* for FieldMode */
	bool rgb565;
    int max_sensor;

    int lanes;
    int bclock; /* bit clock */
    int pseudo; //non_std_ch;
    int histo;
    int nvc;    /* PseudoFrame vc. */

    v4l2_std_id std_id;
    v4l2_std_id prev_std_id;

	//self defined for OV5640
	const struct csi_datafmt *fmt;

	int prev_sysclk;
	int	prev_HTS;
	int AE_low;
	int AE_high;
	int AE_Target;
};

extern struct i2c_client_data *i2c_data_get_client_data(const struct i2c_client *client);
extern struct i2c_client_data *i2c_data_ini_client_data(struct i2c_client *client);

extern void i2c_gpio_power_down(struct i2c_client_data *private);
extern void i2c_gpio_power_up(struct i2c_client_data *private);
extern void i2c_gpio_reset_subdev(struct i2c_client_data *sensor);
extern int i2c_gpio_find_pin(struct device *dev,struct i2c_client_data *private);

extern int i2c_find_mclk(struct device *dev,struct i2c_client_data *private);
extern int i2c_find_pinctl(struct device *dev,struct i2c_client_data *private);
extern int i2c_find_regulater(struct device *dev,struct i2c_client_data *private);
extern int i2c_disable_regulater(struct i2c_client_data *private);
extern int i2c_enable_regulater(struct i2c_client_data *private);

/**************************************************/
#endif 
/* end of data.h */
