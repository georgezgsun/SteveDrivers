
/*---------------------------
 * decoder.c
 *  Created on: May 1, 2019
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
#include "reg.h"
/**************************************************/
static struct reg_value isl79987_decoder[]={
    {0xff,0x0f},{0x08,0x14},{0x2F,0xE6},{0x33,0x85},{0x3d,0x08},{0xE7,0x00}
};

struct reg_value isl79987_Decoder_NTSC[] = {
    {0x07,0x02},{0x08,0x12},{0x09,0xf0},{0x0a,0x0F},{0x0b,0xd0}
};

struct reg_value isl79987_Decoder_PAL[] = {
    {0x07,0x12},{0x08,0x18},{0x09,0x20},{0x0a,0x08},{0x0b,0xd0}
};

/**************************************************/
void csi_dec_ini_PAL(struct i2c_client_data *sensor);
void csi_dec_ini_PAL(struct i2c_client_data *sensor)
{
    WriteTW88(sensor,0xFF,sensor->v_channel+1);
    WriteTW88S(sensor,isl79987_Decoder_PAL, ARRAY_SIZE(isl79987_Decoder_PAL));
    WriteTW88(sensor,0xFF, 0x05);
    WriteTW88(sensor,0x0F, ReadTW88(sensor,0x0F) |  0x04);
    WriteTW88(sensor,0x3C, 0x01);
    WriteTW88(sensor,0x3D, 0x21);
}

/**************************************************/
void csi_dec_ini_NTSC(struct i2c_client_data *sensor);
void csi_dec_ini_NTSC(struct i2c_client_data *sensor)
{
    WriteTW88(sensor,0xFF,sensor->v_channel+1);
    WriteTW88S(sensor,isl79987_Decoder_NTSC, ARRAY_SIZE(isl79987_Decoder_NTSC));
    WriteTW88(sensor,0xFF, 0x05);
    WriteTW88(sensor,0x0F, ReadTW88(sensor,0x0F) & ~0x04);
    WriteTW88(sensor,0x3C, 0x00);
    WriteTW88(sensor,0x3D, 0xF1);
}

/**************************************************/
void csi_dec_write_decorder(struct i2c_client_data *sensor);
void csi_dec_write_decorder(struct i2c_client_data *sensor)
{
    WriteTW88S(sensor,isl79987_decoder, ARRAY_SIZE(isl79987_decoder));
}

/**************************************************/
/* end of decoder.c */
