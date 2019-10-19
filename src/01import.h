
/*---------------------------
 * 01import.h
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

#ifndef SRC_01IMPORT_H_
#define SRC_01IMPORT_H_

#include <linux/device.h>
#include <linux/of_device.h>
#include <uapi/asm-generic/errno-base.h>
/**************************************************/
#ifndef u32
#define u32 unsigned int
#endif

#ifndef u16
#define u16 unsigned short int
#endif

#ifndef u8
#define u8 unsigned char
#endif

#define  os_print(fmt,...)  printk(KERN_ERR fmt,##__VA_ARGS__)
//#define  os_print(fmt,...)
/**************************************************/
#endif 
/* end of 01import.h */
