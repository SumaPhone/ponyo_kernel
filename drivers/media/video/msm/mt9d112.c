/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <media/msm_camera.h>
#include <mach/gpio.h>
#include "mt9d112.h"
#include <linux/pwm.h>
#include <linux/leds_pwm.h>
#include <linux/leds.h>
#include <linux/err.h>
#include <linux/pwm.h>

/* Micron MT9D112 Registers and their values */
/* Sensor Core Registers */
#define  REG_MT9D112_MODEL_ID 0x0000
#define  MT9D112_MODEL_ID     0x2880
#define  MT9D112_AF_STATUS    0xB000
/*  SOC Registers Page 1  */
#define  REG_MT9D112_SENSOR_RESET     0x301A
#define  REG_MT9D112_STANDBY_CONTROL  0x3202
#define  REG_MT9D112_MCU_BOOT         0x3386

#define FLASH_LED

int Flash_Flag = -1;
int effect_mode = -1;
int wb_mode = -1;
int scene_mode = -1;
int brightness_mode = -1;

#if 0
#ifdef FLASH_LED

	struct led_pwm_data {
	struct led_classdev	cdev;
	struct pwm_device	*pwm;
	unsigned int 		active_low;
	unsigned int		period;
};

	struct led_pwm_data *led_dat;
	struct led_pwm *cur_led;

#endif
#endif

struct mt9d112_work {
	struct work_struct work;
};

struct pga_struct {
    int32_t g1_p0q2;     /* 0x3644 */
    int32_t r_p0q2;    /* 0x364E */
    int32_t b_p0q1;    /* 0x3656 */
    int32_t b_p0q2;    /* 0x3658 */
    int32_t g2_p0q2;    /* 0x3662 */
    int32_t r_p1q0;    /* 0x368A */
    int32_t b_p1q0;    /* 0x3694 */
    int32_t g1_p2q0;    /* 0x36C0 */
    int32_t r_p2q0;    /* 0x36CA */
    int32_t b_p2q0;    /* 0x36D4 */
    int32_t g2_p2q0;    /* 0x36DE */
};

static struct  mt9d112_work *mt9d112_sensorw;
static struct  i2c_client *mt9d112_client;

struct mt9d112_ctrl {
	const struct msm_camera_sensor_info *sensordata;
};

static unsigned short use_old_module = 0; /*yangyuan 20110922*/

static struct mt9d112_ctrl *mt9d112_ctrl;

static int32_t mt9d112_i2c_read(unsigned short   saddr,	unsigned short raddr, unsigned short *rdata, enum mt9d112_width width);

static DECLARE_WAIT_QUEUE_HEAD(mt9d112_wait_queue);
DECLARE_MUTEX(mt9d112_sem);
//static int16_t mt9d112_effect = CAMERA_EFFECT_OFF;
//static int16_t mt9d112_scene = CAMERA_BESTSHOT_OFF;

/*=============================================================
	EXTERNAL DECLARATIONS
==============================================================*/
extern struct mt9d112_reg mt9d112_regs;


/*=============================================================*/

static int mt9d112_reset(const struct msm_camera_sensor_info *dev)
{
	int rc = 0;

	//CDBG_ZS("mt9d112_reset \n");

	rc = gpio_request(dev->sensor_reset, "mt9d112");

	if (!rc) {
		rc = gpio_direction_output(dev->sensor_reset, 1);
		mdelay(10);
		rc = gpio_direction_output(dev->sensor_reset, 0);
		mdelay(30);
		rc = gpio_direction_output(dev->sensor_reset, 1);
		mdelay(10);
	}

	gpio_free(dev->sensor_reset);
	return rc;
}

static int32_t mt9d112_i2c_txdata(unsigned short saddr,
	unsigned char *txdata, int length)
{
	struct i2c_msg msg[] = {
		{
			.addr = saddr,
			.flags = 0,
			.len = length,
			.buf = txdata,
		},
	};

	if(use_old_module)
	{
		if (i2c_transfer(mt9d112_client->adapter, msg, 1) < 0) {
			CDBG_ZS("mt9d112_i2c_txdata failed\n");
			return -EIO;
		}
	}
	else
	{
		if (i2c_transfer(mt9d112_client->adapter, msg, 1) < 0) {
			CDBG_ZS("mt9d112_i2c_txdata first failed\n");
				msleep(100);
				if (i2c_transfer(mt9d112_client->adapter, msg, 1) < 0) {
					CDBG_ZS("mt9d112_i2c_txdata twice failed\n");
					msleep(200);
						if (i2c_transfer(mt9d112_client->adapter, msg, 1) < 0) {
							CDBG_ZS("mt9d112_i2c_txdata failed\n");
							return -EIO;
						}
				}
		}
	}
	return 0;
}

static int32_t mt9d112_i2c_write(unsigned short saddr,
	unsigned short waddr, unsigned short wdata, enum mt9d112_width width)
{
	int32_t rc = -EIO;
	unsigned char buf[4];
	uint16_t bytePoll = 0;
	uint8_t count = 0;
	uint8_t poll_reset_count = 0;

	memset(buf, 0, sizeof(buf));
	switch (width) {
	case WORD_LEN: {
		buf[0] = (waddr & 0xFF00)>>8;
		buf[1] = (waddr & 0x00FF);
		buf[2] = (wdata & 0xFF00)>>8;
		buf[3] = (wdata & 0x00FF);

		rc = mt9d112_i2c_txdata(saddr, buf, 4);
	}
		break;

	case BYTE_LEN: {
		buf[0] = (waddr & 0xFF00)>>8;
		buf[1] = (waddr & 0x00FF);
		buf[2] = (wdata & 0x00FF) ;
		rc = mt9d112_i2c_txdata(saddr, buf, 3);
	}
		break;

	case BYTE_POLL: {
		do {
			count++;
			msleep(10);
			rc = mt9d112_i2c_read(saddr, waddr, &bytePoll, BYTE_LEN);
			//printk("[Debug Info] Read Back!!, raddr = 0x%x, rdata = 0x%x.\n", waddr, bytePoll);

			if(count >= 101){
				count = 0;
				rc = mt9d112_i2c_write(mt9d112_client->addr, 0x8404, 0x02, 150);
				poll_reset_count ++;

				printk("[Debug Info] Read Back!!, poll_reset_count = 0x%x\n", poll_reset_count);

				if(poll_reset_count ==3){
					count = 101;
				}
			}
		} while( (bytePoll != wdata) && (count <101) );

		printk("[Debug Info] Read Back!!, raddr = 0x%x, rdata = 0x%x, count = %d.\n", waddr, bytePoll, count);
	}
		break;
	case OTPM_POLL: {
		do {
			count++;
			msleep(40);
			rc = mt9d112_i2c_read(saddr, waddr, &bytePoll, BYTE_LEN);
		} while(( (bytePoll&waddr) == 0) && (count <20) );
	}
		break;

	default:
		break;
	}

	if (rc < 0)
		CDBG_ZS("i2c_write failed, addr = 0x%x, val = 0x%x!\n",waddr, wdata);

	return rc;
}

static int32_t mt9d112_i2c_write_table(
	struct mt9d112_i2c_reg_conf const *reg_conf_tbl,
	int num_of_items_in_table)
{
	int i;
	int32_t rc = -EIO;

	for (i = 0; i < num_of_items_in_table; i++) {
		rc = mt9d112_i2c_write(mt9d112_client->addr,
			reg_conf_tbl->waddr, reg_conf_tbl->wdata,
			reg_conf_tbl->width);
		if (rc < 0)
			break;
		if (reg_conf_tbl->mdelay_time != 0)
			msleep(reg_conf_tbl->mdelay_time);
		reg_conf_tbl++;
	}

	return rc;
}

static int32_t mt9p111_i2c_write_burst_mode(unsigned short saddr,
	unsigned short waddr, unsigned short wdata1, unsigned short wdata2, unsigned short wdata3, unsigned short wdata4,
	unsigned short wdata5, unsigned short wdata6, unsigned short wdata7, unsigned short wdata8, enum mt9d112_width width)
{
	int32_t rc = -EIO;
	unsigned char buf[18];

	//uint16_t readBack = 0;
	//printk("[Debug Info] mt9p111_i2c_write_burst_mode(), waddr = 0x%x, wdata1 = 0x%x.\n", waddr, wdata1);

	memset(buf, 0, sizeof(buf));
	switch (width) {
	case WORD_LEN: {
		buf[0] = (waddr & 0xFF00)>>8;
		buf[1] = (waddr & 0x00FF);
		buf[2] = (wdata1 & 0xFF00)>>8;
		buf[3] = (wdata1 & 0x00FF);
		buf[4] = (wdata2 & 0xFF00)>>8;
		buf[5] = (wdata2 & 0x00FF);
		buf[6] = (wdata3 & 0xFF00)>>8;
		buf[7] = (wdata3 & 0x00FF);
		buf[8] = (wdata4 & 0xFF00)>>8;
		buf[9] = (wdata4 & 0x00FF);
		buf[10] = (wdata5 & 0xFF00)>>8;
		buf[11] = (wdata5 & 0x00FF);
		buf[12] = (wdata6 & 0xFF00)>>8;
		buf[13] = (wdata6 & 0x00FF);
		buf[14] = (wdata7 & 0xFF00)>>8;
		buf[15] = (wdata7 & 0x00FF);
		buf[16] = (wdata8 & 0xFF00)>>8;
		buf[17] = (wdata8 & 0x00FF);

		rc = mt9d112_i2c_txdata(saddr, buf, 18);
	}
		break;

	default:
		break;
	}

	//mdelay(1);
	//rc = mt9p111_i2c_read(saddr, waddr, &readBack, (enum mt9p111_width)width);
	//printk("[Debug Info] Read Back!!, raddr = 0x%x, rdata = 0x%x.\n", waddr, readBack);
	//mdelay(1);

	if (rc < 0)
		printk("Burst mode: i2c_write failed, addr = 0x%x!\n", waddr);

	return rc;
}

static int32_t mt9p111_i2c_write_table_burst_mode(
	struct mt9p111_i2c_reg_burst_mode_conf const *reg_conf_tbl,
	int num_of_items_in_table)
{
	int i;
	int32_t rc = -EIO;

	for (i = 0; i < num_of_items_in_table; i++) {
		rc = mt9p111_i2c_write_burst_mode(mt9d112_client->addr,
			reg_conf_tbl->waddr, reg_conf_tbl->wdata1, reg_conf_tbl->wdata2, reg_conf_tbl->wdata3, reg_conf_tbl->wdata4,
			reg_conf_tbl->wdata5, reg_conf_tbl->wdata6, reg_conf_tbl->wdata7, reg_conf_tbl->wdata8, reg_conf_tbl->width);
		if (rc < 0)
			break;
		if (reg_conf_tbl->mdelay_time != 0)
			msleep(reg_conf_tbl->mdelay_time);

		reg_conf_tbl++;
	}

	return rc;
}

static int mt9d112_i2c_rxdata(unsigned short saddr,
	unsigned char *rxdata, int length)
{
	struct i2c_msg msgs[] = {
	{
		.addr   = saddr,
		.flags = 0,
		.len   = 2,
		.buf   = rxdata,
	},
	{
		.addr   = saddr,
		.flags = I2C_M_RD,
		.len   = length,
		.buf   = rxdata,
	},
	};

	if (i2c_transfer(mt9d112_client->adapter, msgs, 2) < 0) {
		CDBG_ZS("mt9d112_i2c_rxdata failed!\n");
		return -EIO;
	}

	return 0;
}

static int32_t mt9d112_i2c_read(unsigned short   saddr,
	unsigned short raddr, unsigned short *rdata, enum mt9d112_width width)
{
	int32_t rc = 0;
	unsigned char buf[4];

	if (!rdata)
		return -EIO;

	memset(buf, 0, sizeof(buf));

	switch (width) {
	case WORD_LEN: {
		buf[0] = (raddr & 0xFF00)>>8;
		buf[1] = (raddr & 0x00FF);

		rc = mt9d112_i2c_rxdata(saddr, buf, 2);
		if (rc < 0)
			return rc;

		*rdata = buf[0] << 8 | buf[1];
	}
		break;
	case BYTE_LEN: {
		buf[0] = (raddr & 0xFF00)>>8;
		buf[1] = (raddr & 0x00FF);

		rc = mt9d112_i2c_rxdata(saddr, buf, 1);
		if (rc < 0)
			return rc;

		*rdata = buf[0];
	}
		break;

	default:
		break;
	}

	if (rc < 0)
		CDBG_ZS("mt9d112_i2c_read failed!\n");

	return rc;
}

/*
static int32_t mt9d112_set_lens_roll_off(void)
{
	int32_t rc = 0;
	//rc = mt9d112_i2c_write_table(&mt9d112_regs.rftbl[0],
	//							 mt9d112_regs.rftbl_size);
	return rc;
}
*/

static int32_t RegistertoFloat16(unsigned short reg16)
{
    int32_t f16val;

    int32_t fbit15, fbit14to5, fbit4to0;
    if(reg16>>15)
        fbit15 = -1;
    else fbit15 = 1;

    fbit14to5 = ((reg16>>5)&0x3FF) + 1024;
    fbit4to0 = (reg16&0x1F) - 10;
    // for debug
    CDBG_ZS("RegistertoFloat16: fbit15=%d, fbit14to5=%d, fbit4to0=%d\n", fbit15, fbit14to5, fbit4to0);

    if (fbit4to0 < 0)
        return 0;
    else
    {
        f16val = (fbit14to5 << fbit4to0) * fbit15;
        CDBG_ZS("RegistertoFloat16 return: f16val=%d\n", f16val);
        return f16val;
    }
}

static unsigned short Float16toRegister(int32_t f16val)
{
    unsigned short bit4to0, bit14to5, bit15;
    unsigned short Reg16;
    // for debug
    CDBG_ZS("Float16toRegister: f16val=%d\n", f16val);

    // 1. bit15
    if (f16val < 0)
    {
        f16val*= -1;
        bit15 = 1;
    }
    else
    bit15 = 0;

    // 2. bit 4 ~ 0
    bit4to0 = 10;
    while(f16val >= 2048)
    {
        f16val=f16val >> 1;
        bit4to0++;
    }

    // 3. bit 14 ~ 5
    bit14to5 = (unsigned short)(f16val -1024);

    // for debug
    CDBG_ZS("Float16toRegister: bit15=0x%x, bit14to5=0x%x, bit4to0=0x%x\n", bit15, bit14to5, bit4to0);

    // Merge to Register
    Reg16 = ((bit15<<15)&0x8000) | ((bit14to5<<5) & 0x7FE0) | (bit4to0 & 0x001F);
    return Reg16;
}


static int32_t mt9p111_dynPGA_init(void)
{
    struct pga_struct pgaOutdoor, pgaIndoor;
    int32_t rc;
    unsigned short reg_data;
    unsigned short OTPM_Status;

    CDBG_ZS("start mt9p111_dynPGA_init Setting.\n");

    // OTP Load
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x381C, 0x0000, WORD_LEN); 	// Added by Aptina RMA #3594
    if (rc < 0)
    	return rc;

    rc = mt9d112_i2c_write(mt9d112_client->addr,0x3812, 0x2124, WORD_LEN); // OTPM_CFG
    if (rc < 0)
    	return rc;

    rc = mt9d112_i2c_write(mt9d112_client->addr,0xE02A, 0x0001, WORD_LEN); 	// IO_NV_MEM_COMMAND
    if (rc < 0)
    	return rc;

    ////POLL_REG=0x800A,0x00,!=0x07,DELAY=10,TIMEOUT=100	//Wait for the core ready
    //POLL_FIELD= IO_NV_MEM_STATUS, IO_NV_MEM_STATUS!=0xC1,DELAY=100,TIMEOUT=50 //5 sec
    //DELAY =300
    do{
      msleep(40);

      rc = mt9d112_i2c_read(mt9d112_client->addr, 0xE023, &OTPM_Status, BYTE_LEN);
      if(rc < 0)
          return rc;

      printk("mt9p111  lc_data = 0x%x\n", OTPM_Status);
		}while((OTPM_Status&0x40) == 0);

    rc = mt9d112_i2c_write(mt9d112_client->addr,0xD004, 0x04, BYTE_LEN); 	// PGA_SOLUTION
    if (rc < 0)
    	return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0xD006, 0x0008, WORD_LEN); 	// PGA_ZONE_ADDR_0
    if (rc < 0)
    	return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0xD005, 0x00, BYTE_LEN); 	// PGA_CURRENT_ZONE
    if (rc < 0)
    	return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0xD002, 0x8002, WORD_LEN); 	// PGA_ALGO
    if (rc < 0)
    	return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x3210, 0x49B8, WORD_LEN); 	// COLOR_PIPELINE_CONTROL
    if (rc < 0)
    	return rc;

    msleep(100);

    // Read from Sensor for indoor-LSC
    rc = mt9d112_i2c_read(mt9d112_client->addr,0x3644, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.g1_p0q2 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:g1_p0q2=0x%x\n", pgaOutdoor.g1_p0q2);

    rc = mt9d112_i2c_read(mt9d112_client->addr,0x364E, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.r_p0q2 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:r_p0q2=0x%x\n", pgaOutdoor.r_p0q2);

    rc = mt9d112_i2c_read(mt9d112_client->addr,0x3656, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.b_p0q1 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:b_p0q1=0x%x\n", pgaOutdoor.b_p0q1);

    rc = mt9d112_i2c_read(mt9d112_client->addr,0x3658, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.b_p0q2 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:b_p0q2=0x%x\n", pgaOutdoor.b_p0q2);

    rc = mt9d112_i2c_read(mt9d112_client->addr,0x3662, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.g2_p0q2 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:g2_p0q2=0x%x\n", pgaOutdoor.g2_p0q2);

    rc = mt9d112_i2c_read(mt9d112_client->addr,0x368A, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.r_p1q0 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:r_p1q0=0x%x\n", pgaOutdoor.r_p1q0);

    rc = mt9d112_i2c_read(mt9d112_client->addr,0x3694, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.b_p1q0 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:b_p1q0=0x%x\n", pgaOutdoor.b_p1q0);

    rc = mt9d112_i2c_read(mt9d112_client->addr,0x36C0, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.g1_p2q0 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:g1_p2q0=0x%x\n", pgaOutdoor.g1_p2q0);

    rc = mt9d112_i2c_read(mt9d112_client->addr,0x36CA, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.r_p2q0 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:r_p2q0=0x%x\n", pgaOutdoor.r_p2q0);

    rc = mt9d112_i2c_read(mt9d112_client->addr,0x36D4, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.b_p2q0 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:b_p2q0=0x%x\n", pgaOutdoor.b_p2q0);

    rc = mt9d112_i2c_read(mt9d112_client->addr,0x36DE, &reg_data, WORD_LEN);
    if (rc < 0) return rc;
    else pgaOutdoor.g2_p2q0 = RegistertoFloat16(reg_data);
    CDBG_ZS("dynPGA_init read:g2_p2q0=0x%x\n", pgaOutdoor.g2_p2q0);

/*
    // Calculation outdoor LSC from Indoor LSC
    pgaIndoor.g1_p0q2 = pgaOutdoor.g1_p0q2 * 9 / 10;
    pgaIndoor.r_p0q2 = pgaOutdoor.r_p0q2 * 11 / 10;

    pgaIndoor.b_p0q1 = pgaOutdoor.b_p0q1 + 3808;

    pgaIndoor.b_p0q2 = pgaOutdoor.b_p0q2 * 111 / 100;
    pgaIndoor.g2_p0q2 = pgaOutdoor.g2_p0q2 * 9 / 10;

    pgaIndoor.r_p1q0 = pgaOutdoor.r_p1q0 + 616;
    pgaIndoor.b_p1q0 = pgaOutdoor.b_p1q0 + 5243;

    pgaIndoor.g1_p2q0 = pgaOutdoor.g1_p2q0 * 23 / 25;
    pgaIndoor.r_p2q0 = pgaOutdoor.r_p2q0 * 231 / 200;
    pgaIndoor.b_p2q0 = pgaOutdoor.b_p2q0 * 127 / 125;
    pgaIndoor.g2_p2q0 = pgaOutdoor.g2_p2q0 * 23 / 25;
*/
    // Calculation outdoor LSC from Indoor LSC
    pgaIndoor.g1_p0q2 = pgaOutdoor.g1_p0q2 * 7792 / 10000;
    pgaIndoor.r_p0q2 = pgaOutdoor.r_p0q2 * 7496 / 10000;

    pgaIndoor.b_p0q1 = pgaOutdoor.b_p0q1 * 11921  / 10000;

    pgaIndoor.b_p0q2 = pgaOutdoor.b_p0q2 * 9392  / 10000;
    pgaIndoor.g2_p0q2 = pgaOutdoor.g2_p0q2 * 9481  / 10000;

    pgaIndoor.r_p1q0 = pgaOutdoor.r_p1q0 * 9638  / 10000;
    pgaIndoor.b_p1q0 = pgaOutdoor.b_p1q0 * 8142  / 10000;

    pgaIndoor.g1_p2q0 = pgaOutdoor.g1_p2q0 * 9563  / 10000;
    pgaIndoor.r_p2q0 = pgaOutdoor.r_p2q0 * 7870  / 10000;
    pgaIndoor.b_p2q0 = pgaOutdoor.b_p2q0 * 8993  / 10000;
    pgaIndoor.g2_p2q0 = pgaOutdoor.g2_p2q0 * 8767  / 10000;

    // just for changing lens-shading parameters from indoor to outdoor
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x3644, Float16toRegister(pgaIndoor.g1_p0q2), WORD_LEN);
    if (rc < 0) return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x364E, Float16toRegister(pgaIndoor.r_p0q2), WORD_LEN);
    if (rc < 0) return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x3656, Float16toRegister(pgaIndoor.b_p0q1), WORD_LEN);
    if (rc < 0) return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x3658, Float16toRegister(pgaIndoor.b_p0q2), WORD_LEN);
    if (rc < 0) return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x3662, Float16toRegister(pgaIndoor.g2_p0q2), WORD_LEN);
    if (rc < 0) return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x368A, Float16toRegister(pgaIndoor.r_p1q0), WORD_LEN);
    if (rc < 0) return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x3694, Float16toRegister(pgaIndoor.b_p1q0), WORD_LEN);
    if (rc < 0) return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x36C0, Float16toRegister(pgaIndoor.g1_p2q0), WORD_LEN);
    if (rc < 0) return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x36CA, Float16toRegister(pgaIndoor.r_p2q0), WORD_LEN);
    if (rc < 0) return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x36D4, Float16toRegister(pgaIndoor.b_p2q0), WORD_LEN);
    if (rc < 0) return rc;
    rc = mt9d112_i2c_write(mt9d112_client->addr,0x36DE, Float16toRegister(pgaIndoor.g2_p2q0), WORD_LEN);
    if (rc < 0) return rc;
    CDBG_ZS("finish mt9p111_dynPGA_init Setting.\n");

    return 1;
}

static long mt9p111_dynPGA_for_ThreeLSC_Parameters_init(void)
{
		int32_t rc;
		unsigned short OTPM_Status;
		//[Load PGA settings from OTPM with APGA function enabled 2]
		// for using 3 PGA settings for all CT conditions
		rc = mt9d112_i2c_write(mt9d112_client->addr,0x381C, 0x0000, WORD_LEN); // OTPM_CFG2
		if (rc < 0)
			return rc;

		rc = mt9d112_i2c_write(mt9d112_client->addr, 0x3812, 0x2124, WORD_LEN); // OTPM_CFG
		if (rc < 0)
			return rc;

		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xE02A, 0x0001, WORD_LEN);                  //Probe
		//;POLL_FIELD= IO_NV_MEM_STATUS, IO_NVMEM_STAT_OTPM_AVAIL==1,DELAY=100,TIMEOUT=5  //5 sec
		if (rc < 0)
			return rc;

		do{
      			msleep(40);

      			rc = mt9d112_i2c_read(mt9d112_client->addr, 0xE023, &OTPM_Status, BYTE_LEN);
      			if(rc < 0)
          			return rc;
      			printk("mt9p111  lc_data = 0x%x\n", OTPM_Status);
		}while((OTPM_Status&0x40) == 0);

		msleep(150);

		// enalbe PGA setting
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD004, 0x04, BYTE_LEN);            // PGA_SOLUTION
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD006, 0x0200, WORD_LEN);         // PGA_ZONE_ADDR_0 -- this is the address of PGA Zone 0 DNP
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD008, 0x0100, WORD_LEN);         // PGA_ZONE_ADDR_1 -- this is the address of PGA Zone 1 CWF
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD00A, 0x0008, WORD_LEN);         // PGA_ZONE_ADDR_2 -- this is the address of PGA Zone 2 A
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD005, 0x00, BYTE_LEN);            // PGA_CURRENT_ZONE -- Specify PGA Zone to 0~2
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD002, 0x8007, WORD_LEN);         // PGA_ALGO

		// set up APGA parameter according to the AWB result
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD00C, 0x03, BYTE_LEN);            // PGA_NO_OF_ZONES
		// below settings is used for APGA and related to "AWB_CURRENT_CCM_POSITION"
		// User may need to fine tune according to the AWB parameter settings
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD00D, 0x7C, BYTE_LEN);            // PGA_ZONE_LOW_0 -- low limit for higher CT condition, i.e. D65 or DNP
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD00E, 0x18, BYTE_LEN);             // PGA_ZONE_LOW_1 -- low limit for middle CT condition, i.e. CWF or TL84
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD00F, 0x00, BYTE_LEN);             // PGA_ZONE_LOW_2 -- low limit for low CT condition, i.e. A-light
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD011, 0x7F, BYTE_LEN);             // PGA_ZONE_HIGH_0 -- high limit for higher CT condition, i.e. D65 or DNP
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD012, 0x7B, BYTE_LEN);            // PGA_ZONE_HIGH_1 -- high limit for middle CT condition, i.e. CWF or TL84
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD013, 0x17, BYTE_LEN);            // PGA_ZONE_HIGH_2 -- high limit for low CT condition, i.e. A-light
		// PGA Brightness setting
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD014, 0x00, BYTE_LEN);
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xD015, 0x0200, WORD_LEN);

		return 0;
}

static long mt9p111_dynPGA_selection(void)
{
	long rc;
	uint16_t reg_data;

	rc = mt9d112_i2c_read(mt9d112_client->addr, 0x3012, &reg_data, WORD_LEN);  // The read sensor COARSE_INTEGRATION_TIME value

	if (rc < 0)
		return rc;

       CDBG_ZS("mt9p111_dynPGA_selection. 0x3012 = 0x%x\n", reg_data);

	if (reg_data >= 0x200) {
		mt9p111_dynPGA_init(); // The apply indoor LSC data
		CDBG_ZS("finish mt9p111_dynPGA_init Setting.0x3012 = 0x%x\n", reg_data);
	}
	else {
		CDBG_ZS("finish not using mt9p111_dynPGA_init Setting. 0x3012 = 0x%x\n", reg_data);
	}

	return 0;
}

static long mt9d112_reg_init(void)
{
	long rc;

	unsigned short Read_OTPM_ID;
	unsigned short OTPM_Status;

	CDBG_ZS("mt9d112_reg_init\n");

	if(use_old_module)
	{
		// init
		rc = mt9d112_i2c_write_table(&mt9d112_regs.init_tbl[0],
						mt9d112_regs.init_tbl_size);
	}
	else
	{
		//init table 1
		rc = mt9d112_i2c_write_table(&mt9d112_regs.init_tbl1[0],
						mt9d112_regs.init_tbl1_size);
		//init patch table 1
		rc = mt9p111_i2c_write_table_burst_mode(&mt9d112_regs.init_patchbust_mode_tbl[0],
		                            mt9d112_regs.init_patchbust_mode_tbl_size);

		//init table 2
		rc = mt9d112_i2c_write_table(&mt9d112_regs.init_tbl2[0],
						mt9d112_regs.init_tbl2_size);
	}

	if (rc < 0)
		return rc;

	CDBG_ZS("mt9d112_reg_init:success!\n");

	if(use_old_module)
	{
		mdelay(100);

		//fiddle add for Module Selection begin
		//Read the OTPM_ID
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0x381C, 0x0000, WORD_LEN); // OTPM_CFG2
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0x3812, 0x2124, WORD_LEN); // OTPM_CFG
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xE024, 0x0000, WORD_LEN); // IO_NV_MEM_ADDR
		rc = mt9d112_i2c_write(mt9d112_client->addr, 0xE02A, 0xA010, WORD_LEN);// IO_NV_MEM_COMMAND ? READ Command
		//mdelay(100);

		do{
		      msleep(40);

		      rc = mt9d112_i2c_read(mt9d112_client->addr, 0xE023, &OTPM_Status, BYTE_LEN);
		      if(rc < 0)
		          return rc;
		      printk("mt9p111  lc_data = 0x%x\n", OTPM_Status);
		}while((OTPM_Status&0x40) == 0);

		mdelay(150);
		rc = mt9d112_i2c_read(mt9d112_client->addr, 0xE026, &Read_OTPM_ID, WORD_LEN); // ID Read
		printk("mt9p111 useDNPforOTP = 0x%x\n", Read_OTPM_ID); // for debug
		mdelay(100);

		if (Read_OTPM_ID == 0x5103) // 0x5103 means OTP light source is 3 parameters(DNP, CWF, A)
		{
			printk(KERN_ERR "fiddle --- 111 Read_OTPM_ID = %X\n", Read_OTPM_ID);
			mt9p111_dynPGA_for_ThreeLSC_Parameters_init();
		}
		else // 0x5100 means 1 parameter LSC data
		{
			printk(KERN_ERR "fiddle --- 222 Read_OTPM_ID = %X\n", Read_OTPM_ID);
			mt9p111_dynPGA_selection(); // LSC selection
		}
	}

	printk("mt9p111 ffffffffffffffffffffffinish OTPM\n");

	return 0;
}

static long mt9d112_set_effect(int mode, int effect)
{
	long rc = 0;
	{
        if(effect_mode!=effect)
        {
    	CDBG_ZS("mt9d112_set_effect:effect[%d] effect_mode=[%d]\n", effect,effect_mode);
#if 1
        effect_mode=effect;
    	switch (effect) {
    	case CAMERA_EFFECT_OFF:
    		rc = mt9d112_i2c_write_table(&mt9d112_regs.EFFECT_OFF_tbl[0],
    						mt9d112_regs.EFFECT_OFF_tbl_size);
    		//mdelay(200);
    		break;

    	case CAMERA_EFFECT_MONO:
    		rc = mt9d112_i2c_write_table(&mt9d112_regs.EFFECT_MONO_tbl[0],
    						mt9d112_regs.EFFECT_MONO_tbl_size);
    		//mdelay(200);
    		break;

    	case CAMERA_EFFECT_SEPIA:
    		rc = mt9d112_i2c_write_table(&mt9d112_regs.EFFECT_SEPIA_tbl[0],
    						mt9d112_regs.EFFECT_SEPIA_tbl_size);
    		//mdelay(200);
    		break;

    	case CAMERA_EFFECT_NEGATIVE:
    		rc = mt9d112_i2c_write_table(&mt9d112_regs.EFFECT_NEGATIVE_tbl[0],
    						mt9d112_regs.EFFECT_NEGATIVE_tbl_size);
    		//mdelay(200);
    		break;

    	case CAMERA_EFFECT_SOLARIZE:
    		rc = mt9d112_i2c_write_table(&mt9d112_regs.EFFECT_SOLARIZE_tbl[0],
    						mt9d112_regs.EFFECT_SOLARIZE_tbl_size);
    		//mdelay(200);
    		break;

    	default:
    		return -EINVAL;
    		}
    	}
    	else
    	{
    		//CDBG_ZS("mt9d112_set_effect:same effect[%d]not set again!\n",effect);

    	}
	}
#endif
	return rc;
}


static long mt9d112_set_wb(int mode, int wb)
{
	long rc = 0;
#if 1
    {
    	if(wb_mode!=wb)
        {
    	CDBG_ZS("mt9d112_set_wb:wb[%d] wb_mode[%d]\n", wb,wb_mode);

    	wb_mode = wb;
    	switch(wb){
    	case CAMERA_WB_AUTO:
        	rc = mt9d112_i2c_write_table(&mt9d112_regs.awb_tbl[0],
        					mt9d112_regs.awb_tbl_size);

    		//mdelay(200);
    		break;
    	case CAMERA_WB_INCANDESCENT :
        	rc = mt9d112_i2c_write_table(&mt9d112_regs.MWB_INCANDESCENT_tbl[0],
        					mt9d112_regs.MWB_INCANDESCENT_tbl_size);

    		//mdelay(200);
    		break;
    	case CAMERA_WB_FLUORESCENT:
        	rc = mt9d112_i2c_write_table(&mt9d112_regs.MWB_FLUORESCENT_tbl[0],
        					mt9d112_regs.MWB_FLUORESCENT_tbl_size);

    		//mdelay(200);
    		break;
    	case CAMERA_WB_DAYLIGHT:
        	rc = mt9d112_i2c_write_table(&mt9d112_regs.MWB_Day_light_tbl[0],
        					mt9d112_regs.MWB_Day_light_tbl_size);

    		//mdelay(200);
    		break;
    	case CAMERA_WB_CLOUDY_DAYLIGHT:
        	rc = mt9d112_i2c_write_table(&mt9d112_regs.MWB_Cloudy_tbl[0],
        					mt9d112_regs.MWB_Cloudy_tbl_size);
    		//mdelay(200);
    		break;
    	default:
    		return -EINVAL;
    		}
    	}
    	else
    	{
    		//CDBG_ZS("mt9d112_set_wb:same wb[%d]not set again!\n",wb);

    	}
    }

#endif
	return rc;
}


static int32_t mt9d112_set_default_focus(int32_t focusmode, int scene)
{
	long rc = 0;
#if 1
	uint16_t icount = 0, af_status = 0, af_status2 = 0;
	//uint16_t , itotal = 0;

	CDBG_ZS("mt9d112_set_default_focus Enter.................................................. focusmode=0x%x scene=0x%x\n",focusmode,scene);

	switch(focusmode){
	case AF_MODE_AUTO:

    	rc = mt9d112_i2c_write_table(&mt9d112_regs.VCM_Enable_full_scan_tbl[0],
    					mt9d112_regs.VCM_Enable_full_scan_tbl_size);
    	mdelay(10);
	//AF Triger
	rc = mt9d112_i2c_write_table(&mt9d112_regs.AF_Trigger_tbl[0],
    					mt9d112_regs.AF_Trigger_tbl_size);
	#if 0
        if(scene==CAMERA_BESTSHOT_NIGHT)
            itotal=200;
        else
            itotal=30;
    	for(icount=0; icount<itotal; icount++)
    	{
    		rc = mt9d112_i2c_read(mt9d112_client->addr,
    			MT9D112_AF_STATUS, &af_status, WORD_LEN);
    		//CDBG_ZS("mt9d112_set_default_focus:AF_MODE_AUTO af_status = 0x%x  icount=%d\n", af_status,icount);
    		if(af_status&(0x1<<4))
    		{
			CDBG_ZS("mt9d112_set_default_focus:AF_MODE_AUTO 1 waiting done!!!!!!!!!!!!!!!!!!!!!!!!  af_status = 0x%x  icount=%d\n", af_status,icount);
    			return rc;
    		}
    		msleep(100);
    	}
		CDBG_ZS("mt9d112_set_default_focus:AF_MODE_AUTO 2 waiting done!!!!!!!!!!!!!!!!!!!!!!!!  af_status = 0x%x  icount=%d\n", af_status,icount);
	#else
	do{
	    msleep(200);
		icount++;

		rc = mt9d112_i2c_read(mt9d112_client->addr,	MT9D112_AF_STATUS, &af_status, WORD_LEN);

		if(rc < 0)
			return 0;

		//CDBG_ZS("mt9d112_set_default_focus:AF_MODE_AUTO 1 waiting done!!!!!!!!!!!!!!!!!!!!!!!!  af_status = 0x%x  icount=%d\n", af_status,icount);

	}while(((af_status & 0x0010)==0)&&(icount <= 13));

	CDBG_ZS("mt9d112_set_default_focus:AF_MODE_AUTO 2 waiting done!!!!!!!!!!!!!!!!!!!!!!!!  af_status = 0x%x  icount=%d\n", af_status,icount);

	rc = mt9d112_i2c_read(mt9d112_client->addr,	MT9D112_AF_STATUS, &af_status2, WORD_LEN);

	if(rc < 0)
		return 0;

	if(af_status2&0x8000){
		CDBG_ZS("mt9d112_set_default_focus:AF_MODE_AUTO waiting done focus ERROR !!!!!!!!!!!!!!!!!!!!!!!!  af_status2 = 0x%x AF Error !!!\n", af_status2);
		//rc = - 1;
	}
	#endif
	break;

	case AF_MODE_MACRO:
    	rc = mt9d112_i2c_write_table(&mt9d112_regs.Foucs_Marco_tbl[0],
    					mt9d112_regs.Foucs_Marco_tbl_size);
    	mdelay(10);
    	rc = mt9d112_i2c_write_table(&mt9d112_regs.AF_Trigger_tbl[0],
    					mt9d112_regs.AF_Trigger_tbl_size);

	do{
		rc = mt9d112_i2c_read(mt9d112_client->addr,
			MT9D112_AF_STATUS, &af_status, WORD_LEN);
		if(rc < 0)
			return 0;

		//CDBG_ZS("mt9d112_set_default_focus:AF_MODE_MACRO 1 waiting done!!!!!!!!!!!!!!!!!!!!!!!!  af_status = 0x%x  icount=%d\n", af_status,icount);

	}while((af_status & 0x0010)==0);

	CDBG_ZS("mt9d112_set_default_focus:AF_MODE_MACRO 2 waiting done!!!!!!!!!!!!!!!!!!!!!!!!  af_status = 0x%x  icount=%d\n", af_status,icount);

	rc = mt9d112_i2c_read(mt9d112_client->addr,	MT9D112_AF_STATUS, &af_status2, WORD_LEN);

	if(rc < 0)
		return 0;

	if(af_status2&0x8000){
		CDBG_ZS("mt9d112_set_default_focus:AF_MODE_MACRO waiting done focus ERROR !!!!!!!!!!!!!!!!!!!!!!!!  af_status2 = 0x%x AF Error !!! \n", af_status2);
		//rc = - 1;
	}
	break;

	case AF_MODE_NORMAL:
    	rc = mt9d112_i2c_write_table(&mt9d112_regs.Foucs_Infinity_tbl[0],
    					mt9d112_regs.Foucs_Infinity_tbl_size);
    	mdelay(10);
    	rc = mt9d112_i2c_write_table(&mt9d112_regs.AF_Trigger_tbl[0],
    					mt9d112_regs.AF_Trigger_tbl_size);

	do{
		rc = mt9d112_i2c_read(mt9d112_client->addr,
    			MT9D112_AF_STATUS, &af_status, WORD_LEN);
		if(rc < 0)
			return 0;

		//CDBG_ZS("mt9d112_set_default_focus:AF_MODE_NORMAL 1 waiting done!!!!!!!!!!!!!!!!!!!!!!!!  af_status = 0x%x  icount=%d\n", af_status,icount);

	}while((af_status & 0x0010)==0);

	CDBG_ZS("mt9d112_set_default_focus:AF_MODE_NORMAL 2 waiting done!!!!!!!!!!!!!!!!!!!!!!!!  af_status = 0x%x  icount=%d\n", af_status,icount);

	rc = mt9d112_i2c_read(mt9d112_client->addr,	MT9D112_AF_STATUS, &af_status2, WORD_LEN);

	if(rc < 0)
		return 0;

	if(af_status2&0x8000){
		CDBG_ZS("mt9d112_set_default_focus:AF_MODE_NORMAL waiting done  focus ERROR !!!!!!!!!!!!!!!!!!!!!!!!  af_status2 = 0x%x AF Error !!! \n", af_status2);
		//rc = - 1;
	}
	break;

	default:
		CDBG_ZS("mt9d112_set_default_focus: others focusmode=0x%x\n",focusmode);
		return -EINVAL;
    }
#endif
	return rc;
}

static int32_t mt9d112_set_scene(int scene)
{
	long rc = 0;
	if(scene_mode!=scene)
    {

	CDBG_ZS("mt9d112_set_scene:scene=[%d] scene_mode=[%d]\n",scene,scene_mode);
	scene_mode = scene;

#if 1
	switch (scene) {
	case CAMERA_BESTSHOT_OFF:
		rc = mt9d112_i2c_write_table(&mt9d112_regs.SCENE_AUTO_tbl[0],
							mt9d112_regs.SCENE_AUTO_tbl_size);
		//mdelay(200);
		break;

	case CAMERA_BESTSHOT_NIGHT:
		rc = mt9d112_i2c_write_table(&mt9d112_regs.SCENE_NIGHT_tbl[0],
							mt9d112_regs.SCENE_NIGHT_tbl_size);
		//mdelay(200);
		break;

	default:
		return -EINVAL;
		}
	}
	else
	{
		//CDBG_ZS("mt9d112_set_scene:same scene[%d]not set again!\n",scene);

	}
    //CDBG_ZS("rc=%ld!\n",rc);

#endif
	return rc;
}
static int32_t mt9d112_set_brightness(int brightness)
{
	long rc = 0;
	//if(brightness_mode!=brightness)
	//APP not set CAMERA_BRIGHTNESS_H0 when wakeup, so brightness always equals brightness_mode! the true brightness is initialsequence not the value before power down when preview!
	if(1)
    {
	CDBG_ZS("mt9d112_set_brightness:brightness=[%d] brightness_mode=[%d]\n",brightness,brightness_mode);

	brightness_mode = brightness;

#if 1
	switch (brightness) {
	case CAMERA_BRIGHTNESS_L3:
		//printk(KERN_ERR "fiddle --- in CAMERA_BRIGHTNESS_L3!!!\n");
		rc = mt9d112_i2c_write_table(&mt9d112_regs.BRIGHTNESS_L3_tbl[0],
							mt9d112_regs.BRIGHTNESS_L3_tbl_size);
		break;
	case CAMERA_BRIGHTNESS_L2:
		//printk(KERN_ERR "fiddle --- in CAMERA_BRIGHTNESS_L2!!!\n");
		rc = mt9d112_i2c_write_table(&mt9d112_regs.BRIGHTNESS_L2_tbl[0],
							mt9d112_regs.BRIGHTNESS_L2_tbl_size);
		break;

	case CAMERA_BRIGHTNESS_L1:
		//printk(KERN_ERR "fiddle --- in CAMERA_BRIGHTNESS_L1!!!\n");
		rc = mt9d112_i2c_write_table(&mt9d112_regs.BRIGHTNESS_L1_tbl[0],
							mt9d112_regs.BRIGHTNESS_L1_tbl_size);
		//mdelay(200);
		break;

	case CAMERA_BRIGHTNESS_H0:
		//printk(KERN_ERR "fiddle --- in CAMERA_BRIGHTNESS_H0!!!\n");
		rc = mt9d112_i2c_write_table(&mt9d112_regs.BRIGHTNESS_H0_tbl[0],
							mt9d112_regs.BRIGHTNESS_H0_tbl_size);
		//mdelay(200);
		break;

	case CAMERA_BRIGHTNESS_H1:
		//printk(KERN_ERR "fiddle --- in CAMERA_BRIGHTNESS_H1!!!\n");
		rc = mt9d112_i2c_write_table(&mt9d112_regs.BRIGHTNESS_H1_tbl[0],
							mt9d112_regs.BRIGHTNESS_H1_tbl_size);
		//mdelay(200);
		break;

	case CAMERA_BRIGHTNESS_H2:
		//printk(KERN_ERR "fiddle --- in CAMERA_BRIGHTNESS_H2!!!\n");
		rc = mt9d112_i2c_write_table(&mt9d112_regs.BRIGHTNESS_H2_tbl[0],
							mt9d112_regs.BRIGHTNESS_H2_tbl_size);
		//mdelay(200);
		break;

	case CAMERA_BRIGHTNESS_H3:
		//printk(KERN_ERR "fiddle --- in CAMERA_BRIGHTNESS_H3!!!\n");
		rc = mt9d112_i2c_write_table(&mt9d112_regs.BRIGHTNESS_H3_tbl[0],
							mt9d112_regs.BRIGHTNESS_H3_tbl_size);
		//mdelay(200);
		break;

	default:
		//printk(KERN_ERR "fiddle --- in default!!!\n");
		return -EINVAL;
		}
	}
	else
	{
		//CDBG_ZS("mt9d112_set_brightness:same brightness[%d]not set again!\n",brightness);

	}

#endif
	return rc;
}

#ifdef FLASH_LED
static int32_t mt9d112_set_flash(int flash)
{
	long rc = 0;

	if(Flash_Flag != flash)
	{
		CDBG_ZS("mt9d112_set_flash:flash=[%d]\n",flash);

		switch (flash) {
		case LED_MODE_OFF:
	    		Flash_Flag=0;
			break;
		case LED_MODE_AUTO:
	       	Flash_Flag=1;
			break;
		case LED_MODE_ON:
		    	Flash_Flag=2;
			break;
	    	case LED_MODE_TORCH:
	    		Flash_Flag=3;
			break;
		default:
			return -EINVAL;
		}

		//fiddle add begin
		#ifdef FLASH_LED
		if (3 == Flash_Flag)
		{
			// Torch Mode need LED Always On!
			//pwm_config(led_dat->pwm, 2000, 2000);
			//pwm_enable(led_dat->pwm);
			#ifdef CONFIG_LEDS_PMIC8058
			pm8058_set_flash_led_current(PMIC8058_ID_FLASH_LED_1, 80);
			#endif
		}
		else
		{
			// Now we are at Flash Off/Auto Flash/Always on Mode
			//pwm_config(led_dat->pwm, 2000, 2000);
			//pwm_disable(led_dat->pwm);
			#ifdef CONFIG_LEDS_PMIC8058
			pm8058_set_flash_led_current(PMIC8058_ID_FLASH_LED_1, 0);
			#endif
		}
		#endif
		//fiddle add end
	}
	return rc;
}
#endif
static long mt9d112_set_sensor_mode(int mode)
{
	uint16_t reg = 0;

   	 int i;
	long rc = 0;

	#ifdef FLASH_LED
	uint16_t target_brighness = 0;
	uint16_t real_brighness = 0;
	#endif

	CDBG_ZS("mt9d112_set_sensor_mode:mode[%d]\n", mode);

	switch (mode)
	{
		case SENSOR_PREVIEW_MODE:

			rc = mt9d112_i2c_write_table(&mt9d112_regs.preview_tbl[0],
    					mt9d112_regs.preview_tbl_size);

			//msleep(100);

			CDBG_ZS("mt9d112_set_sensor_mode: write preview_tbl finish!!!!!!!!!!!!!!!!  \n");

			break;

		case SENSOR_SNAPSHOT_MODE:
			// fiddle add begin
		#ifdef FLASH_LED
			if (1 == Flash_Flag) // Auto
			{
				mt9d112_i2c_read(mt9d112_client->addr,0xA409, &target_brighness, WORD_LEN);
				mt9d112_i2c_read(mt9d112_client->addr,0xB804, &real_brighness, WORD_LEN);
				if (real_brighness < target_brighness)
				{
					//pwm_config(led_dat->pwm, 2000, 2000);
					//pwm_enable(led_dat->pwm);
					#ifdef CONFIG_LEDS_PMIC8058
					pm8058_set_flash_led_current(PMIC8058_ID_FLASH_LED_1, 80);
					#endif
				}
			}
			else if ((2 == Flash_Flag) || (3 == Flash_Flag)) // On or Torch
			{
				//pwm_config(led_dat->pwm, 2000, 2000);
				//pwm_enable(led_dat->pwm);
				#ifdef CONFIG_LEDS_PMIC8058
				pm8058_set_flash_led_current(PMIC8058_ID_FLASH_LED_1, 80);
				#endif
			}
		#endif
		// fiddle add end
			rc = mt9d112_i2c_write_table(&mt9d112_regs.snapshot_tbl[0],
    					mt9d112_regs.snapshot_tbl_size);

			for(i=0; i<30; i++)
			{
				mt9d112_i2c_write(mt9d112_client->addr,0x098E,0x8405,WORD_LEN);
				mt9d112_i2c_read(mt9d112_client->addr,0x8405, &reg, BYTE_LEN);
				if(reg==0x07)
				{
					CDBG_ZS("mt9d112_set_sensor_mode, wait sensor snapshot finish 1 !!!!!!!!!!!!!!!!!!!!!!!!!!!! :0x8405 = 0x%x  i=%d\n", reg,i);
					break;
				}
				msleep(50);
			}

			CDBG_ZS("mt9d112_set_sensor_mode, wait sensor snapshot finish 2 !!!!!!!!!!!!!!!!!!!!!!!!!!!! :0x8405 = 0x%x  i=%d\n", reg,i);

			msleep(50);

		#ifdef FLASH_LED
		// fiddle add begin
			if (3 != Flash_Flag) // Torch Mode should always on
			{
				//pwm_config(led_dat->pwm, 2000, 2000);
				//pwm_disable(led_dat->pwm);
				#ifdef CONFIG_LEDS_PMIC8058
				//pm8058_set_flash_led_current(PMIC8058_ID_FLASH_LED_1, 0); //masked by yuan.yang@sim.com 20110906
				#endif
			}
		// fiddle add end
		#endif
			break;

		case SENSOR_RAW_SNAPSHOT_MODE:
		// fiddle add begin
		#ifdef FLASH_LED
			if (1 == Flash_Flag) // Auto
			{
				mt9d112_i2c_read(mt9d112_client->addr,0xA409, &target_brighness, WORD_LEN);
				mt9d112_i2c_read(mt9d112_client->addr,0xB804, &real_brighness, WORD_LEN);
				if (real_brighness < target_brighness)
				{
					//pwm_config(led_dat->pwm, 2000, 2000);
					//pwm_enable(led_dat->pwm);
					#ifdef CONFIG_LEDS_PMIC8058
					pm8058_set_flash_led_current(PMIC8058_ID_FLASH_LED_1, 80);
					#endif
				}
			}
			else if ((2 == Flash_Flag) || (3 == Flash_Flag)) // On or Torch
			{
				//pwm_config(led_dat->pwm, 2000, 2000);
				//pwm_enable(led_dat->pwm);
				#ifdef CONFIG_LEDS_PMIC8058
				pm8058_set_flash_led_current(PMIC8058_ID_FLASH_LED_1, 80);
				#endif
			}
		#endif
		// fiddle add end

			rc = mt9d112_i2c_write_table(&mt9d112_regs.snapshot_tbl[0],
    					mt9d112_regs.snapshot_tbl_size);
			for(i=0; i<30; i++)
			{
				mt9d112_i2c_write(mt9d112_client->addr,0x098E,0x8405,WORD_LEN);
				mt9d112_i2c_read(mt9d112_client->addr,0x8405, &reg, BYTE_LEN);
				CDBG_ZS("mt9d112_set_sensor_mode:0x8405 = 0x%x  i=%d\n", reg,i);
				if(reg==0x07)
					break;
				msleep(100);
			}
			msleep(50);

		#ifdef FLASH_LED
		// fiddle add begin
			if (3 != Flash_Flag) // Torch Mode should always on
			{
				//pwm_config(led_dat->pwm, 2000, 2000);
				//pwm_disable(led_dat->pwm);
				#ifdef CONFIG_LEDS_PMIC8058
				pm8058_set_flash_led_current(PMIC8058_ID_FLASH_LED_1, 0);
				#endif
			}
		// fiddle add end
		#endif
			break;

		default:
			return -EINVAL;
	}

	return rc;
}

static int mt9d112_sensor_init_probe(const struct msm_camera_sensor_info *data)
{
	uint16_t model_id = 0;
	int rc = 0;

	unsigned short Read_OTPM_ID;
	unsigned short OTPM_Status;

	CDBG_ZS("mt9d112_sensor_init_probe \n");

	rc = mt9d112_reset(data);
	if (rc < 0)
		goto init_probe_fail;


	/* Read the Model ID of the sensor */
	rc = mt9d112_i2c_read(mt9d112_client->addr,
		REG_MT9D112_MODEL_ID, &model_id, WORD_LEN);
	if (rc < 0)
		goto init_probe_fail;

	CDBG_ZS("mt9d112_sensor_init_probe:mt9d112 model_id = 0x%x\n", model_id);

	/* Check if it matches it with the value in Datasheet */
	if (model_id != MT9D112_MODEL_ID) {
		rc = -EINVAL;
		goto init_probe_fail;
	}

	// init
	rc = mt9d112_i2c_write_table(&mt9d112_regs.init_tbl[0],
					mt9d112_regs.init_tbl_size);

	//Read the OTPM_ID
	rc = mt9d112_i2c_write(mt9d112_client->addr, 0x381C, 0x0000, WORD_LEN); // OTPM_CFG2
	rc = mt9d112_i2c_write(mt9d112_client->addr, 0x3812, 0x2124, WORD_LEN); // OTPM_CFG
	rc = mt9d112_i2c_write(mt9d112_client->addr, 0xE024, 0x0000, WORD_LEN); // IO_NV_MEM_ADDR
	rc = mt9d112_i2c_write(mt9d112_client->addr, 0xE02A, 0xA010, WORD_LEN);// IO_NV_MEM_COMMAND ? READ Command

	do{
		msleep(40);

		rc = mt9d112_i2c_read(mt9d112_client->addr, 0xE023, &OTPM_Status, BYTE_LEN);
		if(rc < 0)
			return rc;
		printk("mt9p111  lc_data = 0x%x\n", OTPM_Status);
	}while((OTPM_Status&0x40) == 0);

	mdelay(150);
	rc = mt9d112_i2c_read(mt9d112_client->addr, 0xE026, &Read_OTPM_ID, WORD_LEN); // ID Read
	printk("mt9p111 useDNPforOTP = 0x%x\n", Read_OTPM_ID); // for debug
	mdelay(100);

	if (Read_OTPM_ID == 0x5103) // 0x5103 means OTP light source is 3 parameters(DNP, CWF, A)
	{
		printk(KERN_ERR "fiddle --- 111 Read_OTPM_ID = %X\n", Read_OTPM_ID);
		use_old_module = 0;
	}
	else // 0x5100 means 1 parameter LSC data
	{
		printk(KERN_ERR "fiddle --- 222 Read_OTPM_ID = %X\n", Read_OTPM_ID);
		use_old_module = 1;
	}

	return rc;

init_probe_fail:

	CDBG_ZS("mt9d112_sensor_init_probe:failed \n");
	return rc;
}

static int mt9d112_sensor_init_start(const struct msm_camera_sensor_info *data)
{
	uint16_t model_id = 0;
	int rc = 0;

	//CDBG("init entry \n");
	//CDBG_ZS("mt9d112_sensor_init_probe \n");

	rc = mt9d112_reset(data);
	if (rc < 0)
		goto init_probe_fail;


	/* Read the Model ID of the sensor */
	rc = mt9d112_i2c_read(mt9d112_client->addr,
		REG_MT9D112_MODEL_ID, &model_id, WORD_LEN);
	if (rc < 0)
		goto init_probe_fail;

	CDBG_ZS("mt9d112_sensor_init_probe:mt9d112 model_id = 0x%x\n", model_id);

	/* Check if it matches it with the value in Datasheet */
	if (model_id != MT9D112_MODEL_ID) {
		rc = -EINVAL;
		goto init_probe_fail;
	}

	rc = mt9d112_reg_init();

	if (rc < 0)
		goto init_probe_fail;

	return rc;

init_probe_fail:

	CDBG_ZS("mt9d112_sensor_init_probe:failed \n");
	return rc;
}

int mt9d112_sensor_init(const struct msm_camera_sensor_info *data)
{
	int rc = 0;

	CDBG_ZS("mt9d112_sensor_init\n");

	mt9d112_ctrl = kzalloc(sizeof(struct mt9d112_ctrl), GFP_KERNEL);
	if (!mt9d112_ctrl) {
		rc = -ENOMEM;
		goto init_done;
	}

	if (data)
		mt9d112_ctrl->sensordata = data;

	/* Input MCLK = 24MHz */
	msm_camio_clk_rate_set(24000000);
	mdelay(5);

	msm_camio_camif_pad_reg_reset();

	CDBG_ZS("mt9d112_sensor_init **************************\n");

	rc = mt9d112_sensor_init_start(data);

	if (rc < 0) {
		goto init_fail;
	}

init_done:
	return rc;

init_fail:
	kfree(mt9d112_ctrl);
	return rc;
}

static int mt9d112_init_client(struct i2c_client *client)
{
	/* Initialize the MSM_CAMI2C Chip */
	init_waitqueue_head(&mt9d112_wait_queue);
	return 0;
}

int mt9d112_sensor_config(void __user *argp)
{
	struct sensor_cfg_data cfg_data;
	long   rc = 0;

	if (copy_from_user(&cfg_data,
			(void *)argp,
			sizeof(struct sensor_cfg_data)))
		return -EFAULT;

	//CDBG_ZS("mt9d112_sensor_config : cfgtype [%d], mode[%d]\n",cfg_data.cfgtype, cfg_data.mode);

		switch (cfg_data.cfgtype) {
		case CFG_SET_MODE:
			rc = mt9d112_set_sensor_mode(cfg_data.mode);
			break;
		case CFG_SET_EFFECT:
			rc = mt9d112_set_effect(cfg_data.mode,cfg_data.cfg.effect);
			break;
		case CFG_SET_WB://add by lijiankun 2010-9-3
			rc = mt9d112_set_wb(cfg_data.mode,cfg_data.cfg.wb);
			break;
		case CFG_SET_DEFAULT_FOCUS:
			rc = mt9d112_set_default_focus(cfg_data.cfg.focus.focusmode,cfg_data.cfg.scene);
			break;
		case CFG_SET_SCENE://add by lijiankun 2010-9-3
			rc = mt9d112_set_scene(cfg_data.cfg.scene);
			break;
		case CFG_SET_BRIGHTNESS://add by lijiankun 2010-9-3
			rc = mt9d112_set_brightness(cfg_data.cfg.brightness);
			break;
	#ifdef FLASH_LED
		case CFG_SET_FLASH://add by lijiankun 2010-9-3
			rc = mt9d112_set_flash(cfg_data.cfg.flash);
			break;
	#endif
		default:
			rc = -EINVAL;
			break;
		}


	return rc;
}

int mt9d112_sensor_release(void)
{
	int rc = 0;

	CDBG_ZS("mt9d112_sensor_release!\n");

  #ifdef FLASH_LED
	//pwm_config(led_dat->pwm, 2000, 2000);
	//pwm_disable(led_dat->pwm);
	#ifdef CONFIG_LEDS_PMIC8058
    	pm8058_set_flash_led_current(PMIC8058_ID_FLASH_LED_1, 0);
	#endif
  #endif

	kfree(mt9d112_ctrl);

	return rc;
}

static int mt9d112_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	int rc = 0;
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		rc = -ENOTSUPP;
		goto probe_failure;
	}

	mt9d112_sensorw =
		kzalloc(sizeof(struct mt9d112_work), GFP_KERNEL);

	if (!mt9d112_sensorw) {
		rc = -ENOMEM;
		goto probe_failure;
	}

	i2c_set_clientdata(client, mt9d112_sensorw);
	mt9d112_init_client(client);
	mt9d112_client = client;

	//CDBG_ZS("mt9d112_i2c_probe succeeded!\n");

	return 0;

probe_failure:
	kfree(mt9d112_sensorw);
	mt9d112_sensorw = NULL;
	CDBG_ZS("mt9d112_i2c_probe failed!\n");
	return rc;
}

static const struct i2c_device_id mt9d112_i2c_id[] = {
	{ "mt9d112", 0},
	{ },
};

static struct i2c_driver mt9d112_i2c_driver = {
	.id_table = mt9d112_i2c_id,
	.probe  = mt9d112_i2c_probe,
	.remove = __exit_p(mt9d112_i2c_remove),
	.driver = {
		.name = "mt9d112",
	},
};

static int mt9d112_sensor_probe(const struct msm_camera_sensor_info *info,
				struct msm_sensor_ctrl *s)
{


	int rc = i2c_add_driver(&mt9d112_i2c_driver);
	if (rc < 0 || mt9d112_client == NULL) {
		rc = -ENOTSUPP;
		goto probe_done;
	}

#if 0
#ifdef FLASH_LED
	led_dat = kzalloc(sizeof(struct led_pwm_data),GFP_KERNEL);
	if (!led_dat)
		return -ENOMEM;
    led_dat->pwm = pwm_request(6,"flash_LED");
    if (IS_ERR(led_dat->pwm)) {
        	CDBG_ZS("unable to request PWM \n");
   	}
#endif
#endif

	/* Input MCLK = 24MHz */
	msm_camio_clk_rate_set(24000000);
	mdelay(5);

	rc = mt9d112_sensor_init_probe(info);

	if (rc < 0){
		i2c_del_driver(&mt9d112_i2c_driver);
		goto probe_done;
	}
	s->s_init = mt9d112_sensor_init;
	s->s_release = mt9d112_sensor_release;
	s->s_config  = mt9d112_sensor_config;
	s->s_camera_type = BACK_CAMERA_2D;
	s->s_mount_angle  = 0;

probe_done:
	//CDBG("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return rc;
}

static int __mt9d112_probe(struct platform_device *pdev)
{
	//CDBG("__mt9d112_probe...\n");

	return msm_camera_drv_start(pdev, mt9d112_sensor_probe);
}

static struct platform_driver msm_camera_driver = {
	.probe = __mt9d112_probe,
	.driver = {
		.name = "msm_camera_mt9d112",
		.owner = THIS_MODULE,
	},
};

static int __init mt9d112_init(void)
{
	return platform_driver_register(&msm_camera_driver);
}

module_init(mt9d112_init);
