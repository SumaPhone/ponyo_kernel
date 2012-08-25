#ifndef _ATMEL_H
#define _ATMEL_H


#include <linux/module.h>
#include <linux/delay.h>
#include <linux/earlysuspend.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <mach/gpio.h>
#include <mach/vreg.h>
#include <linux/device.h>

#include <linux/slab.h>
#include <linux/Atmel.h>





#define MXT140_NAME "qt602240-ts"


#define CONFIG_BOARD_REVISION 0
//#define CONFIG_HAS_EARLYSUSPEND 1
#define MULTITOUCH_ENABLE_CHJ 0	//for muti-touch enable
#define NUM_OF_TOUCH_OBJECTS 10 //the number for muti-touch

/*---------------------------------------------------------/
 *
 * Quantum Code Block
 *
 * -------------------------------------------------------*/
#define  U16    unsigned short int 
#define U8      __u8
#define u8      __u8
#define S16     signed short int
#define U16     unsigned short int
#define S32     signed long int
#define U32     unsigned long int
#define S64     signed long long int
#define U64     unsigned long long int
#define F32     float
#define F64     double

#if 0
struct synaptics_i2c_rmi_platform_data {
	uint32_t version;       /* Use this entry for panels with */
	/* (major << 8 | minor) version or above. */
	/* If non-zero another array entry follows */
	int (*power)(int on);   /* Only valid in first array entry */
	uint32_t flags;
	unsigned long irqflags;
	uint32_t inactive_left; /* 0x10000 = screen width */
	uint32_t inactive_right; /* 0x10000 = screen width */
	uint32_t inactive_top; /* 0x10000 = screen height */
	uint32_t inactive_bottom; /* 0x10000 = screen height */
	uint32_t snap_left_on; /* 0x10000 = screen width */
	uint32_t snap_left_off; /* 0x10000 = screen width */
	uint32_t snap_right_on; /* 0x10000 = screen width */
	uint32_t snap_right_off; /* 0x10000 = screen width */
	uint32_t snap_top_on; /* 0x10000 = screen height */
	uint32_t snap_top_off; /* 0x10000 = screen height */
	uint32_t snap_bottom_on; /* 0x10000 = screen height */
	uint32_t snap_bottom_off; /* 0x10000 = screen height */
	uint32_t fuzz_x; /* 0x10000 = screen width */
	uint32_t fuzz_y; /* 0x10000 = screen height */
	int fuzz_p;
	int fuzz_w;
	int8_t sensitivity_adjust;
};
#endif

struct qt602240_ts_data {
	int (*power)(int on);
	uint16_t addr;
	struct i2c_client *client;
	struct input_dev *input_dev;
	int use_irq;
	struct hrtimer timer;
	struct work_struct  work;
	//      struct work_struct  work_timer;
	struct early_suspend early_suspend;
};


/*------------------------------ definition block -----------------------------------*/






#endif
