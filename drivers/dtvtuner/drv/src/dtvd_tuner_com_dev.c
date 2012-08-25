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
#include <linux/tsif_api.h>

//#include <plat/mcbsp.h>
//#include <linux/cfgdrv.h>

//#include <linux/spi/spi-subpmic.h>

static void dtvd_tuner_com_dev_int_tasklet( unsigned long );
static void dtvd_tuner_com_dev_romdata_debugout( void );
static DECLARE_TASKLET( tdtvd_tuner_int_tasklet, dtvd_tuner_com_dev_int_tasklet, 0 );

static void dtvd_tuner_com_dev_trans_msg_syserr( unsigned char );

#if 0
static const struct xxxx_mcbsp_reg_cfg dtvd_tuner_mcbsp_config
      = {	0,						
      		0,					
      		0,						
      		(RFRLEN1(51-1) | RWDLEN1(5)),		
      		0,						
      		0,						
      		(CLKSM | FPER(1632-1)),				
      		(FWID(188-1) | CLKGDV(1)),			
      		0,						
      		0,						
      		(SCLKME),		        		
      		0,						
      		0,						
      		0, 						
      		0,						
      		0,						
      		0,						
      		0,						
      		0,						
      		0,						
      		0,						
      		0,						
      		0,						
      		0,						
      		(RDMAEN | RFULL_CYCLE),				
      		0						
      };

#endif
static  int dtvint_enable;

//[2011/09/22 simt-leijun]{
struct msm_tsif_device *tsif_device_dtv;
extern struct msm_tsif_device *get_tsif_device(void);

void dtv_get_tsif_device(void)
{
    tsif_device_dtv = get_tsif_device();
}
EXPORT_SYMBOL(dtv_get_tsif_device);
//[2011/09/22 simt-leijun]}

signed int dtvd_tuner_com_dev_i2c_read
(
    unsigned long length,                               
    unsigned char *addr,                                
    unsigned char *data                                 
)
{

    int ret;

    struct i2c_msg msgs[2] = { {0, 0, 0, 0},
                               {0, 0, 0, 0}};
    struct i2c_adapter *adap;

    if(( length > D_DTVD_TUNER_DATALEN_MAX ) ||
       ( length == 0 ))
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, length, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( addr == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( data == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    msgs[0].addr = D_DTVD_TUNER_DEV_TUNER;  
    msgs[0].flags = 0;                      
    msgs[0].len  = 1;                       
    msgs[0].buf  = addr;                    
    msgs[1].addr = D_DTVD_TUNER_DEV_TUNER;  
    msgs[1].flags = I2C_M_RD;               
    msgs[1].len  = length;                  
    msgs[1].buf  = data;                    
    adap = tdtvd_tuner_info.adap;           
    ret = i2c_transfer(adap, msgs, 2);      
    if( ret != 2 )
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_I2C_READ,
                                  DTVD_TUNER_COM_DEV,
                                  ret, D_DTVD_TUNER_DEV_TUNER, addr, *addr, 0, 0 );

        return D_DTVD_TUNER_NG;
    }
    DTVD_DEBUG_MSG_PCOM_READ( "I2C", *addr, *data );

    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_com_dev_i2c_write
(
    unsigned long length,                               
    DTVD_TUNER_COM_I2C_DATA_t *data                     
)
{

    struct i2c_msg msg;
    unsigned char  i2cdata[D_DTVD_TUNER_DATALEN_MAX*2];
    unsigned char  *pdata;
    int i;
    int ret;

    if( data == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( length > D_DTVD_TUNER_DATALEN_MAX )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, length, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( length == 0 )
    {
        return D_DTVD_TUNER_OK;
    }

    pdata = &i2cdata[0];
    for( i = 0; i < length; i++ )
    {
        DTVD_DEBUG_MSG_PCOM_WRITE( "I2C", data[i].adr, data[i].data );
        pdata[0] = data[i].adr; 
        pdata[1] = data[i].data; 
        pdata +=2;
    }
    msg.addr = D_DTVD_TUNER_DEV_TUNER;  
    msg.flags = 0;                      
    msg.len  = length*2;                
    msg.buf  = i2cdata;                 
    ret = i2c_transfer(tdtvd_tuner_info.adap, &msg, 1);  
    if( ret != 1 )
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_I2C_WRITE,
                                  DTVD_TUNER_COM_DEV,
                                  ret, D_DTVD_TUNER_DEV_TUNER, i2cdata, i2cdata[0], i2cdata[1], 0 );
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_com_dev_i2c_init(void)
{

    if(tdtvd_tuner_info.adap)
    {
       return D_DTVD_TUNER_OK;
    }

    tdtvd_tuner_info.adap = i2c_get_adapter(D_DTVD_TUNER_DEV_I2C);
    if(tdtvd_tuner_info.adap == NULL) {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_I2C_INIT,
                                  DTVD_TUNER_COM_DEV,
                                  0, D_DTVD_TUNER_DEV_TUNER, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_com_dev_i2c_term(void)
{

    if(tdtvd_tuner_info.adap == NULL)
    {
       return;
    }

    i2c_put_adapter(tdtvd_tuner_info.adap);

    return;
}

signed int dtvd_tuner_com_dev_i2c_rfic_write     
(                                                
    unsigned long length,                        
    DTVD_TUNER_COM_I2C_DATA_t *data              
)                                                
{                                                

    signed   int  ret;
    unsigned long index;
    DTVD_TUNER_COM_I2C_DATA_t out_data[] = 
        {
            { 0x31, 0x00, { 0, 0 } } , 
            { 0x36, 0x10, { 0, 0 } } , 
            { 0x32, 0x00, { 0, 0 } } , 
            { 0x33, 0x00, { 0, 0 } } , 
            { 0x32, 0x10, { 0, 0 } } , 
            { 0x33, 0x00, { 0, 0 } } , 
            { 0x31, 0x80, { 0, 0 } }   
        };

    unsigned long count = sizeof( out_data ) / sizeof( DTVD_TUNER_COM_I2C_DATA_t );

    if( data == NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A(
                 D_DTVD_TUNER_SYSERR_DRV_PARAM,
                 DTVD_TUNER_COM_DEV,
                 0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( length > D_DTVD_TUNER_RF_DATALEN_MAX )
    {

        DTVD_TUNER_SYSERR_RANK_A(
                 D_DTVD_TUNER_SYSERR_DRV_PARAM,
                 DTVD_TUNER_COM_DEV,
                 0, length, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( length == 0 )
    {
        return D_DTVD_TUNER_OK;
    }

    for( index = 0; index < length; index++ )
    {
        out_data[3].data = data[index].adr; 
        out_data[5].data = data[index].data;

        DTVD_DEBUG_MSG_PCOM_WRITE( "3WIRE", out_data[3].adr, out_data[5].data );

        ret = dtvd_tuner_com_dev_i2c_write( count, out_data );

        if( ret != D_DTVD_TUNER_OK )
        {

            DTVD_TUNER_SYSERR_RANK_B(
            D_DTVD_TUNER_SYSERR_DRV_3WIRE_TRANS,
            DTVD_TUNER_COM_DEV, ret, count,
            out_data[0].adr, out_data[0].data,
            out_data[1].adr, out_data[1].data );

            return D_DTVD_TUNER_NG;
        }
    }

    return D_DTVD_TUNER_OK;                      
}                                                

void dtvd_tuner_com_dev_powic_read
(
    unsigned long addr,                            
    unsigned char *data                            
)
{
    int ret;

    if( data == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

#if 0
    ret = subpmic_spi_read_u8((int)addr, data);
#endif
    if ( ret != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_SPI_READ,
                                  DTVD_TUNER_COM_DEV,
                                  ret, addr, data, *data, 0, 0 );
        return ;
    }
    return ;
}

void dtvd_tuner_com_dev_powic_write
(
    unsigned long addr,                            
    unsigned char data                             
)
{

#if 0
    int ret;
	unsigned char readdata;
	unsigned char setdata;
	int mode;

    ret = subpmic_spi_read_u8((int)addr, &readdata);
    if( ret != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_SPI_READ,
                                  DTVD_TUNER_COM_DEV,
                                  ret, addr, data, 0, 0, 0 );
        return ;
    }

	
	setdata = data ^ readdata;

	
	if ((setdata & readdata) != 0)
	{
		
		mode = SUBPMIC_WRITEMODE_AND;
		setdata = ~setdata;
	}
	else
	{
		
		mode = SUBPMIC_WRITEMODE_OR;
	}

    ret = subpmic_spi_readthenwrite_u8((int)addr, setdata, mode);
    if( ret != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_SPI_WRITE,
                                  DTVD_TUNER_COM_DEV,
                                  ret, addr, data, 0, 0, 0 );
        return ;
    }
    return ;
#endif
}

void dtvd_tuner_com_dev_gpio_read
(
    unsigned long addr,                            
    unsigned char *data                            
)
{
    if( data == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    *data = ( unsigned char )gpio_get_value( ( unsigned )addr );

    DTVD_DEBUG_MSG_PCOM_READ( "GPIO", addr, *data );

    return;
}

void dtvd_tuner_com_dev_gpio_write
(
    unsigned long addr,                            
    unsigned char data                             
)
{
    DTVD_DEBUG_MSG_PCOM_WRITE( "GPIO", addr, data );

    gpio_set_value( ( unsigned )addr, ( int )data );

    return;
}

void dtvd_tuner_com_dev_romdata_read( void )
{
//[2011/09/21 simt-leijun]{
    DTVD_TUNER_COM_NONVOLATILE_t  *ptr = &tdtvd_tuner_nonvola;
    ptr->wait_tmcc = 0x0;
    ptr->cn_wait = 0x0;
    ptr->wait_ts = 0x0;
    ptr->cn_ave_count = 0x1;
    ptr->cn_cycle = 0xa;
    ptr->cn_alpha = 0x0;
    ptr->cn_beta = 0x5;
    ptr->cn_gamma = 0x5;
    ptr->agc_wait = 0x0;
    ptr->agc_cycle = 0x5;
    ptr->ber_wait = 0x0;
    ptr->ber_cycle = 0x5;
    ptr->state_monitor_cycle = 0x2;
    ptr->wait_search_int = 0xf;
    ptr->wait_mnt_sync = 0xa;
    ptr->omt_agc_thr = 0x8c;
    ptr->auto_n2e_ifagc_qp12 = 0x20;
    ptr->auto_n2e_ifagc_qp23 = 0x20;
    ptr->auto_n2e_ifagc_qam12 = 0x20;
    ptr->auto_e2n_ifagc_qp12 = 0x48;
    ptr->auto_e2n_ifagc_qp23 = 0x48;
    ptr->auto_e2n_ifagc_qam12 = 0x48;
    ptr->wait_refclk = 0x19;
    ptr->search_thres = 0x28;
    ptr->wait_lock1 = 0x14;
    ptr->wait_lock2 = 0x5;
    ptr->reserved4 =0x0;
    ptr->reserved5 =0x0;
    ptr->cn_qpsk1_2_m = 0x23f76c;
    ptr->cn_qpsk1_2_h = 0x169cb0;
    ptr->cn_qpsk2_3_h = 0xc975f;
    ptr->cn_qpsk2_3_m = 0x1b81aa;
    ptr->cn_16qam1_2_m = 0x3fd088;
    ptr->cn_16qam1_2_h = 0x185400;
    ptr->cn_qpsk1_2_eco_m = 0x23f76c;
    ptr->cn_qpsk1_2_eco_h = 0x1a71be;
    ptr->cn_qpsk2_3_eco_m = 0x1b81aa;
    ptr->cn_qpsk2_3_eco_h = 0xc975f;
    ptr->cn_16qam1_2_eco_m = 0x3e2894;
    ptr->cn_16qam1_2_eco_h = 0x2275b2;
    ptr->mode_on = 0x0;
    ptr->tmcc_on = 0x1;
    ptr->mode_dec_count = 0x21;
    ptr->search_success_count = 0x21;
    ptr->search_success_thr = 0x28;
    ptr->search_time = 0x10;
    ptr->scan_mode_dec_count = 0x21;
    ptr->scan_search_success_count = 0x21;
    ptr->reserved10 = 0x0;
    ptr->reserved11 = 0x0;
    ptr->reserved12 = 0x0;
    ptr->reserved13 = 0x0;
    ptr->reserved14 = 0x0;
    ptr->reserved16 = 0x0;
    ptr->reserved17 = 0x0;
    ptr->auto_n2e_cn_qp12 = 0x0;
    ptr->auto_n2e_cn_qp23 = 0x0;
    ptr->auto_n2e_cn_qam12 = 0x0;
    ptr->auto_e2n_cn_qp12 = 0x0;
    ptr->auto_e2n_cn_qp23 = 0x0;
    ptr->auto_e2n_cn_qam12 = 0x0;

    dtvd_tuner_com_dev_romdata_debugout();

    return;

//[2011/09/21 simt-leijun]}


#if 0
    signed int ret;

    dtvd_tuner_memset( &tdtvd_tuner_nonvola,
                       0x00,
                       sizeof(DTVD_TUNER_COM_NONVOLATILE_t),
                       sizeof(DTVD_TUNER_COM_NONVOLATILE_t) );

    ret = cfgdrv_read(  D_DTVD_TUNER_INFO_ID,
	                    sizeof(DTVD_TUNER_COM_NONVOLATILE_t),
	                    ( unsigned char* )(void*)&tdtvd_tuner_nonvola );

    if( ret != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_NONVOLA_READ,
                                  DTVD_TUNER_COM_DEV,
                                  (unsigned long)ret, 0, 0, 0, 0, 0 );
        return;
    } 
	ret = 0; 

	

    dtvd_tuner_com_dev_romdata_debugout();

    return;
#endif
}

void dtvd_tuner_com_dev_int_regist
(
    unsigned int param      
)
{
    int ret;

    DTVD_DEBUG_MSG_ENTER( param, 0, 0 );

    ret = gpio_request( D_DTVD_TUNER_INT_PORT, "DTVINT" );
    if( ret )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PINT_REG,
                                  DTVD_TUNER_COM_DEV,
                                  ret,
                                  D_DTVD_TUNER_INT_PORT,
                                  param, 0, 0, 0 );
        return;
    }

    ret = gpio_direction_input( D_DTVD_TUNER_INT_PORT );
    if( ret )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PINT_REG,
                                  DTVD_TUNER_COM_DEV,
                                  ret,
                                  D_DTVD_TUNER_INT_PORT,
                                  param, 0, 0, 0 );
        return;
    }

    ret = request_irq( gpio_to_irq( D_DTVD_TUNER_INT_PORT ),
                       dtvd_tuner_com_dev_int_handler,      
                       IRQF_TRIGGER_LOW ,                   
                       D_DTVD_TUNER_DRV_NAME,               
                       &tdtvd_tuner_info );                 
    if( ret )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PINT_REG,
                                  DTVD_TUNER_COM_DEV,
                                  ret,
                                  D_DTVD_TUNER_INT_PORT,
                                  dtvd_tuner_com_dev_int_handler,
                                  param, 0, 0 );
        return;
    }

    dtvint_enable = 1;
    if( param != D_DTVD_TUNER_INT_MASK_CLEAR )
    {
        dtvd_tuner_com_dev_int_disable();
    }

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_com_dev_int_unregist( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_com_dev_int_disable();

    free_irq( gpio_to_irq( D_DTVD_TUNER_INT_PORT ),     
              &tdtvd_tuner_info );                      

    gpio_free( D_DTVD_TUNER_INT_PORT );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_com_dev_int_enable( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if(!dtvint_enable) 
    {
        dtvint_enable =1;
        enable_irq( gpio_to_irq( D_DTVD_TUNER_INT_PORT ) );
    }
    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_com_dev_int_disable( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if(dtvint_enable)
    {
        dtvint_enable = 0;
        disable_irq( gpio_to_irq( D_DTVD_TUNER_INT_PORT ) );
    }
    DTVD_DEBUG_MSG_EXIT();

    return;
}


irqreturn_t dtvd_tuner_com_dev_int_handler( int irq, void *dev_id )
{

    dtvint_enable = 0;
    disable_irq_nosync( gpio_to_irq( D_DTVD_TUNER_INT_PORT ) );

    tasklet_schedule( &tdtvd_tuner_int_tasklet );

    return IRQ_HANDLED;
}


static void dtvd_tuner_com_dev_int_tasklet
(
    unsigned long data
)
{
    DTVD_TUNER_MSG_u    body;
    unsigned int        len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof(DTVD_TUNER_EXEVT_TUNER_INT_t);
    dtvd_tuner_memset( &body, 0x00,
                       len,
                       sizeof(DTVD_TUNER_MSG_u) );

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_TUNER,
                         D_DTVD_TUNER_CMD_TUNER_INT,
                         &body, len );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_com_dev_ddsync_write
(
    struct file *file,      
    void *      data,       
    size_t      data_size   
 )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0,  0, data_size );

    if( file == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    if( data == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    if( file->f_op->write)
    {
        ret = file->f_op->write(file, data, data_size, &(file->f_pos));
    }
    else
    {
        ret = do_sync_write(file, data, data_size, &(file->f_pos));
    }
    if( ret != (signed int)data_size )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_DDSYNC_WRITE,
                                  DTVD_TUNER_COM_DEV,
                                  (unsigned long)ret, (unsigned long)file,
                                  file->f_op->write, (unsigned long)data, (unsigned long)data_size, 0 );
        return;
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

signed int dtvd_tuner_com_dev_tsif_start(void)
{
    int ret;

    tsif_set_time_limit(tsif_device_dtv, 5000);
    tsif_set_mode(tsif_device_dtv, 2);
    tsif_set_buf_config(tsif_device_dtv, 20, 24);
    ret = tsif_start(tsif_device_dtv);
    if (ret != D_DTVD_TUNER_OK)
        return D_DTVD_TUNER_NG;

    DTVD_DEBUG_MSG_EXIT();

    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_com_dev_tsif_stop(void)
{
    tsif_stop (tsif_device_dtv);
    DTVD_DEBUG_MSG_EXIT();
}



signed int dtvd_tuner_com_dev_tsif_read_start
(
    void       *buffer[2],    
    unsigned int    length[2]      
)
{

	
    int ret;

    if( (buffer == D_DTVD_TUNER_NULL) || (length == D_DTVD_TUNER_NULL) )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, length[0], length[1], 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( (length[0] < 2) || (length[1] < 2) )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, length[0], length[1], 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = tsif_read (tsif_device_dtv, buffer[0], length[0]);
    ret = tsif_read (tsif_device_dtv, buffer[1], length[1]);

    if(ret != 0)
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_MCBSP_READ,
                                  DTVD_TUNER_COM_DEV,
                                  ret, D_DTVD_TUNER_DEV_TUNER, length, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }
    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_com_dev_tsif_read
(
    void *buffer,
    unsigned int    length      
)
{
    int ret;

    if( length < 2 )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_DEV,
                                  0, length, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

	ret = tsif_read (tsif_device_dtv, buffer, length);
    if(ret != 0)
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_MCBSP_READ,
                                  DTVD_TUNER_COM_DEV,
                                  ret, D_DTVD_TUNER_DEV_TUNER, length, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }
    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_com_dev_tsif_read_stop(void)
{
    int ret;
#if 0
    ret = xxxx_mcbsp_recv_buffer_chain_stop(D_DTVD_TUNER_MCBSP);
#endif
    if(ret != 0)
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_MCBSP_READ,
                                  DTVD_TUNER_COM_DEV,
                                  ret, 0, 0, 0, 0, 0 );
        return;
    }

#if 0
    ret =xxxx_mcbsp_recv_buffer_chain_free(D_DTVD_TUNER_MCBSP);
#endif
    if(ret != 0)
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_MCBSP_READ,
                                  DTVD_TUNER_COM_DEV,
                                  ret, 0, 0, 0, 0, 0 );
        return;
    }
    return;
}

void dtvd_tuner_com_dev_tsif_read_cancel(void)
{
    int ret;
#if 0
    ret = xxxx_mcbsp_recv_buffer_chain_cancel(D_DTVD_TUNER_MCBSP);
#endif
    if(ret != 0)
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_MCBSP_READ,
                                  DTVD_TUNER_COM_DEV,
                                  ret, 0, 0, 0, 0, 0 );
        return;
    }
    return;
}

void dtvd_tuner_com_dev_log
(
    unsigned char   rank,       
    unsigned long   kind,       
    unsigned char   file_no,    
    unsigned long   line,       
    unsigned long   data1,      
    unsigned long   data2,      
    unsigned long   data3,      
    unsigned long   data4,      
    unsigned long   data5,      
    unsigned long   data6       
)
{
    unsigned long func_no;
    DTVD_TUNER_COM_SYSERR_t err_log;

    dtvd_tuner_memset( &err_log, 0x00,
                       sizeof(DTVD_TUNER_COM_SYSERR_t),
                       sizeof(DTVD_TUNER_COM_SYSERR_t) );

    func_no = ( D_DTVD_TUNER_SYSERR_BLKID | kind );

    err_log.file_no = file_no;
    err_log.line = line;
    err_log.log_data[0] = data1;
    err_log.log_data[1] = data2;
    err_log.log_data[2] = data3;
    err_log.log_data[3] = data4;
    err_log.log_data[4] = data5;
    err_log.log_data[5] = data6;

    dtvd_tuner_com_dev_trans_msg_syserr(rank);

    return;
}
#if 1
static void dtvd_tuner_com_dev_romdata_debugout( void )
{

    DTVD_DEBUG_MSG_INFO( "wait_tmcc             =%x\n"  ,   tdtvd_tuner_nonvola.wait_tmcc );
    DTVD_DEBUG_MSG_INFO( "cn_wait               =%x\n"  ,   tdtvd_tuner_nonvola.cn_wait );
    DTVD_DEBUG_MSG_INFO( "wait_ts               =%x\n"  ,   tdtvd_tuner_nonvola.wait_ts );
    DTVD_DEBUG_MSG_INFO( "cn_ave_count          =%x\n"  ,   tdtvd_tuner_nonvola.cn_ave_count );
    DTVD_DEBUG_MSG_INFO( "cn_cycle              =%x\n"  ,   tdtvd_tuner_nonvola.cn_cycle );
    DTVD_DEBUG_MSG_INFO( "cn_alpha              =%x\n"  ,   tdtvd_tuner_nonvola.cn_alpha );
    DTVD_DEBUG_MSG_INFO( "cn_beta               =%x\n"  ,   tdtvd_tuner_nonvola.cn_beta );
    DTVD_DEBUG_MSG_INFO( "cn_gamma              =%x\n"  ,   tdtvd_tuner_nonvola.cn_gamma );
    DTVD_DEBUG_MSG_INFO( "agc_wait              =%x\n"  ,   tdtvd_tuner_nonvola.agc_wait );
    DTVD_DEBUG_MSG_INFO( "agc_cycle             =%x\n"  ,   tdtvd_tuner_nonvola.agc_cycle );
    DTVD_DEBUG_MSG_INFO( "ber_wait              =%x\n"  ,   tdtvd_tuner_nonvola.ber_wait );
    DTVD_DEBUG_MSG_INFO( "ber_cycle             =%x\n"  ,   tdtvd_tuner_nonvola.ber_cycle );
    DTVD_DEBUG_MSG_INFO( "state_monitor_cycle   =%x\n"  ,   tdtvd_tuner_nonvola.state_monitor_cycle );
    DTVD_DEBUG_MSG_INFO( "wait_search_int       =%x\n"  ,   tdtvd_tuner_nonvola.wait_search_int );
    DTVD_DEBUG_MSG_INFO( "wait_mnt_sync         =%x\n"  ,   tdtvd_tuner_nonvola.wait_mnt_sync );
    DTVD_DEBUG_MSG_INFO( "omt_agc_thr           =%x\n"  ,   tdtvd_tuner_nonvola.omt_agc_thr );
    DTVD_DEBUG_MSG_INFO( "auto_n2e_ifagc_qp12   =%x\n"  ,   tdtvd_tuner_nonvola.auto_n2e_ifagc_qp12 );
    DTVD_DEBUG_MSG_INFO( "auto_n2e_ifagc_qp23   =%x\n"  ,   tdtvd_tuner_nonvola.auto_n2e_ifagc_qp23 );
    DTVD_DEBUG_MSG_INFO( "auto_n2e_ifagc_qam12  =%x\n"  ,   tdtvd_tuner_nonvola.auto_n2e_ifagc_qam12 );
    DTVD_DEBUG_MSG_INFO( "auto_e2n_ifagc_qp12   =%x\n"  ,   tdtvd_tuner_nonvola.auto_e2n_ifagc_qp12 );
    DTVD_DEBUG_MSG_INFO( "auto_e2n_ifagc_qp23   =%x\n"  ,   tdtvd_tuner_nonvola.auto_e2n_ifagc_qp23 );
    DTVD_DEBUG_MSG_INFO( "auto_e2n_ifagc_qam12  =%x\n"  ,   tdtvd_tuner_nonvola.auto_e2n_ifagc_qam12 );

    DTVD_DEBUG_MSG_INFO( "wait_refclk            =%x\n"  ,   tdtvd_tuner_nonvola.wait_refclk );

    DTVD_DEBUG_MSG_INFO( "search_thres           =%x\n"  ,   tdtvd_tuner_nonvola.search_thres );

    DTVD_DEBUG_MSG_INFO( "wait_lock1            =%x\n"  ,   tdtvd_tuner_nonvola.wait_lock1  );
    DTVD_DEBUG_MSG_INFO( "wait_lock2            =%x\n"  ,   tdtvd_tuner_nonvola.wait_lock2  );
    DTVD_DEBUG_MSG_INFO( "reserved4             =%x\n"  ,   tdtvd_tuner_nonvola.reserved4   );
    DTVD_DEBUG_MSG_INFO( "reserved5             =%x\n"  ,   tdtvd_tuner_nonvola.reserved5   );
    DTVD_DEBUG_MSG_INFO( "cn_qpsk1_2_m          =%x\n"  ,   tdtvd_tuner_nonvola.cn_qpsk1_2_m    );
    DTVD_DEBUG_MSG_INFO( "cn_qpsk1_2_h          =%x\n"  ,   tdtvd_tuner_nonvola.cn_qpsk1_2_h    );
    DTVD_DEBUG_MSG_INFO( "cn_qpsk2_3_m          =%x\n"  ,   tdtvd_tuner_nonvola.cn_qpsk2_3_m    );
    DTVD_DEBUG_MSG_INFO( "cn_qpsk2_3_h          =%x\n"  ,   tdtvd_tuner_nonvola.cn_qpsk2_3_h    );
    DTVD_DEBUG_MSG_INFO( "cn_16qam1_2_m         =%x\n"  ,   tdtvd_tuner_nonvola.cn_16qam1_2_m   );
    DTVD_DEBUG_MSG_INFO( "cn_16qam1_2_h         =%x\n"  ,   tdtvd_tuner_nonvola.cn_16qam1_2_h   );
    DTVD_DEBUG_MSG_INFO( "cn_qpsk1_2_eco_m      =%x\n"  ,   tdtvd_tuner_nonvola.cn_qpsk1_2_eco_m    );
    DTVD_DEBUG_MSG_INFO( "cn_qpsk1_2_eco_h      =%x\n"  ,   tdtvd_tuner_nonvola.cn_qpsk1_2_eco_h    );
    DTVD_DEBUG_MSG_INFO( "cn_qpsk2_3_eco_m      =%x\n"  ,   tdtvd_tuner_nonvola.cn_qpsk2_3_eco_m    );
    DTVD_DEBUG_MSG_INFO( "cn_qpsk2_3_eco_h      =%x\n"  ,   tdtvd_tuner_nonvola.cn_qpsk2_3_eco_h    );
    DTVD_DEBUG_MSG_INFO( "cn_16qam1_2_eco_m     =%x\n"  ,   tdtvd_tuner_nonvola.cn_16qam1_2_eco_m   );
    DTVD_DEBUG_MSG_INFO( "cn_16qam1_2_eco_h     =%x\n"  ,   tdtvd_tuner_nonvola.cn_16qam1_2_eco_h   );
    DTVD_DEBUG_MSG_INFO( "mode_on               =%x\n"  ,   tdtvd_tuner_nonvola.mode_on );
    DTVD_DEBUG_MSG_INFO( "tmcc_on               =%x\n"  ,   tdtvd_tuner_nonvola.tmcc_on );
    DTVD_DEBUG_MSG_INFO( "mode_dec_count        =%x\n"  ,   tdtvd_tuner_nonvola.mode_dec_count  );
    DTVD_DEBUG_MSG_INFO( "search_success_count  =%x\n"  ,   tdtvd_tuner_nonvola.search_success_count    );
    DTVD_DEBUG_MSG_INFO( "search_success_thr    =%x\n"  ,   tdtvd_tuner_nonvola.search_success_thr  );
    DTVD_DEBUG_MSG_INFO( "search_time           =%x\n"  ,   tdtvd_tuner_nonvola.search_time );
    DTVD_DEBUG_MSG_INFO( "scan_mode_dec_count   =%x\n"  ,   tdtvd_tuner_nonvola.scan_mode_dec_count );
    DTVD_DEBUG_MSG_INFO( "scan_search_success_count =%x\n", tdtvd_tuner_nonvola.scan_search_success_count   );
    DTVD_DEBUG_MSG_INFO( "reserved10            =%x\n"  ,   tdtvd_tuner_nonvola.reserved10  );
    DTVD_DEBUG_MSG_INFO( "reserved11            =%x\n"  ,   tdtvd_tuner_nonvola.reserved11  );
    DTVD_DEBUG_MSG_INFO( "reserved12            =%x\n"  ,   tdtvd_tuner_nonvola.reserved12  );
    DTVD_DEBUG_MSG_INFO( "reserved13            =%x\n"  ,   tdtvd_tuner_nonvola.reserved13  );
    DTVD_DEBUG_MSG_INFO( "reserved14            =%x\n"  ,   tdtvd_tuner_nonvola.reserved14  );
    DTVD_DEBUG_MSG_INFO( "reserved15            =%x\n"  ,   tdtvd_tuner_nonvola.reserved15  );
    DTVD_DEBUG_MSG_INFO( "reserved16            =%x\n"  ,   tdtvd_tuner_nonvola.reserved16  );
    DTVD_DEBUG_MSG_INFO( "reserved17            =%x\n"  ,   tdtvd_tuner_nonvola.reserved17  );
    DTVD_DEBUG_MSG_INFO( "auto_n2e_cn_qp12      =%x\n"  ,   tdtvd_tuner_nonvola.auto_n2e_cn_qp12    );
    DTVD_DEBUG_MSG_INFO( "auto_n2e_cn_qp23      =%x\n"  ,   tdtvd_tuner_nonvola.auto_n2e_cn_qp23    );
    DTVD_DEBUG_MSG_INFO( "auto_n2e_cn_qam12     =%x\n"  ,   tdtvd_tuner_nonvola.auto_n2e_cn_qam12   );
    DTVD_DEBUG_MSG_INFO( "auto_e2n_cn_qp12      =%x\n"  ,   tdtvd_tuner_nonvola.auto_e2n_cn_qp12    );
    DTVD_DEBUG_MSG_INFO( "auto_e2n_cn_qp23      =%x\n"  ,   tdtvd_tuner_nonvola.auto_e2n_cn_qp23    );
    DTVD_DEBUG_MSG_INFO( "auto_e2n_cn_qam12     =%x\n"  ,   tdtvd_tuner_nonvola.auto_e2n_cn_qam12   );

    return;
}
#endif

static void dtvd_tuner_com_dev_trans_msg_syserr
(
    unsigned char   rank       
)
{
    DTVD_TUNER_MSG_t    msg;
    unsigned int        len = sizeof(DTVD_TUNER_MSG_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &msg, 0x00, len, len );

    if(tdtvd_tuner_core.pipe_handle != D_DTVD_TUNER_NULL)
    {

        msg.header.msg_id = D_DTVD_TUNER_MSGID_DEVERR;

        msg.header.receiver_id = D_DTVD_TUNER_BLOCK_ID_DMM;

        msg.header.sender_id = D_DTVD_TUNER_BLOCK_ID_TUNERDD;

		msg.data.deverr.error_type = rank;

        dtvd_tuner_com_dev_ddsync_write( tdtvd_tuner_core.pipe_handle,
                                     ( void * )&msg,
                                     len );
    }
    DTVD_DEBUG_MSG_EXIT();

    return;
}

