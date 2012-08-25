/* Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.
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

#include <linux/delay.h>
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <media/msm_camera.h>
#include <mach/gpio.h>
#include <mach/camera.h>
#include <linux/slab.h>
#include "sn12m0pz.h"


#define  REG_SN12M0PZ_MODEL_ID 0x0b
#define  SN12M0PZ_MODEL_ID     0x2013
#define  SN12M0PZ_AF_STATUS    0xB000
/*  SOC Registers Page 1  */
#define  REG_SN12M0PZ_SENSOR_RESET     0x301A
#define  REG_SN12M0PZ_STANDBY_CONTROL  0x3202
#define  REG_SN12M0PZ_MCU_BOOT         0x3386

struct sn12m0pz_work {
	struct work_struct work;
};

static struct  sn12m0pz_work *sn12m0pz_sensorw;
static struct  i2c_client *sn12m0pz_client;

struct sn12m0pz_ctrl {
	const struct msm_camera_sensor_info *sensordata;
};


static struct sn12m0pz_ctrl *sn12m0pz_ctrl;

static DECLARE_WAIT_QUEUE_HEAD(sn12m0pz_wait_queue);
DECLARE_MUTEX(sn12m0pz_sem);
//static int16_t sn12m0pz_effect = CAMERA_EFFECT_OFF;
//static int16_t sn12m0pz_scene = CAMERA_BESTSHOT_OFF;

/*=============================================================
	EXTERNAL DECLARATIONS
==============================================================*/
//extern struct sn12m0pz_reg sn12m0pz_regs;


/*=============================================================*/
/*
static int littleendian2bigendian(unsigned short * dateadd ,int num)
{
	int i;
	unsigned short tempdata;
	unsigned short tempshadow;	
	for(i = 0;i < num; i++)
	{
		tempdata= 0;
		tempshadow = 0;
		tempshadow = dateadd[i];
		tempdata = dateadd[i];
		//CDBG_ZS("littleendian2bigendian dateadd = %x",dateadd[i]);
		tempshadow = tempshadow & (0xff00);	
		tempshadow = (tempshadow >> 8);
		//CDBG_ZS("littleendian2bigendian tempshadow = %x",tempshadow);
		tempdata = tempdata & (0x00ff);
		tempdata = (tempdata << 8);
		//CDBG_ZS("littleendian2bigendian tempdata = %x",tempdata);
		tempshadow = tempshadow|tempdata;
		//CDBG_ZS("littleendian2bigendian tempshadow1 = %x",tempdata);
		dateadd[i] = tempshadow;
		
	}
}
*/
static int sn12m0pz_reset(const struct msm_camera_sensor_info *dev)
{
	int rc = 0;

	CDBG_ZS("sn12m0pz_reset \n");

	rc= gpio_request(dev->sensor_reset, "sn12m0pz");

	if(!rc) {
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

static int32_t sn12m0pz_i2c_txdata(unsigned short saddr,unsigned char *txdata, int length)
{
	struct i2c_msg msg[] = {
		{
			.addr = saddr,
			.flags = 0,
			.len = length,
			.buf = txdata,
		},

	};

	if(i2c_transfer(sn12m0pz_client->adapter, msg, 1) < 0) {
		CDBG_ZS("sn12m0pz_i2c_txdata failed\n");
		return -EIO;
	}
	else
	{
		CDBG_ZS("sn12m0pz_i2c_txdata success!\n");
	}

	return 0;
}

static int32_t sn12m0pz_i2c_write(unsigned short saddr,
	 unsigned char *wdata, int length)
{
	int32_t rc = -EIO;
	unsigned char *buf;
	buf = wdata;
	rc = sn12m0pz_i2c_txdata(saddr, buf, length);
	
/*
	if(rc < 0)
		CDBG_ZS("i2c_write failed, addr = 0x%x, val = 0x%x!\n",waddr, wdata);
*/
	
return rc;
}


static int sn12m0pz_i2c_rxdata(unsigned short saddr,
	unsigned char *rxdata, int length)
{
	struct i2c_msg msgs1[] = {
	{
	
		.addr   = saddr,
		.flags = 0,
		.len   = 1,
		.buf   = rxdata,
	},
	};
	struct i2c_msg msgs2[] = {
	{
	
		.addr   = saddr,
		.flags = I2C_M_RD,
		.len   = length,
		.buf   = rxdata,
	},
	};

	if(i2c_transfer(sn12m0pz_client->adapter, msgs1, 1) < 0) {
		CDBG_ZS("sn12m0pz_i2c_rxdata failed 1!\n");
		return -EIO;
	}
	
	else 
	{
		msleep(2);
		if(i2c_transfer(sn12m0pz_client->adapter, msgs2, 1) < 0)
		{
			CDBG_ZS("sn12m0pz_i2c_rxdata failed 2!\n");
			return -EIO;
		}
		else
		{
			//CDBG_ZS("sn12m0pz_i2c_rxdata sucess!\n");
		}
		

	}

	return 0;
}

static int32_t sn12m0pz_i2c_read(unsigned short   saddr,
	unsigned char raddr, unsigned char *rdata, int length)
{
	int32_t rc = 0;
	unsigned char *buf;
	//CDBG_ZS("sn12m0pz_i2c_read saddr:%x,raddr:%x,length:%d",saddr,raddr,length);
	if (!rdata)
		return -EIO;
	buf   = rdata;
	*buf = raddr;
	//CDBG_ZS("sn12m0pz_i2c_read saddr:%x,*buf :%x,length:%d",saddr,*buf ,length);
	rc = sn12m0pz_i2c_rxdata(saddr, buf, length);
	if (rc < 0)
		return rc;

	rdata = buf;
	return rc;
}
/*

static int32_t sn12m0pz_set_lens_roll_off(void)
{
	int32_t rc = 0;
	//rc = sn12m0pz_i2c_write_table(&sn12m0pz_regs.rftbl[0],
	//	
						 sn12m0pz_regs.rftbl_size);
	return rc;
}
*/



static long sn12m0pz_set_effect(int mode, int effect)
{
	long rc = 0;
	unsigned char cmdbuf[5];
	CDBG_ZS("sn12m0pz_set_effect:mode[%d],effect[%d]\n", mode,effect);
	switch (effect) {		
	case CAMERA_EFFECT_OFF:
		cmdbuf[0] = 0x1e;  //color effect
		cmdbuf[1] = 0;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_EFFECT_OFF wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_EFFECT_OFF sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_EFFECT_OFF fail 1");
					}
				}
			}
		}		
		break;
		
	
	case CAMERA_EFFECT_MONO:
		cmdbuf[0] = 0x1e;
		cmdbuf[1] = 1;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_EFFECT_MONO wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_EFFECT_MONO sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_EFFECT_MONO fail 1");
					}
				}
			}
		}
		break;
			
		
	case CAMERA_EFFECT_SEPIA:
		cmdbuf[0] = 0x1e;
		cmdbuf[1] = 7;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_EFFECT_SEPIA wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_EFFECT_SEPIA sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_EFFECT_SEPIA fail 1");
					}
				}
			}
		}
		
		break;

	case CAMERA_EFFECT_NEGATIVE:
		cmdbuf[0] = 0x1e;
		cmdbuf[1] = 5;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_EFFECT_SEPIA wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_EFFECT_SEPIA sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_EFFECT_SEPIA fail 1");
					}
				}
			}
		}		
		break;

	case CAMERA_EFFECT_SOLARIZE:
	
		break;

	default: 
		return -EINVAL;
	}
	
return rc;
}



static long sn12m0pz_set_wb(int mode, int wb)
{
	long rc = 0;
	unsigned char cmdbuf[5];
	CDBG_ZS("sn12m0pz_set_wb:mode[%d],wb[%d]\n", mode,wb);
	switch(wb){
	case CAMERA_WB_AUTO:
		cmdbuf[0] = 0x1c;  //set wb
		cmdbuf[1] = 0;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_WB_AUTO wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_WB_AUTO sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_WB_AUTO fail 1");
					}
				}
			}
		}	
		break;
	case CAMERA_WB_INCANDESCENT :
		cmdbuf[0] = 0x1c;
		cmdbuf[1] = 3;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_WB_INCANDESCENT wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_WB_INCANDESCENT sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_WB_INCANDESCENT fail 1");
					}
				}
			}
		}		
		break;
	case CAMERA_WB_FLUORESCENT: 
    
		cmdbuf[0] = 0x1c;
		cmdbuf[1] = 2;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_WB_FLUORESCENT wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_WB_FLUORESCENT sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_WB_FLUORESCENT fail 1");
					}
				}
			}
		}	
		break;
	case CAMERA_WB_DAYLIGHT:
    
		cmdbuf[0] = 0x1c;
		cmdbuf[1] = 1;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_WB_DAYLIGHT wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_WB_DAYLIGHT sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_WB_DAYLIGHT fail 1");
					}
				}
			}
		}	
		break;
	case CAMERA_WB_CLOUDY_DAYLIGHT:
		cmdbuf[0] = 0x1c;
		cmdbuf[1] = 4;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_WB_CLOUDY_DAYLIGHT wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_WB_CLOUDY_DAYLIGHT sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_WB_CLOUDY_DAYLIGHT fail 1");
					}
				}
			}
		}		
		break;
	default: 
		return -EINVAL;
		}
	return rc;
}






/*
static int32_t sn12m0pz_set_scene(int scene)
{
	long rc = 0;
	unsigned char cmdbuf[5];
	CDBG_ZS("sn12m0pz_set_scene:scene=[%d]\n",scene);
	switch (scene) {		
	case CAMERA_AUTO_MOD:
		cmdbuf[0] = 0x2e;
		cmdbuf[1] = 0;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_AUTO_MOD wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_AUTO_MOD sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_AUTO_MOD fail 1");
					}
				}
			}
		}	
		msleep(200);
		break;
			
	case CAMERA_PORTTRAIT_MOD:
		cmdbuf[0] = 0x2e;
		cmdbuf[1] = 1;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_PORTTRAIT_MOD wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_PORTTRAIT_MOD sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_PORTTRAIT_MOD fail 1");
					}
				}
			}
		}			
		msleep(200);
		
		break;
	case CAMERA_SPORT_MOD:
		cmdbuf[0] = 0x2e;
		cmdbuf[1] = 2;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_SPORT_MOD wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_SPORT_MOD sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_SPORT_MOD fail 1");
					}
				}
			}
		}			
		msleep(200);
		
		break;
	case CAMERA_LANDSCAPE_MOD:
		cmdbuf[0] = 0x2e;
		cmdbuf[1] = 3;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_LANDSCAPE_MOD wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_LANDSCAPE_MOD sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_LANDSCAPE_MOD fail 1");
					}
				}
			}
		}			
		msleep(200);
		
		break;				
	case CAMERA_NIGHT_MOD:
		cmdbuf[0] = 0x2e;
		cmdbuf[1] = 4;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_NIGHT_MOD wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_NIGHT_MOD sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_NIGHT_MOD fail 1");
					}
				}
			}
		}			
		msleep(200);
		
		break;	
	case CAMERA_ANTISHAKE_MOD:
		cmdbuf[0] = 0x2e;
		cmdbuf[1] = 5;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_ANTISHAKE_MOD wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_ANTISHAKE_MOD sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_ANTISHAKE_MOD fail 1");
					}
				}
			}
		}			
		msleep(200);
		
		break;	
	case CAMERA_SNOW_MOD:
		cmdbuf[0] = 0x2e;
		cmdbuf[1] = 6;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_SNOW_MOD wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_SNOW_MOD sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_SNOW_MOD fail 1");
					}
				}
			}
		}			
		msleep(200);
		
		break;	
	case CAMERA_BEACH_MOD:
		cmdbuf[0] = 0x2e;
		cmdbuf[1] = 7;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_AUTOMOD_OFF wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_AUTOMOD_OFF sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_AUTOMOD_OFF fail 1");
					}
				}
			}
		}			
		msleep(200);
		
		break;
	case CAMERA_CHILDPAT_MOD:
		cmdbuf[0] = 0x2e;
		cmdbuf[1] = 9;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_CHILDPAT_MOD wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_CHILDPAT_MOD sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_CHILDPAT_MOD fail 1");
					}
				}
			}
		}			
		msleep(200);
		
		break;

	case CAMERA_MUSEUM_MOD:
		cmdbuf[0] = 0x2e;
		cmdbuf[1] = 10;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw CAMERA_MUSEUM_MOD wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if((cmdbuf[0] == 0x1e)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw CAMERA_MUSEUM_MOD sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw CAMERA_MUSEUM_MOD fail 1");
					}
				}
			}
		}			
		msleep(200);
		
		break;	
	default: 
		return -EINVAL;
	}   
   

	return rc;
}
*/

/*
	4:QVGA(320X240)
	7:VGA(640X480)
	10:QVGA(400X240)

*/

int Domingo_SetPreviewSize(unsigned char a_Size)
{
	unsigned char cmdbuf[5];
	int ret;

	CDBG_ZS("Domingo_SetPreviewSize: %d", a_Size);

	cmdbuf[0] = 0x35;
	
	sn12m0pz_i2c_read(sn12m0pz_client->addr,
		cmdbuf[0], cmdbuf+1, 1);
	if (cmdbuf[1]!=a_Size)
	{
	    cmdbuf[0] = 0x34; // OP-Code
	    cmdbuf[1] = a_Size;
	        
	    sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);

	    msleep(15);
	    return ret;
	}
	else
	{
	    CDBG_ZS("Domingo_SetPreviewSize: No change");
	    return 0;
	}
}


int DomingoIOSetGet(unsigned char *a_inbuf, int a_inlen, int a_CmdDelayTime)
{
	int ret = 0;	
	unsigned char cmdbuf[5];
	unsigned char cmd;
	
	cmd = a_inbuf[0];
	ret = sn12m0pz_i2c_write(sn12m0pz_client->addr,a_inbuf,a_inlen);

	if(ret < 0)
	{
		CDBG_ZS("dw %s failed,line:%d", __func__,__LINE__);
		return -1;	
	}
	else
	{
		msleep(a_CmdDelayTime);
		
		ret = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
		if(ret <0)
		{
			while(ret < 0)
			{
				ret = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
			}
		}
		else
		{
			if((cmdbuf[0] == cmd) && (cmdbuf[1] == 0))
			{
				CDBG_ZS("dw %s success,line:%d", __func__,__LINE__);
			}
			else
			{
				CDBG_ZS("dw %s failed,line:%d,cmdbuf[0]:%x,cmdbuf[1]:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
				CDBG_ZS("dw %s failed,line:%d", __func__,__LINE__);
				return -1;
			}
		}

	}
	

	return ret;
}
/*
//========================
int DomingoSetAndWait(unsigned char *cmdbuf, int length, int cmd_delay_ms)
{
	unsigned char status[4];
	int ret, retry;
	  
	CDBG_ZS("DomingoSetAndWait[%x]: %x %x", cmdbuf[0], cmdbuf[1], cmdbuf[2]);


	ret = 0;
	retry = 10;  //
	while (retry--)
	{
		ret = DomingoIOSetGet(cmdbuf, length,  100);
		if (ret==0)
		{
			break;
		}
		else
		{		
			msleep(100);
		}

	}
	
	return ret;
}

int Domingo_SetPreview(void)
{
	long rc = 0;
	unsigned char cmdbuf[5];
	
	Domingo_SetPreviewSize(7);

	cmdbuf[0] = 0x0c;
 	cmdbuf[1] = 1;
	rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
	if(rc<0)
	{
		CDBG_ZS("dw %s failed,line:%d", __func__,__LINE__);
		return -1;		
	}
	
	msleep(10);
		
	rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x55,cmdbuf,1);
	if(rc<0)
	{

		CDBG_ZS("dw %s failed,line:%d", __func__,__LINE__);
		return -1;		
	}

	if(cmdbuf[0] != 1)
	{
		CDBG_ZS("dw %s failed,line:%d", __func__,__LINE__);
		return -1;	
	}
	
	return rc;
}
*/
static long sn12m0pz_set_sensor_mode(int mode)
{
	//unsigned short reg = 0;    
//	int i;
	long rc = 0;
	unsigned char cmdbuf[5];
    

	CDBG_ZS("sn12m0pz_set_sensor_mode:mode[%d]\n", mode);

	switch (mode) {
	case SENSOR_PREVIEW_MODE:	
		CDBG_ZS("dw enter  SENSOR_PREVIEW_MODE %s line:%d", __func__,__LINE__);		

		cmdbuf[0] = 0x0c;
 		cmdbuf[1] = 0x01;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x55,cmdbuf,1);
					CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
					if(cmdbuf[0] == 1)
					{
						CDBG_ZS("dw SENSOR_PREVIEW_MODE sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw SENSOR_PREVIEW_MODE fail 1");
					}
				}
			}
		}

		Domingo_SetPreviewSize(7);
		CDBG_ZS("dw exit SENSOR_PREVIEW_MODE %s line:%d", __func__,__LINE__);
		//Domingo_SetPreview();
		/*
		rc = DomingoSetAndWait(cmdbuf,2,100);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		*/
		break;

	case SENSOR_SNAPSHOT_MODE:
		CDBG_ZS("dw exit SENSOR_SNAPSHOT_MODE %s line:%d", __func__,__LINE__);
		cmdbuf[0] = 0x32;  //set capture size
		cmdbuf[1] = 13;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,cmd:%x,data1:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
					if((cmdbuf[0] == 0x32)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw set capture size sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw set capture size fail 1");
					}
				}
			}
			
		}
		/*
		msleep(200);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
		}
*/
		cmdbuf[0] = 0x42;		//set output format
		cmdbuf[1] = 0;
		cmdbuf[2] = 0;//5;//0;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,3);
		msleep(1);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);

		}
		CDBG_ZS("dw exit SENSOR_SNAPSHOT_MODE %s line:%d", __func__,__LINE__);
		
		msleep(5);
		cmdbuf[0] = 0x68;  //set af led
		cmdbuf[1] = 1;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,cmd:%x,data1:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
					if((cmdbuf[0] == 0x68)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw set capture size sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw set capture size fail 1");
					}
				}
			}
			
		}
		msleep(5);
		cmdbuf[0] = 0x38;  //set flash mod
		cmdbuf[1] = 1;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,cmd:%x,data1:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
					if((cmdbuf[0] == 0x38)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw set capture size sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw set capture size fail 1");
					}
				}
			}
			
		}
		msleep(5);
		cmdbuf[0] = 0x3a;		//set led
		cmdbuf[1] = 0x0b;
		cmdbuf[2] = 1;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,3);
		msleep(1);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);

		}
		CDBG_ZS("dw exit SENSOR_SNAPSHOT_MODE %s line:%d", __func__,__LINE__);
		msleep(5);
		cmdbuf[0] = 0x42;		//set output format
		cmdbuf[1] = 0;
		cmdbuf[2] = 0;//5;//0;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,3);
		msleep(1);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);

		}
		CDBG_ZS("dw exit SENSOR_SNAPSHOT_MODE %s line:%d", __func__,__LINE__);	

/*
 		cmdbuf[0] = 0x46;  //set quickview size
 		cmdbuf[1] = 2;
		//rc = DomingoSetAndWait(cmdbuf,2,100);
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x,data1:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
					if((cmdbuf[0] == 0x32) && (cmdbuf[0] == 0))
					{
						CDBG_ZS("dw set quickview size  sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw set quickview size  fail 1");
					}
				}
			}
		}
*/
 		cmdbuf[0] = 0x36;   //capture
 		cmdbuf[1] = 0;
		//rc = DomingoSetAndWait(cmdbuf,2,100);
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x37,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x,data1:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
					if((cmdbuf[0] == 0))
					{
						CDBG_ZS("dw set capture  sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw set capture  fail 1");
					}
				}
			}
		}

		


		break;
	
	if (mode == SENSOR_HFR_120FPS_MODE)

	
	case SENSOR_RAW_SNAPSHOT_MODE:
		CDBG_ZS("dw exit SENSOR_SNAPSHOT_MODE %s line:%d", __func__,__LINE__);
		cmdbuf[0] = 0x32;  //set capture size
		cmdbuf[1] = 13;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,cmd:%x,data1:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
					if((cmdbuf[0] == 0x32)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw set capture size sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw set capture size fail 1");
					}
				}
			}
			
		}
		
		msleep(5);
		cmdbuf[0] = 0x68;  //set af led
		cmdbuf[1] = 1;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,cmd:%x,data1:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
					if((cmdbuf[0] == 0x68)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw set capture size sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw set capture size fail 1");
					}
				}
			}
			
		}
		msleep(5);
		cmdbuf[0] = 0x38;  //set flash mod
		cmdbuf[1] = 1;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,cmd:%x,data1:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
					if((cmdbuf[0] == 0x38)&&(cmdbuf[1] == 0))
					{
						CDBG_ZS("dw set capture size sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw set capture size fail 1");
					}
				}
			}
			
		}
		msleep(5);
		cmdbuf[0] = 0x3a;		//set led
		cmdbuf[1] = 0x0b;
		cmdbuf[2] = 1;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,3);
		msleep(1);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);

		}
		CDBG_ZS("dw exit SENSOR_SNAPSHOT_MODE %s line:%d", __func__,__LINE__);
		msleep(5);
		cmdbuf[0] = 0x42;		//set output format
		cmdbuf[1] = 0;
		cmdbuf[2] = 0;//5;//0;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,3);
		msleep(1);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);

		}
		CDBG_ZS("dw exit SENSOR_SNAPSHOT_MODE %s line:%d", __func__,__LINE__);
	

/*
 		cmdbuf[0] = 0x46;  //set quickview size
 		cmdbuf[1] = 2;
		//rc = DomingoSetAndWait(cmdbuf,2,100);
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x09,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x,data1:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
					if((cmdbuf[0] == 0x32) && (cmdbuf[0] == 0))
					{
						CDBG_ZS("dw set quickview size  sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw set quickview size  fail 1");
					}
				}
			}
		}
*/
/*
		msleep(5);
		cmdbuf[0] = 0x40;		//set AF
		cmdbuf[1] = 0;
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		msleep(1);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);

		}
		CDBG_ZS("dw exit SENSOR_SNAPSHOT_MODE %s line:%d", __func__,__LINE__);
		
		rc = -1;
		if(rc <0)
		{
		while(rc < 0)
		{
			rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x3b,cmdbuf,1);
			CDBG_ZS(" %s,line:%d,data:%x", __func__,__LINE__,cmdbuf[0]);
			if((cmdbuf[0] == 0))
			{
				CDBG_ZS("dw set capture  sucess 1");
			}
			else
			{
				rc = -1;
				CDBG_ZS("dw set capture  fail 1");
			}
		}
		}
		*/
 		cmdbuf[0] = 0x36;   //capture
 		cmdbuf[1] = 0;
		//rc = DomingoSetAndWait(cmdbuf,2,100);
		rc = sn12m0pz_i2c_write(sn12m0pz_client->addr,cmdbuf,2);
		if(rc < 0)
		{
			CDBG_ZS("dw DomingoSetAndWait wrong: %s,line:%d", __func__,__LINE__);
		}
		else
		{
			CDBG_ZS("dw DomingoSetAndWait right: %s,line:%d", __func__,__LINE__);
			rc = -1;
			if(rc <0)
			{
				while(rc < 0)
				{
					rc = sn12m0pz_i2c_read(sn12m0pz_client->addr,0x37,cmdbuf,2);
					CDBG_ZS(" %s,line:%d,data:%x,data1:%x", __func__,__LINE__,cmdbuf[0],cmdbuf[1]);
					if((cmdbuf[0] == 0))
					{
						CDBG_ZS("dw set capture  sucess 1");
					}
					else
					{
						rc = -1;
						CDBG_ZS("dw set capture  fail 1");
					}
				}
			}
		}

		break;

	default:
		return -EINVAL;
	}
 
   

	return rc;
}

static int sn12m0pz_sensor_init_probe(const struct msm_camera_sensor_info *data)
{
	uint16_t model_id = 0;
	int rc = 0;
	int flag = 0;
	int num = 0;

	//CDBG("init entry \n");
	CDBG_ZS("sn12m0pz_sensor_init_probe \n");

	rc = sn12m0pz_reset(data);
	if (rc < 0) 
		goto init_probe_fail;
	mdelay(2000);

	/*Read the Model ID of the sensor */
	flag = sn12m0pz_i2c_read(sn12m0pz_client->addr,
		REG_SN12M0PZ_MODEL_ID, (unsigned char *)(&model_id), 2);
	while(flag < 0)
	{	
		num++;
		flag= sn12m0pz_i2c_read(sn12m0pz_client->addr,
		REG_SN12M0PZ_MODEL_ID, (unsigned char *)(&model_id), 2);
		if(num>50)
		{
			break;
		}
	}
/*
	while(1)
	{	

		flag= sn12m0pz_i2c_read(sn12m0pz_client->addr,
		REG_SN12M0PZ_MODEL_ID, (unsigned char *)(&model_id), 2);
		CDBG_ZS("****************************\n");
		msleep(50);
	}
	*/
	CDBG_ZS("sn12m0pz_sensor_init_probe:sn12m0pzxx model_id = 0x%x \n", model_id);
	//littleendian2bigendian(&model_id,1);
	
	if (model_id != 0x1320)
		{		
		//goto init_probe_fail;
			CDBG_ZS("sn12m0pz_i2c_read id fail!");
			rc = -EINVAL;
			goto init_probe_fail;
		}
		
	else
		{
		
			CDBG_ZS("sn12m0pz_i2c_read id sucess!");
		}
	
	

	CDBG_ZS("sn12m0pz_sensor_init_probe:sn12m0pz model_id = 0x%x\n", model_id);
	/*
	//Check if it matches it with the value in Datasheet 
	if (model_id != SN12M0PZ_MODEL_ID) {
		rc = -EINVAL;
		goto init_probe_fail;
	}

	*/

	return rc;

init_probe_fail:
    

	CDBG_ZS("sn12m0pz_sensor_init_probe:failed \n");
	return rc;
}


int sn12m0pz_sensor_init(const struct msm_camera_sensor_info *data)
{
	int rc = 0;

	CDBG_ZS("dw sn12m0pz_sensor_init\n");

	sn12m0pz_ctrl = kzalloc(sizeof(struct sn12m0pz_ctrl), GFP_KERNEL);
	if (!sn12m0pz_ctrl) {
		rc = -ENOMEM;
		goto init_done;
	}

	if (data)
		sn12m0pz_ctrl->sensordata = data;


	msm_camio_camif_pad_reg_reset();

	rc = sn12m0pz_sensor_init_probe(data);

	if (rc < 0) {
		goto init_fail;
	}

init_done:
	return rc;

init_fail:
	kfree(sn12m0pz_ctrl);
	return rc;
}

static int sn12m0pz_init_client(struct i2c_client *client)
{
	/* Initialize the MSM_CAMI2C Chip */
	CDBG_ZS("sn12m0pz_init_client !\n");
	init_waitqueue_head(&sn12m0pz_wait_queue);
	return 0;
}

int sn12m0pz_sensor_config(void __user *argp)
{
	struct sensor_cfg_data cfg_data;
	long   rc = 0;

	if (copy_from_user(&cfg_data,
			(void *)argp,
			sizeof(struct sensor_cfg_data)))
		return -EFAULT;

	CDBG_ZS("sn12m0pz_sensor_config : cfgtype [%d], mode[%d]\n",cfg_data.cfgtype, cfg_data.mode);

		switch (cfg_data.cfgtype) {
		case CFG_SET_MODE:
			rc = sn12m0pz_set_sensor_mode(cfg_data.mode);
			break;
		case CFG_SET_EFFECT:
			rc = sn12m0pz_set_effect(cfg_data.mode,cfg_data.cfg.effect);
			break;
		case CFG_SET_WB:
			rc = sn12m0pz_set_wb(cfg_data.mode,cfg_data.cfg.wb);
			break;
		case CFG_SET_DEFAULT_FOCUS:
			//rc = sn12m0pz_set_default_focus();
			break;
		case CFG_SET_SCENE:
			//rc = sn12m0pz_set_scene(cfg_data.cfg.scene);
			break;
		default:
			rc = -EINVAL;
			break;
		}


	return rc;
}

int sn12m0pz_sensor_release(void)
{
	int rc = 0;

	CDBG_ZS("dw sn12m0pz_sensor_release!\n");


	kfree(sn12m0pz_ctrl);

	return rc;
}

static int sn12m0pz_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	
	int rc = 0;
	CDBG_ZS("dw sn12m0pz_i2c_probe !\n");
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		rc = -ENOTSUPP;
		goto probe_failure;
	}

	sn12m0pz_sensorw =
		kzalloc(sizeof(struct sn12m0pz_work), GFP_KERNEL);

	if (!sn12m0pz_sensorw) {
		rc = -ENOMEM;
		goto probe_failure;
	}

	i2c_set_clientdata(client, sn12m0pz_sensorw);
	sn12m0pz_init_client(client);
	sn12m0pz_client = client;

	CDBG_ZS("dw sn12m0pz_i2c_probe succeeded!\n");

	return 0;

probe_failure:
	kfree(sn12m0pz_sensorw);
	sn12m0pz_sensorw = NULL;
	CDBG_ZS("sn12m0pz_i2c_probe failed!\n");
	return rc;
}

static const struct i2c_device_id sn12m0pz_i2c_id[] = {
	{ "sn12m0pz", 0},
	{ },
};

static struct i2c_driver sn12m0pz_i2c_driver = {
	.id_table = sn12m0pz_i2c_id,
	.probe  = sn12m0pz_i2c_probe,
	.remove = __exit_p(sn12m0pz_i2c_remove),
	.driver = {
		.name = "sn12m0pz",
	},
};

static int sn12m0pz_sensor_probe(const struct msm_camera_sensor_info *info,
				struct msm_sensor_ctrl *s)
{


	int rc = i2c_add_driver(&sn12m0pz_i2c_driver);
	if (rc < 0 || sn12m0pz_client == NULL) {
		rc = -ENOTSUPP;
		goto probe_done;
	}

	rc = sn12m0pz_sensor_init_probe(info);
    
	if (rc < 0){
		i2c_del_driver(&sn12m0pz_i2c_driver);     
		goto probe_done;
	}
	s->s_init = sn12m0pz_sensor_init;
	s->s_release = sn12m0pz_sensor_release;
	s->s_config  = sn12m0pz_sensor_config;
	s->s_camera_type = BACK_CAMERA_2D;
	s->s_mount_angle  = 0;

probe_done:
	//CDBG("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return rc;
}

static int __sn12m0pz_probe(struct platform_device *pdev)
{
	CDBG("dw __sn12m0pz_probe...\n");

	return msm_camera_drv_start(pdev, sn12m0pz_sensor_probe);
}

static struct platform_driver msm_camera_driver = {
	.probe = __sn12m0pz_probe,
	.driver = {
		.name = "msm_camera_sn12m0pz",
		.owner = THIS_MODULE,
	},
};

static int __init sn12m0pz_init(void)
{
	CDBG("dw sn12m0pz_init...\n");
	return platform_driver_register(&msm_camera_driver);
}

module_init(sn12m0pz_init);

