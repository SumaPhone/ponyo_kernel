/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
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
#include "m6mo.h"

#define M6MO_FALSE	0
#define M6MO_TRUE	1

#define M6MO_OK		0
#define M6MO_NG		1

#define	M6MO_PARA_MODE_ERR		(0)
#define	M6MO_PARA_MODE_PAR		(0x01)
#define	M6MO_PARA_MODE_MON		(0x02)
#define	M6MO_PARA_MODE_CAP		(0x03)

/** The length of the header of the I2C write memory command (8 bit access) */
#define M6MO_WRITEMEMORY_START		8 

// M6MO Host Interrupt Factor
#define	M6MO_HOSTINT_MON			(0x0001)	/**< MonitorMode change */
#define	M6MO_HOSTINT_AF				(0x0002)	/**< AF operation(After focusing) */
#define	M6MO_HOSTINT_ZOOM			(0x0004)	/**< Zoom operation((when reach edge) */
#define	M6MO_HOSTINT_CAPTURE		(0x0008)	/**< Capture operation */
#define	M6MO_HOSTINT_FRAMESYNC		(0x0010)	/**< Frame sync(frame output in dual capture) */
#define	M6MO_HOSTINT_FD				(0x0020)	/**< Face detection (When detecting faces) */
#define	M6MO_HOSTINT_LENSINIT		(0x0040)	/**< Lens Initialize */
#define	M6MO_HOSTINT_SOUND			(0x0080)	/**< Shutter Sound and AF complete */
#define M6MO_HOSTINT_ALL			(0x00FF)	/**< Superset of all factors */

// interrupt waiting mode
#define M6MO_WAIT_AND				(0x0000)	/**< Wait until all interrupt factors received */
#define M6MO_WAIT_OR				(0x0001)	/**< Wait until any interrupt factor received */
#define M6MO_WAIT_PEEK				(0x0002) 	/**< Check if the specified interrupt are received and return immediately */


#define M6MO_WRITEMEMORY_START		8 

// M6MO Host Interrupt Factor
#define	M6MO_HOSTINT_MON			(0x0001)	/**< MonitorMode change */
#define	M6MO_HOSTINT_AF				(0x0002)	/**< AF operation(After focusing) */
#define	M6MO_HOSTINT_ZOOM			(0x0004)	/**< Zoom operation((when reach edge) */
#define	M6MO_HOSTINT_CAPTURE		(0x0008)	/**< Capture operation */
#define	M6MO_HOSTINT_FRAMESYNC		(0x0010)	/**< Frame sync(frame output in dual capture) */
#define	M6MO_HOSTINT_FD				(0x0020)	/**< Face detection (When detecting faces) */
#define	M6MO_HOSTINT_LENSINIT		(0x0040)	/**< Lens Initialize */
#define	M6MO_HOSTINT_SOUND			(0x0080)	/**< Shutter Sound and AF complete */
#define M6MO_HOSTINT_ALL			(0x00FF)	/**< Superset of all factors */

// interrupt waiting mode
#define M6MO_WAIT_AND				(0x0000)	/**< Wait until all interrupt factors received */
#define M6MO_WAIT_OR				(0x0001)	/**< Wait until any interrupt factor received */
#define M6MO_WAIT_PEEK				(0x0002) 	/**< Check if the specified interrupt are received and return immediately */

#define M6MO_CUSTOM_DEFAULT_POLL_TIME		20 		// 20 ms
#define M6MO_CUSTOM_DEFAULT_POLL_COUNT		300		// 300 times


/**
 * AF mode
 * @note Value of this enum is important
 */
typedef enum {
	E_M6MO_AF_FULL_SCAN = 0x00, 	// Scan all the VCM step
	E_M6MO_AF_DUMMY1_SCAN, 			// Dummy mode
	E_M6MO_AF_DUMMY2_SCAN, 			// Dummy mode
	E_M6MO_AF_FAST_SCAN, 			// Fast scan (default setting)
	E_M6MO_AF_CONT_SCAN, 			// Continuous focus
	E_M6MO_AF_FINE_CONT_SCAN, 		// Special focus for continuous focus
} E_M6MO_AF_SACN_MODE;

typedef enum {
	E_M6MO_AF_WND_CENTER_SMALL = 0x00, 	// Use center small window
	E_M6MO_AF_WND_CENTER_LARGE, 		// Use center large window (default setting)
 	E_M6MO_AF_WND_5_PTS,		 		// Use Five small windows
	E_M6MO_AF_WND_FD, 					// Use face detection window
	E_M6MO_AF_WND_CUSTOM, 				// Use customize window for touch AF
} E_M6MO_AF_WINDOW_MODE;

typedef enum {
	E_M6MO_AF_MODE_RANGE_INFINITY = 0x00, 	// Infinity range
	E_M6MO_AF_MODE_RANGE_NORMAL, 			// Macro range
	E_M6MO_AF_MODE_RANGE_FULL, 				// Normal (Full) range (default setting)
} E_M6MO_AF_RANGE_MODE;

typedef enum {
	E_M6MO_Mode_Parameter,				/**< Parameter mode */
	E_M6MO_Mode_Monitor,				/**< Monitor mode */
	E_M6MO_Mode_Capture,				/**< Capture mode */
} E_M6MO_Mode;

static char ahs = M6MO_FALSE;
static char facefocus = M6MO_FALSE;
static char en_smile = M6MO_FALSE;
static char smiled = M6MO_FALSE;
//=========================================================================

struct m6mo_work {
	struct work_struct work;
};
//static struct delayed_work	*m6mowork;
static struct  m6mo_work *m6mo_sensorw;
static struct  i2c_client *m6mo_client;

struct m6mo_ctrl {
	const struct msm_camera_sensor_info *sensordata;
};


static struct m6mo_ctrl *m6mo_ctrl;

static DECLARE_WAIT_QUEUE_HEAD(m6mo_wait_queue);
DECLARE_MUTEX(m6mo_sem);

static int m6mo_reset(const struct msm_camera_sensor_info *dev)
{
	int rc = 0;

	CDBG_ZS("m6mo_reset \n");

	rc= gpio_request(dev->sensor_reset, "m6mo");

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

static int32_t m6mo_i2c_txdata(unsigned short saddr,unsigned char *txdata, int length)
{
	struct i2c_msg msg[] = {
		{
			.addr = saddr,
			.flags = 0,
			.len = length,
			.buf = txdata,
		},

	};

	if(i2c_transfer(m6mo_client->adapter, msg, 1) < 0) {
		CDBG_ZS("m6mo_i2c_txdata failed\n");
		return -EIO;
	}
	else
	{
		//CDBG_ZS("m6mo_i2c_txdata success!\n");
	}

	return 0;
}

static int32_t m6mo_i2c_write(unsigned short saddr,
	 unsigned char *wdata, int length)
{
	int32_t rc = -EIO;
	unsigned char *buf;
	buf = wdata;
	rc = m6mo_i2c_txdata(saddr, buf, length);
	
/*
	if(rc < 0)
		CDBG_ZS("i2c_write failed, addr = 0x%x, val = 0x%x!\n",waddr, wdata);
*/
	
	return rc;
}


static int m6mo_i2c_rxdata(unsigned short saddr,
	unsigned char *rxdata, int length)
{

	struct i2c_msg msgs2[] = {
	{
	
		.addr   = saddr,
		.flags = I2C_M_RD,
		.len   = length,
		.buf   = rxdata,
	},
	};



	if(i2c_transfer(m6mo_client->adapter, msgs2, 1) < 0)
	{
		CDBG_ZS("m6mo_i2c_rxdata failed 2!\n");
		return -EIO;
	}
	else
	{
		//CDBG_ZS("m6mo_i2c_rxdata sucess!\n");
	}
		


	return 0;
}

static int32_t m6mo_i2c_read(unsigned short   saddr,
	 unsigned char *rdata, int length)
{
	int32_t rc = 0;
	
	//CDBG_ZS("m6mo_i2c_read saddr:%x,raddr:%x,length:%d",saddr,raddr,length);
	if (!rdata)
		return -EIO;

	//CDBG_ZS("m6mo_i2c_read saddr:%x,*buf :%x,length:%d",saddr,*buf ,length);
	rc = m6mo_i2c_rxdata(saddr, rdata, length);
	if (rc < 0)
		return rc;

	
	return rc;
}

//=====================================================================================
//#define PRINT_SEND_LOG
/**
 * Send I2C WRITE command
 * @param[in] send_buf		the address of buffer containing the I2C command header and write data
 * @param[in] send_num		the size of the send_buf
 * @return 0:OK
 * @return 1:NG
 * @attention it is assumed that this function could send 64KB + 8 bytes. If it cannot,
 * some functions in M6MO_FwWriter.c has to be modified.
 */
int M6MO_Custom_SendWriteCommand(unsigned char * send_buf, int send_num)
{

	int ret;
	ret = m6mo_i2c_write(m6mo_client->addr,send_buf,send_num);
	//ret = SendCommand_NoVerify(send_buf, send_num, I2C_NORMAL_ACCESS);
	return ret;

}

/**
 * Send I2C READ command
 * @param[in] send_buf		the address of buffer containing the I2C command header
 * @param[in] send_num		the size of the send_buf
 * @param[in] rcv_buf		the address of buffer for containing reply data from M6MO
 * @param[in] rcv_num		the size of the rcv_buf
 * @return 0:OK
 * @return 1:NG
 * @attention it is assumed that this function could send 64KB + 8 bytes. If it cannot,
 * some functions in M6MO_FwWriter.c has to be modified.
 */

int M6MO_Custom_SendReadCommand(unsigned char * send_buf, int send_num, unsigned char * rcv_buf,
		int rcv_num)
{

	int ret;
	ret = m6mo_i2c_write(m6mo_client->addr,send_buf,send_num);
	if(ret < 0)
	{
		return ret;
	}
	else
	{
		msleep(2);
		//CDBG_ZS("dw M6MO_Custom_SendReadCommand success 1 !\n");
		ret = m6mo_i2c_read(m6mo_client->addr,rcv_buf,rcv_num);
	}
	//ret = SendCommand(send_buf, send_num, rcv_buf, rcv_num, I2C_NORMAL_ACCESS);
	return ret;


}



//=====================================================================================

/**
 *	@brief		Write one byte to specified category and byte
 *  @param[in]	cat			Indicate which category to be written
 *  @param[in]	byte		Indicate which byte of the category to be written
 *  @param[in]	data		value to be written
 *	@return		0:OK, 1:NG
 *	@note		None
 *	@attention
*/
int M6MO_WriteOneByteCRAM(unsigned char cat,unsigned char byte,unsigned char data)
{
	int ret;
	unsigned char cmd_senddata[8];
	cmd_senddata[0] = 5;
	cmd_senddata[1] = 2;
	cmd_senddata[2] = cat;
	cmd_senddata[3] = byte;
	cmd_senddata[4] = data;
	ret = M6MO_Custom_SendWriteCommand( cmd_senddata, 5);
	return ret;
}

/**
 *	@brief		Read one byte from specified category and byte
 *  @param[in]	cat			Indicate which category to be read
 *  @param[in]	byte		Indicate which byte of the category to be read
 *	@return		The byte required to be read
 *	@note		None
 *	@attention
*/
unsigned char M6MO_ReadOneByteCRAM(unsigned char cat,unsigned char byte)
{
	unsigned char cmd_senddata[8];
	unsigned char cmd_recdata[8];
	cmd_senddata[0] = 5;
	cmd_senddata[1] = 1;
	cmd_senddata[2] = cat;
	cmd_senddata[3] = byte;
	cmd_senddata[4] = 1;
	//Read 2 byte: 1 byte for length and 1 byte data
	M6MO_Custom_SendReadCommand( cmd_senddata, 5, cmd_recdata, 2); 
	return cmd_recdata[1];
}

/**
 *	@brief		Write two byte to specified category and byte
 *  @param[in]	cat			Indicate which category to be written
 *  @param[in]	byte		Indicate the starting byte of the category to be written
 *  @param[in]	data		value to be written
 *	@return		0:OK, 1:NG
 *	@note		None
 *	@attention
*/
int M6MO_WriteOneHalfwordCRAM( unsigned char cat, unsigned char byte, unsigned short data )
{
	int ret;
	unsigned char cmd_senddata[8];
	cmd_senddata[0] = 6;
	cmd_senddata[1] = 2;
	cmd_senddata[2] = cat;
	cmd_senddata[3] = byte;
	cmd_senddata[4] = (unsigned char)( data >>8 );
	cmd_senddata[5] = (unsigned char)( data &0x00FF );
	ret = M6MO_Custom_SendWriteCommand( cmd_senddata, 6);
	return ret;
}

/**
 *	@brief		Read two byte from specified category and byte
 *  @param[in]	cat			Indicate which category to be read
 *  @param[in]	byte		Indicate which byte of the category to be read
 *	@return		The byte required to be read
 *	@note		None
 *	@attention
*/
unsigned short M6MO_ReadOneHalfwordCRAM( unsigned char cat, unsigned char byte )
{
	unsigned char	cmd_senddata[8];
	unsigned char	cmd_recdata[8];
	unsigned short	ret;
	cmd_senddata[0] = 5;
	cmd_senddata[1] = 1;
	cmd_senddata[2] = cat;
	cmd_senddata[3] = byte;
	cmd_senddata[4] = 2;
	//Read 3 byte: 1 byte for length and 2 byte data
	M6MO_Custom_SendReadCommand( cmd_senddata, 5, cmd_recdata, 3); 
	ret  = cmd_recdata[1]<<8;
	ret += cmd_recdata[2];
	return	ret;
}

/**
 *	@brief		Write four byte to specified category and byte
 *  @param[in]	cat			Indicate which category to be written
 *  @param[in]	byte		Indicate the starting byte of the category to be written
 *  @param[in]	data		value to be written
 *	@return		0:OK, 1:NG
 *	@note		None
 *	@attention
*/
int M6MO_WriteOneWordCRAM( unsigned char cat, unsigned char byte, unsigned long data )
{
	int ret;
	unsigned char cmd_senddata[8];
	cmd_senddata[0] = 8;
	cmd_senddata[1] = 2;
	cmd_senddata[2] = cat;
	cmd_senddata[3] = byte;
	cmd_senddata[4] = (unsigned char)(( data &0xFF000000 )>>24 );
	cmd_senddata[5] = (unsigned char)(( data &0x00FF0000 )>>16 );
	cmd_senddata[6] = (unsigned char)(( data &0x0000FF00 )>>8 );
	cmd_senddata[7] = (unsigned char)(  data &0x000000FF );
	ret = M6MO_Custom_SendWriteCommand( cmd_senddata, 8);
	return ret;
}

/**
 *	@brief		Read four byte from specified category and byte
 *  @param[in]	cat			Indicate which category to be read
 *  @param[in]	byte		Indicate which byte of the category to be read
 *	@return		The byte required to be read
 *	@note		None
 *	@attention
*/
unsigned long M6MO_ReadOneWordCRAM( unsigned char cat, unsigned char byte )
{
	unsigned char	cmd_senddata[8];
	unsigned char	cmd_recdata[8];
	unsigned long	ret;
	cmd_senddata[0] = 5;
	cmd_senddata[1] = 1;
	cmd_senddata[2] = cat;
	cmd_senddata[3] = byte;
	cmd_senddata[4] = 4;
	//Read 5 byte: 1 byte for length and 4 byte data
	M6MO_Custom_SendReadCommand( cmd_senddata, 5, cmd_recdata, 5); 
	ret  = cmd_recdata[1]<<24;
	ret += cmd_recdata[2]<<16;
	ret += cmd_recdata[3]<<8;
	ret += cmd_recdata[4];
	return	ret;
}

/**
 *	@brief		Read bytes from specified category and byte
 *  @param[in]	cat			indicate which category to be read
 *  @param[in]	byte		indicate which byte of the category to be read
 *  @param[in] 	byteCnt		the number of byte to read (max: 32 bytes)
 *  @param[in]	cmd_recdata	the buffer for containing the received data
 *	@return		0:OK, 1:NG
 *	@note		None
 *	@attention
*/
int M6MO_ReadBytesCRAM( unsigned char cat, unsigned char byte , unsigned char byteCnt, unsigned char* cmd_recdata)
{
	int ret;
	unsigned char	cmd_senddata[8];
	cmd_senddata[0] = 5;
	cmd_senddata[1] = 1;
	cmd_senddata[2] = cat;
	cmd_senddata[3] = byte;
	cmd_senddata[4] = byteCnt;
	//Read byteCnt+1 byte: 1 byte for length and byteCnt byte data
	ret = M6MO_Custom_SendReadCommand( cmd_senddata, 5, cmd_recdata, byteCnt+1); 
	return ret;
}

/**
 *	@brief		M6MO memory write (8 bit access)
 *  @param[in]	send_buf		address of send buffer, the data to be written at the specified 
 *  							address should be stored at the (send_buf + M6MO_WRITEMEMORY_START) 
 *  							position. It is because the first 8 bytes are to be reserved for 
 *  							the I2C command header.
 *  @param[in]	buf_size		the size of the send_buf
 *  @param[in]	addr			the destination address to be written in M6MO
 *  @param[in]	write_size		size of data to be written
 *	@return		0: OK
 *	@return		1: NG
 *	@note		None
 *	@par Example use:
 *	@code
 *	unsigned char send_buf[16];
 *	int value = 1;
 *	memcpy(send_buf + M6MO_WRITEMEMORY_START, &value, sizeof(int)); // the first 8 bytes should be reserved!
 *	M6MO_WriteMemory(send_buf, sizeof(send_buf), 0x80000000, sizeof(int));
 *	@endcode
 */
int M6MO_WriteMemory(unsigned char * send_buf, unsigned long buf_size, unsigned long addr, unsigned long write_size)
{
	if(buf_size >= 8 + write_size)
	{
		int ret;
		send_buf[0] = 0x0;
		send_buf[1] = 4;	//CMD_EX_WRITE
		send_buf[2] = (unsigned char)((addr & 0xFF000000) >> 24);
		send_buf[3] = (unsigned char)((addr & 0x00FF0000) >> 16);
		send_buf[4] = (unsigned char)((addr & 0x0000FF00) >>  8);
		send_buf[5] = (unsigned char)( addr & 0x000000FF);
		send_buf[6] = (unsigned char)((write_size & 0xFF00) >> 8);
		send_buf[7] = (unsigned char)(write_size & 0x00FF);		
		ret = M6MO_Custom_SendWriteCommand( send_buf, 8 + write_size);
		return ret;
	}else{
		// send_buf's size not large enough
		// note that send_buf's first 8 bytes should be reserved for I2C command header
		return 1;
	}
}


/**
 *	@brief		M6MO memory read (8 bit access)
 *  @param[in]	rcv_buf			the address for saving recevied data
 *  @param[in]	addr			the address to be read in M6MO
 *  @param[in]	rcv_num			size of data to be received
 *	@return		0: OK
 *	@return		1: NG
 *	@note		None
 */
int M6MO_ReadMemory(unsigned char * rcv_buf, unsigned long addr, unsigned long rcv_num)
{
	unsigned char send_buf[16];
	int ret;

	send_buf[0] = 0x00;
	send_buf[1] = 0x03;	//CMD_EX_READ
	send_buf[2] = (unsigned char)((addr & 0xFF000000) >> 24);
	send_buf[3] = (unsigned char)((addr & 0x00FF0000) >> 16);
	send_buf[4] = (unsigned char)((addr & 0x0000FF00) >>  8);
	send_buf[5] = (unsigned char)( addr & 0x000000FF);
	send_buf[6] = (unsigned char)((rcv_num & 0xFF00) >> 8);
	send_buf[7] = (unsigned char)(rcv_num & 0x00FF);
	ret = M6MO_Custom_SendReadCommand(send_buf, 8, rcv_buf, rcv_num+3); //rcv_num+3 since there are 3 bytes for I2C information (0x00, number of byte to read (bit15-08), number of byte to read (bit07-00))
	return ret;

}
//=====================================================================================


//=====================================================================================





/**
 * @brief		M6MO system initialization
 * @param[in]	None
 * @return		0:OK, 1:NG
 * @note 		Availability:
 * Flash Writer [OK] Parameter [--] Monitor [--] Capture Preview [--]
 * @attention	Need to wait about 100ms after CAM_START command
 * 				Do not issue I2C command during system initialization
 * @see 		M6MO_SetMCLK, M6MO_SetYUVClock
 */
int M6MO_System_Init(void)
{
	int i, ret;
	//Write: CAM_START = '0x01' (i.e. write category:0x0F, byte: 0x12 to 0x01)
	M6MO_WriteOneByteCRAM(0x0F, 0x12, 0x01); 
	msleep(50);
	//Read system mode until a normal value instead of 0xFF is returned 
	for( i=0 ; i<2500; i++ ){
		msleep(10);
		ret = M6MO_ReadOneByteCRAM(0x00, 0x0B);
		if(( ret & 0xF0 )!= 0xF0 ){
			return M6MO_OK;
		}
	}
	return M6MO_NG;
}

/**
 * Set continuous preview, thumbnail, main image output.
 * @param[in] enable 	0: disable, 1:enable
 * @note Availability:
 * Flash Writer [--] Parameter [OK] Monitor [--] Capture Preview [--]
 */
void M6MO_System_SetContYUVOut(int enable)
{
	M6MO_WriteOneByteCRAM(0x01, 0x31, enable?1:0);
}

/**
 * Return if continuous output is activated.
 * @return if 1 if continous output is ON and 0 if OFF.
 */
int M6MO_System_GetContYUVOut(void)
{
	return (int)M6MO_ReadOneByteCRAM(0x01, 0x31);
}



//=====================================================================================

/**
 * Wait until specified interrupt occured by polling with timeout.
 * @param[in] factor	a bitwise OR of the interrupt factors to wait
 * @param[in] mode 		the waiting mode 
 * 						M6MO_WAIT_AND: wait until all factors are received
 * 						M6MO_WAIT_OR: wait until any of the factors are received
 * 						M6MO_WAIT_PEEK: Check if the specified interrupt are received and return immediately 
 * @return 	0 when no factor occured before timeout; otherwise, it returns
 * 			the bitwise OR of the interrupt factors received
 */
int M6MO_Custom_PollInterrupt(int factor, int mode)
{
	int loop;
	int curFactor = 0;
	int gotFactor = 0;
	unsigned int iteration;
	
	if(mode == M6MO_WAIT_PEEK)
		iteration = 1;
	else
		iteration = M6MO_CUSTOM_DEFAULT_POLL_COUNT;
	
	for( loop = 0; factor && loop < iteration; loop++ )
	{
		// delay 
		msleep(M6MO_CUSTOM_DEFAULT_POLL_TIME);

		// Read interrupt factor
		curFactor = (int)M6MO_ReadOneByteCRAM( 0x00, 0x1C );	

		if(curFactor & factor){
			gotFactor |= (curFactor & factor);
			// clear obtained factor(s)
			factor &= ~curFactor;
			if(mode == M6MO_WAIT_OR){
				break;
			}
		}
	}

	return gotFactor;
}
int M6MO_WaitInterrupt(int factor, int mode)
{
	int ret;
	//Funciton to be implemented by host to wait M6MO interrupt
	ret = M6MO_Custom_PollInterrupt(factor, mode);	
	return ret;
}
//=====================================================================================


int M6MO_ChangeMode(E_M6MO_Mode mode)
{
	int parameter;
	//unsigned char ret;
	switch(mode){
	case E_M6MO_Mode_Parameter:
		parameter = M6MO_PARA_MODE_PAR;
		break;
	case E_M6MO_Mode_Monitor:
		parameter = M6MO_PARA_MODE_MON;
		break;		
	case E_M6MO_Mode_Capture:
		parameter = M6MO_PARA_MODE_CAP;
		break;		
	default:
		parameter = M6MO_PARA_MODE_ERR;	
		break;
	}

	if(parameter != M6MO_PARA_MODE_ERR)
	{

		int intFactor;
		//Enable all type of interrupt factor
		//Write: INT_EN_MODE = '0xFF' (i.e. write category:0x00, byte: 0x10 to 0xFF)
		M6MO_WriteOneByteCRAM(0x00, 0x10, 0xff);

		//Enable interrupt signal
		//Write: INT_ROOT_ENABLE = '0x01' (i.e. write category:0x00, byte: 0x12 to 0x01)
		M6MO_WriteOneByteCRAM(0x00, 0x12, 0x01);		

		//Go to the specified mode
		//Write: SYS_MODE = parameter (i.e. write category:0x00, byte: 0x0B to parameter)
		M6MO_WriteOneByteCRAM(0x00, 0x0B, parameter);		

		//Wait interrupt 
		if(mode == E_M6MO_Mode_Monitor)
		{
			intFactor = M6MO_WaitInterrupt(M6MO_HOSTINT_MON, M6MO_WAIT_AND);
			
			if(intFactor)
			{
				CDBG_ZS("dw M6MO_ChangeMode M6MO_WaitInterrupt ok1,%x",intFactor);
			}

			return (intFactor? M6MO_OK:M6MO_NG);
		}
		else if(mode == E_M6MO_Mode_Capture)
		{

				intFactor = M6MO_WaitInterrupt(M6MO_HOSTINT_CAPTURE, M6MO_WAIT_AND);			
				return (intFactor? M6MO_OK:M6MO_NG);
		}
		else
		{
			// E_M6MO_Mode_Parameter
			// TODO Confirm 
			return 0;
		}
	}
	return -1;
}



//=====================================================================================




/**
 *	@brief		Request M6MO to capture one image
 *  @param[in]	En_AHS, 0:Disable 1:Enable
 *	@return		0:OK, 1:NG
 *	@note		None
 *	@attention	None
 */
int M6MO_SingleCap_Start(unsigned char En_AHS)
{
	int intFactor;

	//Set Capture-mode
	if(En_AHS){
		M6MO_WriteOneByteCRAM(0x0C, 0x00, 0x0D);		//Write: CAP_MODE = '0x0D': Anti-handshaking
	}
	else{
		M6MO_WriteOneByteCRAM(0x0C, 0x00, 0x00);		//Write: CAP_MODE = '0x00': Normal Single Capture
	}

	//Enable all type of interrupt factor
	M6MO_WriteOneByteCRAM(0x00, 0x10, 0xff);		//Write: INT_EN_MODE = '0xFF' (i.e. write category:0x00, byte: 0x10 to 0xFF)

	//Enable interrupt signal
	M6MO_WriteOneByteCRAM(0x00, 0x12, 0x01);		//Write: INT_ROOT_ENABLE = '0x01' (i.e. write category:0x00, byte: 0x12 to 0x01)

	//Go to capture mode
	M6MO_WriteOneByteCRAM(0x00, 0x0B, 0x03);		//Write: SYS_MODE = '0x03' (i.e. write category:0x00, byte: 0x0B to 0x03)

	//Wait shutter sound interrupt
	//Wait interrupt (INT_STATUS_SOUND) from M6MO
	intFactor = M6MO_WaitInterrupt(M6MO_HOSTINT_SOUND, M6MO_WAIT_AND);	

	if(intFactor)
	{
		CDBG_ZS("dw M6MO_SingleCap_Start M6MO_WaitInterrupt ok1,%x",intFactor);
	}
	
	if(!intFactor){
		return M6MO_NG;	
	}
	
	//Enable interrupt signal
	M6MO_WriteOneByteCRAM(0x00, 0x12, 0x01);		//Write: INT_ROOT_ENABLE = '0x01' (i.e. write category:0x00, byte: 0x12 to 0x01)

	//Wait capture interrupt
	//Wait interrupt (INT_STATUS_CAPTURE) from M6MO
	intFactor = M6MO_WaitInterrupt(M6MO_HOSTINT_CAPTURE, M6MO_WAIT_AND);
	if(intFactor)
	{
		CDBG_ZS("dw M6MO_SingleCap_Start M6MO_WaitInterrupt ok2,%x",intFactor);
	}
	if(!intFactor){
		return M6MO_NG;
	}
	
	return M6MO_OK;
}



/**
 *	@brief		Request M6MO to output main 
 */
void M6MO_SingleCap_GetMain(void)
{
	//unsigned char testVal;
	//Select image number of frame for Main image.When Single Capture, Specify "1".
	M6MO_WriteOneByteCRAM(0x0C, 0x06, 0x01);		//Write: CAP_SEL_FRAME_MAIN = '0x01' (i.e. write category:0x0C, byte: 0x06 to 0x01)

	//Enable interrupt signal
	M6MO_WriteOneByteCRAM(0x00, 0x12, 0x01);		//Write: INT_ROOT_ENABLE = '0x01' (i.e. write category:0x00, byte: 0x12 to 0x01)

	//Call M6MO to send main image
	M6MO_WriteOneByteCRAM(0x0C, 0x09, 0x01);		//Write: CAP_TRANSFER_START = '0x01' (i.e. write category:0x0C, byte: 0x09 to 0x03)



}


void M6MO_SingleCap_GetPreview(void)
{
	//Select image number of frame for Preview image. When Single Capture, Specify "1".
	//Write: CAP_SEL_FRAME_PRV = '0x01' (i.e. write category:0x0C, byte: 0x07 to 0x01)
	M6MO_WriteOneByteCRAM(0x0C, 0x07, 0x01);		

	//Enable interrupt signal
	//Write: INT_ROOT_ENABLE = '0x01' (i.e. write category:0x00, byte: 0x12 to 0x01)
	M6MO_WriteOneByteCRAM(0x00, 0x12, 0x01);
	
	//Get Preview data
	//Write: CAP_TRANSFER_START = '0x02' (i.e. write category:0x0C, byte: 0x09 to 0x02)
	M6MO_WriteOneByteCRAM(0x0C, 0x09, 0x02);		
}

//int M6MO_AF_Start(E_M6MO_AF_WINDOW_MODE wnd_mode, E_M6MO_AF_SACN_MODE scan_mode, E_M6MO_AF_RANGE_MODE range_mode, unsigned short touchWndW, unsigned short touchWndH, unsigned short touchWndX, unsigned short touchWndY)
int M6MO_AF_Start(void)
{
	unsigned char intFactor;
	unsigned char afResult;

	//Enable all type of interrupt factor
	M6MO_WriteOneByteCRAM(0x00, 0x10, 0xff);		//Write: INT_EN_MODE = '0xFF' (i.e. write category:0x00, byte: 0x10 to 0xFF)

	//Enable interrupt signal
	M6MO_WriteOneByteCRAM(0x00, 0x12, 0x01);		//Write: INT_ROOT_ENABLE = '0x01' (i.e. write category:0x00, byte: 0x12 to 0x01)

	//Release auto focus
	M6MO_WriteOneByteCRAM(0x0A, 0x02, 0x00);		//Write: AF_START = '0x00' (i.e. write category:0x0A, byte: 0x02 to 0x00)

	//Wait interrupt
	intFactor = M6MO_WaitInterrupt(M6MO_HOSTINT_AF, M6MO_WAIT_AND);

	if(!intFactor){
		// AF Release failed
		return 1;
	}

	//Set AF mode parameter (Optional)
	//M6MO_WriteOneByteCRAM(0x0A, 0x40, wnd_mode);	//Set for AF window
	//M6MO_WriteOneByteCRAM(0x0A, 0x41, scan_mode);	//Set for AF scan method
	//M6MO_WriteOneByteCRAM(0x0A, 0x42, range_mode);	//Set for AF scan range
	//M6MO_WriteOneHalfwordCRAM(0x0A, 0x22, touchWndW);		//Set for touch AF window width
	//M6MO_WriteOneHalfwordCRAM(0x0A, 0x24, touchWndH);		//Set for touch AF window height
	//M6MO_WriteOneHalfwordCRAM(0x0A, 0x2A, touchWndX);		//Set for touch AF window x position
	//M6MO_WriteOneHalfwordCRAM(0x0A, 0x2C, touchWndY);		//Set for touch AF window y position

	//Enable interrupt signal
	M6MO_WriteOneByteCRAM(0x00, 0x12, 0x01);		//Write: INT_ROOT_ENABLE = '0x01' (i.e. write category:0x00, byte: 0x12 to 0x01)

	//Start auto focus
	M6MO_WriteOneByteCRAM(0x0A, 0x02, 0x01);		//Write: AF_START = '0x01' (i.e. write category:0x0A, byte: 0x02 to 0x01)

	//Wait interrupt
	intFactor = M6MO_WaitInterrupt(M6MO_HOSTINT_AF, M6MO_WAIT_AND);

	//Read AF result
	//READ: AF_RESULT (i.e. read category: 0x0A, byte: 0x03)
	afResult = M6MO_ReadOneByteCRAM(0x0A, 0x03);

	if(afResult == 1)
		return 0;
	else
		return 2;
}


static long m6mo_set_effect(int mode, int effect)
{
	long rc = 0;
	//unsigned char cmdbuf[5];
	CDBG_ZS("m6mo_set_effect:mode[%d],effect[%d]\n", mode,effect);
	switch (effect) {		
	case CAMERA_EFFECT_OFF:
		M6MO_WriteOneByteCRAM(0x02, 0x0B, 0x00);	//Write: COLOR_EFFECT = 0x00 (i.e. write category:0x02, byte: 0x0B to 0x00)
		break;
		
	
	case CAMERA_EFFECT_MONO:
		M6MO_WriteOneByteCRAM(0x02, 0x0B, 0x01);	//Write: COLOR_EFFECT = 0x01 (i.e. write category:0x02, byte: 0x0B to 0x01)
		//Black and white (Cb=0x00, Cr=0x00)
		M6MO_WriteOneByteCRAM(0x02, 0x09, 0x00);
		M6MO_WriteOneByteCRAM(0x02, 0x0A, 0x00);
		break;
			
		
	case CAMERA_EFFECT_SEPIA:
		M6MO_WriteOneByteCRAM(0x02, 0x0B, 0x01);	//Write: COLOR_EFFECT = 0x01 (i.e. write category:0x02, byte: 0x0B to 0x01)
		//Sepia (Cb=0xD8, Cr=0x18)
		M6MO_WriteOneByteCRAM(0x02, 0x09, 0xD8);
		M6MO_WriteOneByteCRAM(0x02, 0x0A, 0x18);		
		break;

	case CAMERA_EFFECT_NEGATIVE:
		M6MO_WriteOneByteCRAM(0x02, 0x0B, 0x02);	//Write: COLOR_EFFECT = 0x01 (i.e. write category:0x02, byte: 0x0B to 0x01)
		break;



	default: 
		return -EINVAL;
	}
	
	return rc;
}

static long m6mo_get_smiled(void)
{
	long rc = 0;
	int intFactor;
	//CDBG_ZS("dw enter m6mo_get_smiled!!");
	if(en_smile)
	{
		intFactor = M6MO_WaitInterrupt(M6MO_HOSTINT_FRAMESYNC, M6MO_WAIT_PEEK);
		if(intFactor == M6MO_HOSTINT_FRAMESYNC)
		{
			//CDBG_ZS("dw has detect smile!!");
			smiled = M6MO_TRUE;
			
		}
		else
		{
			smiled = M6MO_FALSE;
		}
	}

	return rc;
}

static long m6mo_set_antishake(int en_antishake)
{
	long rc = 0;
	CDBG_ZS("m6mo_set_antishake:en_antishake[%d]\n", en_antishake);
	if(en_antishake)
	{
		ahs = M6MO_TRUE;
	}
	else
	{
		ahs = M6MO_FALSE;
	}
	return rc;
}

static long m6mo_set_smile(int smile)
{
	long rc = 0;
	CDBG_ZS("m6mo_set_smile:smile[%d]\n", smile);
	if(smile)
	{	
		smiled = 0;
		en_smile = M6MO_TRUE;

		M6MO_WriteOneByteCRAM(0x09, 0x00, 0x51);
		// Set smile level thershold (the set value could be  
		// 0x01 - 0x64 : SMILE Level(1 - 100%))
		// 0x00: SMILE detection (and Interrupt) disable.
		// Write: FD_SMILE_LEVEL_THRS = 0x32 (i.e. write category:0x09, byte: 0x2A to 0x32)
		M6MO_WriteOneByteCRAM(0x09, 0x2A, 0x14);
		//schedule_delayed_work(m6mowork, 5);

	}
	else
	{
		en_smile = M6MO_FALSE;
		if(facefocus == M6MO_TRUE)
		{
			M6MO_WriteOneByteCRAM(0x09, 0x2A, 0x00);            
			M6MO_WriteOneByteCRAM(0x09, 0x00, 0x11);
		}
		else
		{
			M6MO_WriteOneByteCRAM(0x09, 0x2A, 0x00);            
			M6MO_WriteOneByteCRAM(0x09, 0x00, 0x10);
		}
		//cancel_delayed_work(m6mowork);

	}
	return rc;
}



static long m6mo_set_iso(int iso)
{
	long rc = 0;	
	CDBG_ZS("m6mo_set_iso:iso[%d]\n", iso);
	switch (iso)
	{
		case CAMERA_ISO_AUTO:
			M6MO_WriteOneByteCRAM(0x03, 0x05, 0x00);
			break;
		case CAMERA_ISO_100:
			M6MO_WriteOneByteCRAM(0x03, 0x05, 0x02);
			break;
		case CAMERA_ISO_200:
			M6MO_WriteOneByteCRAM(0x03, 0x05, 0x03);
			break;
		case CAMERA_ISO_400:
			M6MO_WriteOneByteCRAM(0x03, 0x05, 0x04);
			break;

	default: 
		return -EINVAL;
	}
	return rc;
}
static long m6mo_set_ev(int ev)
{
	long rc = 0;	
	CDBG_ZS("m6mo_set_ev:ev[%d]\n", ev);
	switch(ev)
	{
		case -2:
			M6MO_WriteOneByteCRAM(0x03, 0x09, 0x0a);
			break;
		case -1:
			M6MO_WriteOneByteCRAM(0x03, 0x09, 0x14);
			break;
		case 0:
			M6MO_WriteOneByteCRAM(0x03, 0x09, 0x1e);
			break;
		case 1:
			M6MO_WriteOneByteCRAM(0x03, 0x09, 0x28);
			break;
		case 2:
			M6MO_WriteOneByteCRAM(0x03, 0x09, 0x32);
			break;
		default: 
		return -EINVAL;
	}
	return rc;
}

static long m6mo_set_metering(int metering)
{
	long rc = 0;	
	CDBG_ZS("m6mo_set_metering:metering[%d]\n", metering);
	switch (metering)
	{
		case CAMERA_AEC_FRAME_AVERAGE:
			M6MO_WriteOneByteCRAM(0x03, 0x05, 0x00);
			break;
		case CAMERA_AEC_CENTER_WEIGHTED:
			M6MO_WriteOneByteCRAM(0x03, 0x05, 0x02);
			break;
		case CAMERA_AEC_SPOT_METERING:
			M6MO_WriteOneByteCRAM(0x03, 0x05, 0x03);
			break;
	default: 
		return -EINVAL;
	}
	return rc;

}


static long m6mo_set_flash(int flash)
{
	long rc = 0;


	CDBG_ZS("m6mo_set_flash:flash=[%d]\n",flash);

	switch (flash) {
	case LED_MODE_OFF:
		//M6MO_WriteOneByteCRAM(0x0b,0x1f,0x00);
		break;
	
	case LED_MODE_ON:
		//M6MO_WriteOneByteCRAM(0x0b,0x1f,0x03);
		break;
		
	case LED_MODE_AUTO:
		//M6MO_WriteOneByteCRAM(0x0b,0x1f,0x02);
		break;

	default:
		//return -EINVAL;
		break;
	}

	return rc;
}


static long m6mo_set_wb(int mode, int wb)
{
	long rc = 0;
	CDBG_ZS("m6mo_set_wb:mode[%d],wb[%d]\n", mode,wb);
	switch(wb){
	case CAMERA_WB_AUTO:
		M6MO_WriteOneByteCRAM(0x06,0x02,0x01);
		break;
	case CAMERA_WB_INCANDESCENT :
		M6MO_WriteOneByteCRAM(0x06,0x02,0x02);
		M6MO_WriteOneByteCRAM(0x06,0x03,0x01);
		break;
	case CAMERA_WB_FLUORESCENT: 
    	M6MO_WriteOneByteCRAM(0x06,0x02,0x02);
		M6MO_WriteOneByteCRAM(0x06,0x03,0x02);	
		break;
	case CAMERA_WB_DAYLIGHT:
        M6MO_WriteOneByteCRAM(0x06,0x02,0x02);
		M6MO_WriteOneByteCRAM(0x06,0x03,0x04);	
		break;
	case CAMERA_WB_CLOUDY_DAYLIGHT:
		M6MO_WriteOneByteCRAM(0x06,0x02,0x02);
		M6MO_WriteOneByteCRAM(0x06,0x03,0x05);
		break;
	default: 
		return -EINVAL;
		}
	msleep(5);
	return rc;
}




//sun set :0x06
static int32_t m6mo_set_scene(int scene)
{
	long 	rc = 0;
//	unsigned char cmdbuf[5];
	CDBG_ZS("m6mo_set_scene:scene=[%d]\n",scene);
	switch (scene) {
	case CAMERA_BESTSHOT_OFF:
		M6MO_WriteOneByteCRAM(0x02,0x37,0x01);
		break;

	case CAMERA_BESTSHOT_PORTRAIT:
		M6MO_WriteOneByteCRAM(0x02,0x37,0x02);
		break;
	case CAMERA_BESTSHOT_SPORTS:
		M6MO_WriteOneByteCRAM(0x02,0x37,0x04);
		break;
	case CAMERA_BESTSHOT_LANDSCAPE:
		M6MO_WriteOneByteCRAM(0x02,0x37,0x03);
		break;
	case CAMERA_BESTSHOT_NIGHT:
		M6MO_WriteOneByteCRAM(0x02,0x37,0x05);
		break;

	case CAMERA_BESTSHOT_SNOW:
		M6MO_WriteOneByteCRAM(0x02,0x37,0x0a);
		break;

	case CAMERA_BESTSHOT_BEACH:
		
		break;


	case CAMERA_BESTSHOT_SUNSET:
		M6MO_WriteOneByteCRAM(0x02,0x37,0x06);
		break;

	case CAMERA_BESTSHOT_TEXT:
		M6MO_WriteOneByteCRAM(0x02,0x37,0x0b);
		break;
	default:
		return -EINVAL;
	}   
   

	return rc;
}



static long m6mo_set_brightness(int brightness)
{
	long rc = 0;
	CDBG_ZS("m6mo_set_brightness:brightness=[%d]\n",brightness);
	switch (brightness) {
	case CAMERA_BRIGHTNESS_L3:		

		break;
	case CAMERA_BRIGHTNESS_L2:

		break;

	case CAMERA_BRIGHTNESS_L1:

		break;

	case CAMERA_BRIGHTNESS_H0:

		break;

	case CAMERA_BRIGHTNESS_H1:

		break;

	case CAMERA_BRIGHTNESS_H2:

		break;

	case CAMERA_BRIGHTNESS_H3:

		break;

	default:
		return -EINVAL;
	}

	return rc;
}


static long m6mo_set_default_focus(int focusmode)
{
	long 	rc = 0;
	int 	ret;

	//unsigned char intFactor;
	//unsigned char afResult;
	
	CDBG_ZS("dw m6mo_set_default_focus,focusmod:%d\n",focusmode);

	

	switch(focusmode)
	{
		case AF_MODE_NORMAL:
		case AF_MODE_AUTO:
			M6MO_WriteOneByteCRAM(0x0a,0x42,0x02);    //af range mode
			M6MO_WriteOneByteCRAM(0x0a,0x40,0x01); 	  //window
			M6MO_WriteOneByteCRAM(0x09,0x00,0x10);
			facefocus = M6MO_FALSE;
			
			break;
		case AF_MODE_MACRO:
			M6MO_WriteOneByteCRAM(0x0a,0x42,0x01); 
			M6MO_WriteOneByteCRAM(0x0a,0x40,0x01); 
			M6MO_WriteOneByteCRAM(0x09,0x00,0x10);
			facefocus = M6MO_FALSE;
			
			break;

		case AF_MODE_FACE:

			
			break;
		default:
			M6MO_WriteOneByteCRAM(0x0a,0x42,0x02); 
			M6MO_WriteOneByteCRAM(0x0a,0x40,0x01); 
			M6MO_WriteOneByteCRAM(0x09,0x00,0x10); 
			facefocus = M6MO_FALSE;
			break;
	}


	ret = M6MO_AF_Start();

	if(ret == 0)
	{
		CDBG_ZS("dw focus ok***************************");
		rc = 0;		
	}
	else
	{
		CDBG_ZS("dw focus fail ========================");
		//rc = -EIO;

	}
	
	return rc;
}

static long m6mo_set_face(int enface)
{
	long 	rc = 0;
	int 	ret;
	CDBG_ZS("m6mo_set_face:enface[%d]\n", enface);
	if(enface)
	{
		M6MO_WriteOneByteCRAM(0x0a,0x42,0x02); 
		M6MO_WriteOneByteCRAM(0x09,0x00,0x11); 
		M6MO_WriteOneByteCRAM(0x0a,0x40,0x03); 
		facefocus = M6MO_TRUE;	
		ret = M6MO_AF_Start();

		if(ret == 0)
		{
			CDBG_ZS("dw focus ok 2***************************");
			msleep(100);
			rc = 0;		
		}
		else
		{
			CDBG_ZS("dw focus fail 2========================");
			//rc = -EIO;

		}

		M6MO_WriteOneByteCRAM(0x0A, 0x02, 0x00);
	}
	else
	{

		facefocus = M6MO_FALSE;
		if(en_smile == M6MO_TRUE)
		{

		}
		else
		{
			M6MO_WriteOneByteCRAM(0x0a,0x42,0x02); 
			M6MO_WriteOneByteCRAM(0x0a,0x40,0x01); 
			M6MO_WriteOneByteCRAM(0x09,0x00,0x10); 
			M6MO_WriteOneByteCRAM(0x09,0x2A,0x00);
		}
	}

	return rc;
}


static long m6mo_set_sensor_mode(int mode)
{
   
//	int 	intFactor;
	long 	rc = 0;
//	int 	i = 0;   

	CDBG_ZS("m6mo_set_sensor_mode:mode[%d]\n", mode);

	switch (mode) {
	case SENSOR_PREVIEW_MODE:		
		CDBG_ZS("dw enter  SENSOR_PREVIEW_MODE %s line:%d", __func__,__LINE__);	
		M6MO_ChangeMode(E_M6MO_Mode_Monitor);
		break;

	case SENSOR_SNAPSHOT_MODE:
		//M6MO_WriteOneByteCRAM(0x0b,0x01,0x20);  //main image size 5m
		M6MO_WriteOneByteCRAM(0x0b,0x01,0x25);  //main image size
		//M6MO_WriteOneByteCRAM(0x0b,0x01,0x24);    //main image size 8m

		M6MO_WriteOneByteCRAM(0x0b,0x00,0x00);	//snapshot output format
		 

		if(!M6MO_SingleCap_Start(ahs))
		{
			CDBG_ZS("dw M6MO_SingleCap_Start ok");
		}


        M6MO_SingleCap_GetMain();

		

		CDBG_ZS("dw exit SENSOR_SNAPSHOT_MODE %s line:%d", __func__,__LINE__);
		break;	
	case SENSOR_RAW_SNAPSHOT_MODE:
		CDBG_ZS("dw exit SENSOR_RAW_SNAPSHOT_MODE %s line:%d", __func__,__LINE__);


		break;

	default:
		return -EINVAL;
	}
 
   

	return rc;
}

static int m6mo_sensor_init_probe(const struct msm_camera_sensor_info *data)
{
	uint8_t model_id = 0;
	uint8_t version_id = 0;
	int rc = 0;
	//char temp;



	//CDBG("init entry \n");
	CDBG_ZS("m6mo_sensor_init_probe \n");

	rc = m6mo_reset(data);
	if (rc < 0) 
		goto init_probe_fail;
	mdelay(1000);
	if(!M6MO_System_Init())
	{
		CDBG_ZS("M6MO_System_Init \n");
	}
	M6MO_System_SetContYUVOut(M6MO_FALSE);
	M6MO_WriteOneByteCRAM(0x01,0x31,0x01); 
	//M6MO_WriteOneByteCRAM(0x01,0x01,0x1a);     //800*480
	//M6MO_WriteOneByteCRAM(0x01,0x01,0x17);   //preview size  640*480
	//M6MO_WriteOneByteCRAM(0x0A, 0x02, 0x00);  //dw
	M6MO_WriteOneByteCRAM(0x01,0x01,0x1f);    //800*600
	//M6MO_WriteOneByteCRAM(0x01,0x01,0x09);
	
	/*Read the Model ID of the sensor */
	model_id = M6MO_ReadOneByteCRAM(0,0x01);

	CDBG_ZS("m6mo_sensor_init_probe:m6moxx model_id = 0x%x \n", model_id);

	
	version_id = M6MO_ReadOneHalfwordCRAM(0,0x15);
	CDBG_ZS("m6mo_sensor_init_probe:m6moxx version_id = 0x%x \n", version_id);

	if (model_id != 0x03)
		{		
		//goto init_probe_fail;
			CDBG_ZS("m6mo_i2c_read id fail!");
			rc = -EINVAL;
			goto init_probe_fail;
		}
		
	else
		{
		
			CDBG_ZS("m6mo_i2c_read id sucess!");
		}
	
	

	CDBG_ZS("m6mo_sensor_init_probe:m6mo model_id = 0x%x\n", model_id);
	/*
	//Check if it matches it with the value in Datasheet 
	if (model_id != SN12M0PZ_MODEL_ID) {
		rc = -EINVAL;
		goto init_probe_fail;
	}

	*/

	return rc;

init_probe_fail:
    

	CDBG_ZS("m6mo_sensor_init_probe:failed \n");
	return rc;
}


int m6mo_sensor_init(const struct msm_camera_sensor_info *data)
{
	int rc = 0;

	CDBG_ZS("dw m6mo_sensor_init\n");

	m6mo_ctrl = kzalloc(sizeof(struct m6mo_ctrl), GFP_KERNEL);
	if (!m6mo_ctrl) {
		rc = -ENOMEM;
		goto init_done;
	}

	if (data)
		m6mo_ctrl->sensordata = data;


	msm_camio_camif_pad_reg_reset();

	rc = m6mo_sensor_init_probe(data);

	if (rc < 0) {
		goto init_fail;
	}

init_done:
	return rc;

init_fail:
	kfree(m6mo_ctrl);
	return rc;
}

static int m6mo_init_client(struct i2c_client *client)
{
	/* Initialize the MSM_CAMI2C Chip */
	CDBG_ZS("m6mo_init_client !\n");
	init_waitqueue_head(&m6mo_wait_queue);
	return 0;
}

int m6mo_sensor_config(void __user *argp)
{
	struct sensor_cfg_data cfg_data;
	long   rc = 0;

	if (copy_from_user(&cfg_data,
			(void *)argp,
			sizeof(struct sensor_cfg_data)))
		return -EFAULT;
	if(cfg_data.cfgtype != CFG_GET_SMILED)
	CDBG_ZS("m6mo_sensor_config : cfgtype [%d], mode[%d]\n",cfg_data.cfgtype, cfg_data.mode);

		switch (cfg_data.cfgtype) {
		case CFG_SET_MODE:
			rc = m6mo_set_sensor_mode(cfg_data.mode);
			break;
		case CFG_SET_EFFECT:
			rc = m6mo_set_effect(cfg_data.mode,cfg_data.cfg.effect);
			break;
		case CFG_SET_WB:
			rc = m6mo_set_wb(cfg_data.mode,cfg_data.cfg.wb);
			break;
		case CFG_SET_DEFAULT_FOCUS:
			rc = m6mo_set_default_focus(cfg_data.cfg.focus.focusmode);
			break;
		case CFG_SET_SCENE:
			rc = m6mo_set_scene(cfg_data.cfg.scene);
			break;
		case CFG_SET_BRIGHTNESS:
			rc = m6mo_set_brightness(cfg_data.cfg.brightness);
			break;
		case CFG_SET_FLASH:
			rc = m6mo_set_flash(cfg_data.cfg.flash);
			break;	
		case CFG_SET_ISO:
			rc = m6mo_set_iso(cfg_data.cfg.iso);			
			break;
		case CFG_SET_METERING:
			rc = m6mo_set_metering(cfg_data.cfg.exposure_mode);			
			break;
		case CFG_SET_ANTISHAKE:
			rc = m6mo_set_antishake(cfg_data.cfg.en_antishake);			
			break;
		case CFG_SET_EV:
			rc = m6mo_set_ev(cfg_data.cfg.ev);			
			break;	
		case CFG_SET_SMILE:
			rc = m6mo_set_smile(cfg_data.cfg.smile);			
			break;
		case CFG_SET_FACE:
			rc = m6mo_set_face(cfg_data.cfg.enface);			
			break;	
		case CFG_GET_SMILED:
			rc = m6mo_get_smiled();
			cfg_data.cfg.smiled = smiled;			
			if(copy_to_user((void *)argp,&cfg_data,sizeof(struct sensor_cfg_data)))
			rc = 0;
			break;
		default:
			rc = -EINVAL;
			break;
		}


	return rc;
}

int m6mo_sensor_release(void)
{
	int rc = 0;

	CDBG_ZS("dw m6mo_sensor_release!\n");
	smiled = M6MO_FALSE;
	//cancel_delayed_work(m6mowork);
	kfree(m6mo_ctrl);

	return rc;
}
/*
static void simm6mo_work(struct work_struct *work)
{
	int intFactor;
	//CDBG_ZS("dw enter simm6mo_work!!");
	if(en_smile)
	{
		intFactor = M6MO_WaitInterrupt(M6MO_HOSTINT_FRAMESYNC, M6MO_WAIT_PEEK);
		if(intFactor == M6MO_HOSTINT_FRAMESYNC)
		{
			//CDBG_ZS("dw has detect smile!!");
			smiled = M6MO_TRUE;
			
		}
		else
		{
			smiled = M6MO_FALSE;
		}
	}
	CDBG_ZS("dw kernel smiled:%x",smiled);
	schedule_delayed_work(m6mowork, 200);

}
*/
static int m6mo_i2c_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	
	int rc = 0;
	CDBG_ZS("dw m6mo_i2c_probe !\n");
	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		rc = -ENOTSUPP;
		goto probe_failure;
	}

	m6mo_sensorw =
		kzalloc(sizeof(struct m6mo_work), GFP_KERNEL);
	//m6mowork = kzalloc(sizeof(struct delayed_work),GFP_KERNEL);
	if (!m6mo_sensorw) {
		rc = -ENOMEM;
		goto probe_failure;
	}

	i2c_set_clientdata(client, m6mo_sensorw);
	m6mo_init_client(client);
	m6mo_client = client;
	//INIT_DELAYED_WORK_DEFERRABLE(m6mowork, simm6mo_work);
	

	CDBG_ZS("dw m6mo_i2c_probe succeeded!\n");

	return 0;

probe_failure:
	kfree(m6mo_sensorw);
	m6mo_sensorw = NULL;
	CDBG_ZS("m6mo_i2c_probe failed!\n");
	return rc;
}

static const struct i2c_device_id m6mo_i2c_id[] = {
	{ "m6mo", 0},
	{ },
};

static struct i2c_driver m6mo_i2c_driver = {
	.id_table = m6mo_i2c_id,
	.probe  = m6mo_i2c_probe,
	.remove = __exit_p(m6mo_i2c_remove),
	.driver = {
		.name = "m6mo",
	},
};

static int m6mo_sensor_probe(const struct msm_camera_sensor_info *info,
				struct msm_sensor_ctrl *s)
{


	int rc = i2c_add_driver(&m6mo_i2c_driver);
	if (rc < 0 || m6mo_client == NULL) {
		rc = -ENOTSUPP;
		goto probe_done;
	}

	rc = m6mo_sensor_init_probe(info);
    
	if (rc < 0){
		i2c_del_driver(&m6mo_i2c_driver);     
		goto probe_done;
	}
	s->s_init = m6mo_sensor_init;
	s->s_release = m6mo_sensor_release;
	s->s_config  = m6mo_sensor_config;
	s->s_camera_type = BACK_CAMERA_2D;
	s->s_mount_angle  = 0;

probe_done:
	//CDBG("%s %s:%d\n", __FILE__, __func__, __LINE__);
	return rc;
}

static int __m6mo_probe(struct platform_device *pdev)
{
	CDBG("dw __m6mo_probe...\n");

	return msm_camera_drv_start(pdev, m6mo_sensor_probe);
}

static struct platform_driver msm_camera_driver = {
	.probe = __m6mo_probe,
	.driver = {
		.name = "msm_camera_m6mo",
		.owner = THIS_MODULE,
	},
};

static int __init m6mo_init(void)
{
	CDBG("dw m6mo_init...\n");
	return platform_driver_register(&msm_camera_driver);
}

module_init(m6mo_init);


