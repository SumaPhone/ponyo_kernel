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

#include "dtvd_tuner_com.h"
#include "dtvd_tuner_driver.h"
#include "dtvd_tuner_core.h"
#include "dtvd_tuner_tuner.h"
#include "dtvd_tuner_ant.h"
#include <linux/kthread.h>

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/wakelock.h>
#include "dtvd_tuner_tuner.h"
#include "dtvd_tuner_pro.h"
#include "dtvd_tuner_cit.h"


static signed int dtvd_tuner_pwron( void * );
static signed int dtvd_tuner_pwroff( void * );
static signed int dtvd_tuner_init( void * );
static signed int dtvd_tuner_end( void * );
static signed int dtvd_tuner_tune_dtv( void * );
static signed int dtvd_tuner_stop( void * );
static void dtvd_tuner_get_cn( void *);
static void dtvd_tuner_get_info( void *);
static void dtvd_tuner_style( void * );
static signed int dtvd_tuner_wait_event( unsigned long );
static signed int dtvd_tuner_init_omt( void * );
static signed int dtvd_tuner_stop_omt( void * );
static signed int dtvd_tuner_tune_dtv_omt( void * );
static signed int dtvd_tuner_ber_start_omt( void * );
static signed int dtvd_tuner_ber_stop_omt( void * );
static signed int dtvd_tuner_ber_get_omt( void * );
static signed int dtvd_tuner_cn_start_omt( void * );
static signed int dtvd_tuner_cn_stop_omt( void * );
static signed int dtvd_tuner_cn_get_omt( void * );
static signed int dtvd_tuner_agc_start_omt( void * );
static signed int dtvd_tuner_agc_stop_omt( void * );
static signed int dtvd_tuner_agc_get_omt( void * );
static void dtvd_tuner_ant_set( void * );
static void dtvd_tuner_set_eco( void * );

//static DEVICE_ATTR(name, S_IRUGO | S_IWUGO, NULL, NULL);
static DEVICE_ATTR(name, S_IRUGO, NULL, NULL);
static struct class *dtvtuner_dev_class;
static struct device *dtvtuner_dev_device;
struct task_struct *dtvtuner_tuner_thread;

static struct wake_lock wlock;
extern int msm_config_mb86a29(void);
extern void msm_waitmode_mb86a29(void);
extern void msm_wakeup_mb86a29(void);
extern void dtv_get_tsif_device(void);


void open_state_init(void)
{
    unsigned char data = 0;

    set_cit_static_variable(CN_MONITORING_NUM, &data);
    set_cit_static_variable(MER_MONITORING_NUM, &data);
    set_cit_static_variable(MER_LAST_VALUE_NUM, &data);

    dtv_get_tsif_device();
    return;
}

int dtvd_tuner_open
(
    struct inode *inode,    
    struct file  *file      
)
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );
	
    msm_wakeup_mb86a29();
    wake_lock(&wlock);

    open_state_init();

    if( inode == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( file == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( (file->f_flags & O_ACCMODE) != O_RDWR )

    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  file->f_flags, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( tdtvd_tuner_info.count == 0 )
    {

        ret = dtvd_tuner_com_dev_i2c_init();
        if( ret )
        {
            return D_DTVD_TUNER_NG;
        }
    }

    tdtvd_tuner_info.count++;

    DTVD_DEBUG_MSG_EXIT();
//[2011/09/21 simt-leijun]{
#if 0
    ret = dtvd_tuner_com_dev_tsif_start();	
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }
#endif
//[2011/09/21 simt-leijun]}
    return D_DTVD_TUNER_OK;
}

int dtvd_tuner_close
(
    struct inode *inode,    
    struct file  *file      
)
{
    signed int state;

   wake_unlock(&wlock);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( inode == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( file == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    tdtvd_tuner_info.count--;

    if( tdtvd_tuner_info.count == 0 )
    {
        DTVD_DEBUG_MSG_ERR("*tuner closed*\n");

        state = dtvd_tuner_core_get_state();
        if( state != D_DTVD_TUNER_CORE_000 )
        {

            dtvd_tuner_tuner_com_term();

            dtvd_tuner_tuner_com_pwr_off();
        }

        if(tdtvd_tuner_core.pipe_handle)
        {
            filp_close( tdtvd_tuner_core.pipe_handle, 0 );
            tdtvd_tuner_core.pipe_handle = D_DTVD_TUNER_NULL;
        }

        dtvd_tuner_com_dev_i2c_term();

    }

    msm_waitmode_mb86a29();
	
    DTVD_DEBUG_MSG_EXIT();

    return D_DTVD_TUNER_OK;
}

ssize_t dtvd_tuner_read
(
    struct file    *file,           
    char __user    *buffer,         
    size_t         count,           
    loff_t         *f_pos           
)
{
    signed int     state;
    signed int     readlen;
    unsigned long  len;

    DTVD_DEBUG_MSG_ENTER( count, 0, 0 );

    if( file == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( buffer == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( f_pos == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    state = dtvd_tuner_core_get_state();
    if( state < D_DTVD_TUNER_CORE_002 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_STATE,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }

    len = count;
    if( count > D_DTVD_TUNER_TSP_LENGTH )
    {
        len = D_DTVD_TUNER_TSP_LENGTH;
    }

    readlen = dtvd_tuner_tsprcv_read_tsp( buffer, len );
    if( readlen == D_DTVD_TUNER_NG )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();

    return ( ssize_t )readlen;
}

int dtvd_tuner_ioctl
(
    struct inode *inode,    
    struct file *file,      
    unsigned int cmd,       
    unsigned long arg       
)
{
    signed int  result;
    signed int  ret;

    char reglist_test_value;
    char rssi_test_value[4];
    char search_channels[7];
    unsigned long cit_test_value = 0;
    unsigned int mer_test_value = 0;
    long cit_test_wrong_value = -1;	

    DTVD_DEBUG_MSG_ENTER( cmd, 0, 0 );

    if( inode == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( file == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;
 
//    printk("********** ioctl cmd=%d\n", cmd);
    switch( cmd )
    {

        case D_DTVD_TUNER_REQ_PWRON:
            result = dtvd_tuner_pwron( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {
                ret = D_DTVD_TUNER_OK;
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_PWROFF:
            result = dtvd_tuner_pwroff( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {
                ret = D_DTVD_TUNER_OK;
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_INIT:
            result = dtvd_tuner_init( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_END:
            result = dtvd_tuner_end( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {
                ret = D_DTVD_TUNER_OK;
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_TUNE_DTV:
            result = dtvd_tuner_tune_dtv( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_STOP:
            result = dtvd_tuner_stop( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_GET_CN:
            dtvd_tuner_get_cn( (void *)arg );
            ret = D_DTVD_TUNER_OK;
            break;

        case D_DTVD_TUNER_REQ_GET_INFO:
            dtvd_tuner_get_info( (void *)arg );
            break;

        case D_DTVD_TUNER_REQ_SET_STYLE:
            dtvd_tuner_style( (void *)arg );
            ret = D_DTVD_TUNER_OK;
            break;

        case D_DTVD_TUNER_REQ_INIT_OMT:
            result = dtvd_tuner_init_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_STOP_OMT:
            result = dtvd_tuner_stop_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_TUNE_DTV_OMT:
            result = dtvd_tuner_tune_dtv_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_BER_START_OMT:
            result = dtvd_tuner_ber_start_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_BER_STOP_OMT:
            result = dtvd_tuner_ber_stop_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_BER_GET_OMT:
            result = dtvd_tuner_ber_get_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_CNR_START_OMT:
            result = dtvd_tuner_cn_start_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_CNR_STOP_OMT:
            result = dtvd_tuner_cn_stop_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_CNR_GET_OMT:
            result = dtvd_tuner_cn_get_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_AGC_START_OMT:
            result = dtvd_tuner_agc_start_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_AGC_STOP_OMT:
            result = dtvd_tuner_agc_stop_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_AGC_GET_OMT:
            result = dtvd_tuner_agc_get_omt( (void *)arg );
            if( result == D_DTVD_TUNER_OK )
            {

                if( tdtvd_tuner_info.result == D_DTVD_TUNER_OK )
                {
                    ret = D_DTVD_TUNER_OK;
                }
                else
                {
                    ret = -EIO;
                }
            }
            else
            {
                ret = -EINTR;
            }
            break;

        case D_DTVD_TUNER_REQ_ANT_SET_OMT:
            dtvd_tuner_ant_set( (void *)arg );
            ret = D_DTVD_TUNER_OK;
            break;

        case D_DTVD_TUNER_REQ_SET_ECO:
            dtvd_tuner_set_eco( (void *)arg );
            ret = D_DTVD_TUNER_OK;
            break;
        //add for CIT TEST
        case D_DTVD_TUNER_CIT_TEST_SBER:           //SBER
            ret = dtvd_tuner_ber_com_read_CIT_TEST(&cit_test_value);
            if (ret == D_DTVD_TUNER_OK)
            {
                printk("\n**********cit_test_value in ber = %lx\n", cit_test_value);
                ret = copy_to_user((void *)arg, &cit_test_value, sizeof(unsigned long));
            }
            else
            {
                printk("**********cit_test_value in ber fail\n");
                ret = copy_to_user((void *)arg, &cit_test_wrong_value, sizeof(unsigned long));
            }
            break;
			
        case D_DTVD_TUNER_CIT_TEST_CN:       //C/N
            ret = dtvd_tuner_cn_com_read_CIT_TEST(&cit_test_value);
            if (ret == D_DTVD_TUNER_OK)
            {
                printk("\n**********cit_test_value in cn = %lx\n", cit_test_value);
                ret = copy_to_user((void *)arg, &cit_test_value, sizeof(unsigned long));	                 
            }
            else
            {
                printk("**********cit_test_value in cn fail\n");
                ret = copy_to_user((void *)arg, &cit_test_wrong_value, sizeof(unsigned long));
            }				
            break;

        case D_DTVD_TUNER_CIT_TEST_RSSI:     //RSSI
            ret = dtvd_tuner_rssi_com_read_CIT_TEST(rssi_test_value);
            if(ret == D_DTVD_TUNER_OK)
            {
                printk("\n**********rssi_test_value in rssi = %d\n", rssi_test_value[0]);
                ret = copy_to_user((void *)arg, rssi_test_value, sizeof(long));	            
            }
            break;
			
        case D_DTVD_TUNER_CIT_TEST_REGLIST:    //REGLIST
            ret = dtvd_tuner_reglist_com_read_CIT_TEST(&reglist_test_value, 1);
            if (ret == D_DTVD_TUNER_OK)
            {
                printk("\n**********cit_test_value in rssi = 0x%x\n", reglist_test_value);
                ret = copy_to_user((void *)arg, &reglist_test_value, sizeof(char));	            
            }
            break;
			
        case D_DTVD_TUNER_CIT_TEST_CHANNELSCAN:   //CHANNEL SCAN
            ret =  dtvd_tuner_core_search_channel(search_channels);
            if (ret == D_DTVD_TUNER_OK)
            {
                ret = copy_to_user((void *)arg, search_channels, 7*sizeof(char));	                 
            }
            break;

        case D_DTVD_TUNER_CIT_TEST_VBER:   //VBER 
            ret =  dtvd_tuner_vber_com_read_TEST(&cit_test_value);
            if (ret == D_DTVD_TUNER_OK)
            {
                ret = copy_to_user((void *)arg, &cit_test_value, sizeof(unsigned long));	                 
            }
            else
            {
                printk("**********cit_test_value in vber fail\n");
                ret = copy_to_user((void *)arg, &cit_test_wrong_value, sizeof(unsigned long));
            }
            break;

        case D_DTVD_TUNER_CIT_TEST_MER:   //MER
            ret =  dtvd_tuner_mer_com_read(&mer_test_value);
            if (ret == D_DTVD_TUNER_OK)
            {
                ret = copy_to_user((void *)arg, &mer_test_value, sizeof(unsigned long));	                 
            }
            else
            {
                printk("**********cit_test_value in mer fail\n");
                ret = copy_to_user((void *)arg, &cit_test_wrong_value, sizeof(unsigned long));
            }			
            break;			
				
        default:

            DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                      DTVD_TUNER_DRIVER,
                                      0, 0, 0, 0, 0, 0 );
            break;
    }

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_wait_event
(
    unsigned long req   
)
{
    DTVD_TUNER_CONTROL_t    *info = &tdtvd_tuner_info;

    DTVD_DEBUG_MSG_ENTER( (int)req, 0, 0 );

    wait_event_interruptible( info->wait_queue_block,
                              (info->block_flg & req) );

    if(info->block_flg == 0xFFFFFFFF){
        info->block_flg = 0;
        info->result = D_DTVD_TUNER_NG;
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_OK;
    }

    if( (info->block_flg & req) != req )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_INTR;
    }

    info->block_flg &= (~req);

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;

}

static signed int dtvd_tuner_pwron
(
    void* arg
)
{
    signed int              ret;
    unsigned long           result;
    DTVD_TUNER_REQ_PWRON_t  req;
    DTVD_TUNER_MSG_u        body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = D_DTVD_TUNER_OK;

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_PWRON_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }

    if( req.mode == D_DTVD_TUNER_CALLMODE_RETRY )
    {

        ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_PWRON );

        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }
    dtvd_tuner_memset( &body, 0x00,
                       sizeof(DTVD_TUNER_EXEVT_MDL_PWRON_t),
                       sizeof(DTVD_TUNER_MSG_u) );

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                         D_DTVD_TUNER_CMD_MDL_PWRON,
                         (void*)&body,
                         sizeof(DTVD_TUNER_EXEVT_MDL_PWRON_t) );

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_PWRON );
    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_pwroff
(
    void* arg
)
{
    signed int              ret;
    unsigned long           result;
    DTVD_TUNER_REQ_PWROFF_t req;
    DTVD_TUNER_MSG_u        body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = D_DTVD_TUNER_OK;

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_PWROFF_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }

    if( req.mode == D_DTVD_TUNER_CALLMODE_RETRY )
    {

        ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_PWROFF );

        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }
    dtvd_tuner_memset( &body, 0x00,
                       sizeof(DTVD_TUNER_EXEVT_MDL_PWROFF_t),
                       sizeof(DTVD_TUNER_MSG_u) );

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                         D_DTVD_TUNER_CMD_MDL_PWROFF,
                         (void*)&body,
                         sizeof(DTVD_TUNER_EXEVT_MDL_PWROFF_t) );
    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_PWROFF );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_init
(
    void  *arg
)
{
    signed int              ret;
    unsigned long           result;
    DTVD_TUNER_REQ_INIT_t   req;
    DTVD_TUNER_MSG_u        body;
    struct file             *file;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_INIT_t ) );
    if( result != 0 )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }

    if( req.mode == D_DTVD_TUNER_CALLMODE_RETRY )
    {

        ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_INIT );

        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    dtvd_tuner_memset( &body, 0x00,
                       sizeof( DTVD_TUNER_EXEVT_MDL_INIT_t ),
                       sizeof( DTVD_TUNER_MSG_u ) );

    file = filp_open( ( const char * )req.pipename, O_WRONLY | O_NONBLOCK, 0 );
    if( IS_ERR( file ) )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  file, 0, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }
    body.ex_mdl_init.pipe_handle = file;

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                         D_DTVD_TUNER_CMD_MDL_INIT,
                         ( void * )&body,
                         sizeof( DTVD_TUNER_EXEVT_MDL_INIT_t ) );

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_INIT );

    DTVD_DEBUG_MSG_EXIT();

    return ret;
}

static signed int dtvd_tuner_end
(
    void* arg
)
{
    signed int              ret;
    unsigned long           result;
    DTVD_TUNER_REQ_END_t    req;
    DTVD_TUNER_MSG_u        body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = D_DTVD_TUNER_OK;

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_END_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }

    if( req.mode == D_DTVD_TUNER_CALLMODE_RETRY )
    {

        ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_END );

        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    dtvd_tuner_memset( &body, 0x00,
                       sizeof(DTVD_TUNER_EXEVT_MDL_END_t),
                       sizeof(DTVD_TUNER_MSG_u) );

    body.ex_mdl_end.mode = req.kind;

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                         D_DTVD_TUNER_CMD_MDL_END,
                         (void*)&body,
                         sizeof(DTVD_TUNER_EXEVT_MDL_END_t) );

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_END );

    DTVD_DEBUG_MSG_EXIT();

    return ret;
}

static signed int dtvd_tuner_tune_dtv
(
    void* arg
)
{
    signed int              ret;
    unsigned long           result;
    DTVD_TUNER_REQ_TUNE_t   req;
    DTVD_TUNER_MSG_u        body;
    unsigned short          command;
    unsigned int            len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = D_DTVD_TUNER_OK;

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_TUNE_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }

    if( req.mode == D_DTVD_TUNER_CALLMODE_RETRY )
    {
        ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_TUNE );

        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    if( req.kind == D_DTVD_TUNER_TUNE_KIND_SCAN )
    {
        command = D_DTVD_TUNER_CMD_MDL_SCAN_DTV;
        len = sizeof(DTVD_TUNER_EXEVT_MDL_SCAN_DTV_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        body.ex_mdl_scan_dtv.ch = req.ch;
        body.ex_mdl_scan_dtv.seq_id = req.seq_id;
//[2011/09/21 simt-leijun]{
        set_cit_static_variable(SCANNING_CHANNEL_NUM, &req.ch.no);
//[2011/09/21 simt-leijun]}

    }
    else
    {

        command = D_DTVD_TUNER_CMD_MDL_TUNE_DTV;
        len = sizeof(DTVD_TUNER_EXEVT_MDL_TUNE_DTV_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        body.ex_mdl_tune_dtv.ch = req.ch;

        body.ex_mdl_tune_dtv.adj = req.adj;

        body.ex_mdl_tune_dtv.kind = req.kind;

        body.ex_mdl_tune_dtv.seq_id = req.seq_id;

    }

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                         command,
                         (void*)&body,
                         len );
    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_TUNE );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_stop
(
    void* arg
)
{
    signed int              ret;
    unsigned long           result;
    DTVD_TUNER_REQ_STOP_t   req;
    DTVD_TUNER_MSG_u        body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = D_DTVD_TUNER_OK;

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_STOP_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }

    if( req.mode == D_DTVD_TUNER_CALLMODE_RETRY )
    {

        ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_STOP );

        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    dtvd_tuner_memset( &body, 0x00,
                       sizeof(DTVD_TUNER_EXEVT_MDL_STOP_t),
                       sizeof(DTVD_TUNER_MSG_u) );

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                         D_DTVD_TUNER_CMD_MDL_STOP,
                         (void*)&body,
                         sizeof(DTVD_TUNER_EXEVT_MDL_STOP_t) );

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_STOP );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static void dtvd_tuner_get_cn
(
    void* arg
)
{
    unsigned long           result;
    DTVD_TUNER_REQ_GET_CN_t req;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_GET_CN_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return;
    }

    if( tdtvd_tuner_monitor.rx.cna.state == D_DTVD_TUNER_MEASURE_STATE_ON )
    {
        req.result = D_DTVD_TUNER_GET_RESULT_OK;

        req.rx_level = dtvd_tuner_core_get_rx_level();
    }
    else
    {
        req.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;

        req.rx_level = D_DTVD_TUNER_RX_LEVEL_OUT;
    }

    result = copy_to_user( arg, &req, sizeof(DTVD_TUNER_REQ_GET_CN_t) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return;
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_get_info
(
    void* arg
)
{
    unsigned long               result;
    DTVD_TUNER_REQ_GET_INFO_t   *out;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    out = arg;
    result = copy_to_user( &out->monitor, &tdtvd_tuner_monitor,
                           sizeof(DTVD_TUNER_MONITOR_INFO_t) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return;
    }
    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_style
(
    void* arg
)
{
#if 0 
    unsigned long               result;
    DTVD_TUNER_REQ_SET_STYLE_t  req;
    DTVD_TUNER_MSG_u            body;
#endif

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

#if 0 
    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_SET_STYLE_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return;
    }

    dtvd_tuner_memset( &body, 0x00,
                       sizeof(DTVD_TUNER_EXEVT_MDL_STYLE_t),
                       sizeof(DTVD_TUNER_MSG_u) );

    body.ex_mdl_style.style = req.style;

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                         D_DTVD_TUNER_CMD_MDL_STYLE,
                         (void*)&body,
                         sizeof(DTVD_TUNER_EXEVT_MDL_STYLE_t) );
#endif

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static signed int dtvd_tuner_init_omt
(
    void* arg
)
{
    signed int                  ret;
    unsigned long               result;
    DTVD_TUNER_REQ_INIT_OMT_t   req;
    DTVD_TUNER_MSG_u            body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = D_DTVD_TUNER_OK;

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_INIT_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        dtvd_tuner_memset( &body, 0x00,
                           sizeof(DTVD_TUNER_EXEVT_MDL_INIT_OMT_t),
                           sizeof(DTVD_TUNER_MSG_u) );

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_INIT_OMT,
                             (void*)&body,
                             sizeof(DTVD_TUNER_EXEVT_MDL_INIT_OMT_t) );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_INIT_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_stop_omt
(
    void* arg
)
{
    signed int                  ret;
    unsigned long               result;
    DTVD_TUNER_REQ_STOP_OMT_t   req;
    DTVD_TUNER_MSG_u            body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = D_DTVD_TUNER_OK;

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_STOP_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        dtvd_tuner_memset( &body, 0x00,
                           sizeof(DTVD_TUNER_EXEVT_MDL_STOP_OMT_t),
                           sizeof(DTVD_TUNER_MSG_u) );

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_STOP_OMT,
                             (void*)&body,
                             sizeof(DTVD_TUNER_EXEVT_MDL_STOP_OMT_t) );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_tune_dtv_omt
(
    void* arg
)
{
    signed int                      ret;
    unsigned long                   result;
    DTVD_TUNER_REQ_TUNE_DTV_OMT_t   req;
    DTVD_TUNER_MSG_u                body;
    unsigned int                    len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_TUNE_DTV_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        len = sizeof(DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        body.ex_mdl_tune_dtv_omt.ch_no = req.ch_no;

        body.ex_mdl_tune_dtv_omt.tune_sync_kind = req.sync;

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_TUNE_DTV_OMT,
                             (void*)&body,
                             len );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
    if( ret == D_DTVD_TUNER_OK )
    {

        if( tdtvd_tuner_info.omt_tune_result == D_DTVD_TUNER_OK )
        {

            req.result = D_DTVD_TUNER_OK;

            if( tdtvd_tuner_monitor.tune.adj.mode == 0 )
            {
                req.adj.mode = D_DTVD_TUNER_TSMODE_MODE2;
            }
            else
            {
                req.adj.mode = D_DTVD_TUNER_TSMODE_MODE3;
            }
            req.adj.gi = tdtvd_tuner_monitor.tune.adj.gi;
        }
        else
        {

            req.result = D_DTVD_TUNER_NG;
        }

        result = copy_to_user( arg, &req, sizeof(DTVD_TUNER_REQ_TUNE_DTV_OMT_t) );
        if( result != 0 )
        {

            DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                      DTVD_TUNER_DRIVER,
                                      result, 0, 0, 0, 0, 0 );
            return D_DTVD_TUNER_NG;
        }
    }

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_ber_start_omt
(
    void* arg
)
{
    signed int                      ret;
    unsigned long                   result;
    DTVD_TUNER_REQ_BER_START_OMT_t  req;
    DTVD_TUNER_MSG_u                body;
    unsigned int                    len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_BER_START_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        len = sizeof(DTVD_TUNER_EXEVT_MDL_BER_START_OMT_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        body.ex_mdl_ber_start_omt.out = req.out;

        body.ex_mdl_ber_start_omt.period = req.period;

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_BER_START_OMT,
                             (void*)&body,
                             len );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_BER_START_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_ber_stop_omt
(
    void* arg
)
{
    signed int                      ret;
    unsigned long                   result;
    DTVD_TUNER_REQ_BER_STOP_OMT_t   req;
    DTVD_TUNER_MSG_u                body;
    unsigned int                    len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_BER_STOP_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        len = sizeof(DTVD_TUNER_EXEVT_MDL_BER_STOP_OMT_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_BER_STOP_OMT,
                             (void*)&body,
                             len );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_BER_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_ber_get_omt
(
    void* arg
)
{
    signed int                      ret;
    unsigned long                   result;
    DTVD_TUNER_REQ_BER_GET_OMT_t    req;
    DTVD_TUNER_MSG_u                body;
    unsigned int                    len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_BER_GET_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        len = sizeof(DTVD_TUNER_EXEVT_MDL_BER_GET_OMT_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_BER_GET_OMT,
                             (void*)&body,
                             len );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_BER_GET_OMT );
    if( ret == D_DTVD_TUNER_OK )
    {

        req.value = tdtvd_tuner_monitor.rx.ber.value;

        result = copy_to_user( arg, &req, sizeof(DTVD_TUNER_REQ_BER_GET_OMT_t) );
        if( result != 0 )
        {

            DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                      DTVD_TUNER_DRIVER,
                                      result, 0, 0, 0, 0, 0 );
            return D_DTVD_TUNER_NG;
        }
    }

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_cn_start_omt
(
    void* arg
)
{
    signed int                      ret;
    unsigned long                   result;
    DTVD_TUNER_REQ_CNR_START_OMT_t  req;
    DTVD_TUNER_MSG_u                body;
    unsigned int                    len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_CNR_START_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        len = sizeof(DTVD_TUNER_EXEVT_MDL_CNR_START_OMT_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        body.ex_mdl_cnr_start_omt.carrier = req.carrier;

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_CNR_START_OMT,
                             (void*)&body,
                             len );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_CNR_START_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_cn_stop_omt
(
    void* arg
)
{
    signed int                      ret;
    unsigned long                   result;
    DTVD_TUNER_REQ_CNR_STOP_OMT_t   req;
    DTVD_TUNER_MSG_u                body;
    unsigned int                    len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_CNR_STOP_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        len = sizeof(DTVD_TUNER_EXEVT_MDL_CNR_STOP_OMT_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_CNR_STOP_OMT,
                             (void*)&body,
                             len );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_CNR_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_cn_get_omt
(
    void* arg
)
{
    signed int                      ret;
    unsigned long                   result;
    DTVD_TUNER_REQ_CNR_GET_OMT_t    req;
    DTVD_TUNER_MSG_u                body;
    unsigned int                    len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_CNR_GET_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        len = sizeof(DTVD_TUNER_EXEVT_MDL_CNR_GET_OMT_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_CNR_GET_OMT,
                             (void*)&body,
                             len );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_CNR_GET_OMT );
    if( ret == D_DTVD_TUNER_OK )
    {

        req.value_x = tdtvd_tuner_monitor.rx.cna.value_x;
        req.value_y = tdtvd_tuner_monitor.rx.cna.value_y;
        req.value_comp = tdtvd_tuner_monitor.rx.cna.value_comp;

        result = copy_to_user( arg, &req, sizeof(DTVD_TUNER_REQ_CNR_GET_OMT_t) );
        if( result != 0 )
        {

            DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                      DTVD_TUNER_DRIVER,
                                      result, 0, 0, 0, 0, 0 );
            return D_DTVD_TUNER_NG;
        }
    }

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_agc_start_omt
(
    void* arg
)
{
    signed int                      ret;
    unsigned long                   result;
    DTVD_TUNER_REQ_AGC_START_OMT_t  req;
    DTVD_TUNER_MSG_u                body;
    unsigned int                    len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_AGC_START_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        len = sizeof(DTVD_TUNER_EXEVT_MDL_AGC_START_OMT_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_AGC_START_OMT,
                             (void*)&body,
                             len );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_AGC_START_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_agc_stop_omt
(
    void* arg
)
{
    signed int                      ret;
    unsigned long                   result;
    DTVD_TUNER_REQ_AGC_STOP_OMT_t   req;
    DTVD_TUNER_MSG_u                body;
    unsigned int                    len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_AGC_STOP_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        len = sizeof(DTVD_TUNER_EXEVT_MDL_AGC_STOP_OMT_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_AGC_STOP_OMT,
                             (void*)&body,
                             len );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_AGC_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static signed int dtvd_tuner_agc_get_omt
(
    void* arg
)
{
    signed int                      ret;
    unsigned long                   result;
    DTVD_TUNER_REQ_AGC_GET_OMT_t    req;
    DTVD_TUNER_MSG_u                body;
    unsigned int                    len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_AGC_GET_OMT_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = D_DTVD_TUNER_OK;

    if( req.mode == D_DTVD_TUNER_CALLMODE_NORMAL )
    {
        len = sizeof(DTVD_TUNER_EXEVT_MDL_AGC_GET_OMT_t);
        dtvd_tuner_memset( &body, 0x00,
                           len, sizeof(DTVD_TUNER_MSG_u) );

        dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                             D_DTVD_TUNER_CMD_MDL_AGC_GET_OMT,
                             (void*)&body,
                             len );
    }

    ret = dtvd_tuner_wait_event( D_DTVD_TUNER_WAKEUP_AGC_GET_OMT );
    if( ret == D_DTVD_TUNER_OK )
    {

        req.value_x = tdtvd_tuner_monitor.rx.agc.value_x;
        req.value_y = tdtvd_tuner_monitor.rx.agc.value_y;

        result = copy_to_user( arg, &req, sizeof(DTVD_TUNER_REQ_AGC_GET_OMT_t) );
        if( result != 0 )
        {

            DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                      DTVD_TUNER_DRIVER,
                                      result, 0, 0, 0, 0, 0 );
            return D_DTVD_TUNER_NG;
        }
    }

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

static void dtvd_tuner_ant_set
(
    void *arg
)
{

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_set_eco
(
    void* arg
)
{
    unsigned long               result;
    DTVD_TUNER_REQ_SET_ECO_t    req;
    DTVD_TUNER_MSG_u            body;
    unsigned int                len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( arg == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    result = copy_from_user( &req, arg, sizeof( DTVD_TUNER_REQ_SET_ECO_t ) );
    if( result != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_DRIVER,
                                  result, 0, 0, 0, 0, 0 );

        return;
    }

    len = sizeof(DTVD_TUNER_EXEVT_MDL_ECO_SET_t);
    dtvd_tuner_memset( &body, 0x00,
                       len, sizeof(DTVD_TUNER_MSG_u) );

     body.ex_mdl_eco_set.mode = req.eco_mode;

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_MIDDLE,
                         D_DTVD_TUNER_CMD_MDL_ECO_SET,
                         (void*)&body,
                         len );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

int dtvd_tuner_init_module(void)
{
    int         ret = 0;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );
	
    wake_lock_init(&wlock, WAKE_LOCK_SUSPEND, "dtvwake");
    msm_waitmode_mb86a29();

    ret = register_chrdev( D_DTVD_TUNER_CHAR_MAJOR,
                           D_DTVD_TUNER_DRV_NAME,
                           (struct file_operations*)&tdtvd_tuner_fops );

    if (ret < 0)
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return -EIO;
    }

    dtvtuner_dev_class = class_create(THIS_MODULE, "dtvtuner-dev");
    if (IS_ERR(dtvtuner_dev_class))
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return -EIO;
    }

    dtvtuner_dev_device = device_create(dtvtuner_dev_class, NULL, MKDEV(D_DTVD_TUNER_CHAR_MAJOR, 0), NULL, D_DTVD_TUNER_DRV_NAME);
    if (IS_ERR(dtvtuner_dev_device))
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return -EIO;
    }

    ret = device_create_file(dtvtuner_dev_device, &dev_attr_name);
    if(ret) 
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return -EIO;
    }

    dtvtuner_tuner_thread = kthread_create(dtvd_tuner_main, (void *)D_DTVD_TUNER_NULL, "tunercore");
    if(IS_ERR(dtvtuner_tuner_thread)){

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_DRIVER,
                                  0, 0, 0, 0, 0, 0 );
        return -EIO;

    }
    wake_up_process(dtvtuner_tuner_thread);

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_cleanup_module(void)
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    device_remove_file(dtvtuner_dev_device, &dev_attr_name);

    device_destroy(dtvtuner_dev_class, MKDEV(D_DTVD_TUNER_CHAR_MAJOR, 0));

    class_destroy(dtvtuner_dev_class);

    unregister_chrdev( D_DTVD_TUNER_CHAR_MAJOR, D_DTVD_TUNER_DRV_NAME );

    DTVD_DEBUG_MSG_EXIT();
}

module_init( dtvd_tuner_init_module );
module_exit( dtvd_tuner_cleanup_module );
MODULE_DESCRIPTION( "1SEG Tuner Driver" );
MODULE_LICENSE("GPL");

