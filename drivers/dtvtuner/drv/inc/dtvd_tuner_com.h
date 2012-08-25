/*
 *    DTV tuner linux driver
 *
 * Usage:    DTV tuner driver by i2c for linux
 *
 *
 * This software program is licensed subject to the GNU General Public License
 * (GPL).Version 2,June 1991, available at http://www.fsf.org/copyleft/gpl.html
 *
 */

#ifndef _DTVD_TUNER_COM_H_
#define _DTVD_TUNER_COM_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/errno.h>
#include <linux/wait.h>          

#include <linux/semaphore.h>

#include <asm/atomic.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>

#include <asm/gpio.h>
//#include <plat/mcbsp.h>
#include <linux/i2c.h>

#include "dtvtuner.h"             
#include "dtvd_tuner_def.h"       
#include "dtvd_tuner_event.h"     
#include "dtvd_tuner_tag.h"       
#include "dtvd_tuner_pro.h"       
#include "dtvd_tuner_hw.h"        

#include "dtvd_tuner_mtx.h"       

#include "dtvd_tuner_tsprcv.h"    

#endif 

