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

#include "dtvd_tuner_core.h"

#include "dtvd_tuner_tuner.h"
#include "dtvd_tuner_tmcc.h"
#include <linux/delay.h>

static signed int dtvd_tuner_tuner_com_shift_eco_mode( void );
static signed int dtvd_tuner_tuner_com_chg_eco_mode( void );
static signed int dtvd_tuner_tuner_com_shift_normal_mode( void );
static signed int dtvd_tuner_tuner_com_chg_normal_mode( void );

static  DTVD_TUNER_COM_I2C_DATA_t rf_init_data[] = {
  { 0xFA, 0x4A, { 0x00, 0x00 }} 
, { 0x86, 0xE7, { 0x00, 0x00 }} 
, { 0x85, 0x28, { 0x00, 0x00 }} 

, { 0x87, 0x6C, { 0x00, 0x00 }} 

, { 0x88, 0x0C, { 0x00, 0x00 }} 
, { 0xC0, 0x01, { 0x00, 0x00 }} 
, { 0xC1, 0x00, { 0x00, 0x00 }} 
, { 0xC2, 0x01, { 0x00, 0x00 }} 
, { 0xC3, 0x00, { 0x00, 0x00 }} 
, { 0xEB, 0xAE, { 0x00, 0x00 }} 
, { 0xEC, 0xC7, { 0x00, 0x00 }} 

, { 0xF7, 0x27, { 0x00, 0x00 }} 

, { 0xFE, 0x0C, { 0x00, 0x00 }} 
};

static DTVD_TUNER_COM_I2C_DATA_t rf_soft_reset_data[] = {
  { 0xFF, 0x00, { 0x00, 0x00 }}
};

static DTVD_TUNER_COM_I2C_DATA_t rf_AVDE_init_data[] = {
  { 0x97, 0x08, { 0x00, 0x00 }}
, { 0x97, 0x00, { 0x00, 0x00 }}
};

static DTVD_TUNER_COM_I2C_DATA_t seq_start_data[] = {
  { 0x08, 0x00, { 0x00, 0x00 }}
};

void dtvd_tuner_tuner_com_set_status
(
    signed int next_status              
)
{
    DTVD_DEBUG_MSG_ENTER( next_status, 0, 0 );

    tdtvd_tuner_tuner.status = next_status;

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_tuner_com_pwr_on_pega( void )
{
    unsigned long  addr;
    unsigned char  data;

    gpio_direction_output(D_DTVD_TUNER_PORT_NTV_RST, 0x00);

    addr = D_DTVD_TUNER_PSCNT_LDO6;
    dtvd_tuner_com_dev_powic_read( addr,           
                                   &data );        

    data = ( unsigned char )( data | D_DTVD_TUNER_PSCNT_LDO6_ON );
    dtvd_tuner_com_dev_powic_write( addr,          
                                    data );        

    addr = D_DTVD_TUNER_PSCNT_DCDC1;
    dtvd_tuner_com_dev_powic_read( addr,           
                                   &data );        

    data = ( unsigned char )( data | D_DTVD_TUNER_PSCNT_DCDC1_ON );
    dtvd_tuner_com_dev_powic_write( addr,          
                                    data );        
    return;
}

void dtvd_tuner_tuner_com_pwr_on_tuner( void )
{

    dtvd_tuner_timer_sleep_on_timeout((signed long)tdtvd_tuner_tuner.wait_refclk_time);

}

void dtvd_tuner_tuner_com_pwr_off( void )
{

    unsigned long  addr;
    unsigned char  data;

    dtvd_tuner_tsprcv_stop_receiving();

    addr = D_DTVD_TUNER_PSCNT_LDO14;
    dtvd_tuner_com_dev_powic_read( addr,           
                                   &data );        

    data = ( unsigned char )( data & ( unsigned char )( D_DTVD_TUNER_PSCNT_LDO14_OFF ) );
    dtvd_tuner_com_dev_powic_write( addr,          
                                    data );        

    dtvd_tuner_timer_sleep_on_timeout( D_DTVD_TUNER_TIMER_PWROFF1_MSEC );

    dtvd_tuner_tuner_com_reset();

    addr = D_DTVD_TUNER_PSCNT_LDO7;
    dtvd_tuner_com_dev_powic_read( addr,           
                                   &data );        

    data = ( unsigned char )( data & ( unsigned char )( D_DTVD_TUNER_PSCNT_LDO7_OFF ) );
    dtvd_tuner_com_dev_powic_write( addr,          
                                    data );        

    dtvd_tuner_timer_sleep_on_timeout( D_DTVD_TUNER_TIMER_PWROFF2_MSEC );

    addr = D_DTVD_TUNER_PSCNT_LDO6;
    dtvd_tuner_com_dev_powic_read( addr,           
                                   &data );        

    data = ( unsigned char )( data & ( unsigned char )( D_DTVD_TUNER_PSCNT_LDO6_OFF ) );
    dtvd_tuner_com_dev_powic_write( addr,          
                                    data );        

    addr = D_DTVD_TUNER_PSCNT_DCDC1;
    dtvd_tuner_com_dev_powic_read( addr,           
                                   &data );        

    data = ( unsigned char )( data & ( unsigned char )( D_DTVD_TUNER_PSCNT_DCDC1_OFF ) );
    dtvd_tuner_com_dev_powic_write( addr,          
                                    data );        

    return;
}

signed int dtvd_tuner_tuner_com_init( void )
{
//    unsigned long  addr;
//    unsigned char  data;
    signed   int   ret;
    unsigned long  length;
    DTVD_TUNER_COM_I2C_DATA_t dtvd_tuner_com_i2c_data;

    DTVD_TUNER_COM_I2C_DATA_t lna_init_data[] =
    {
        { 0xE3, 0x00, { 0x00, 0x00 } }
    };

    DTVD_TUNER_COM_I2C_DATA_t spurious_off_data[] =
    {
        { 0x28, 0xC0, { 0x00, 0x00 } },
        { 0x29, 0x00, { 0x00, 0x00 } },
        { 0x2A, 0x00, { 0x00, 0x00 } },
        { 0x2B, 0x00, { 0x00, 0x00 } }
    };

    DTVD_TUNER_COM_I2C_DATA_t lna_force_off_data[] =
    {
        { 0xE0, 0x04, { 0x00, 0x00 } }
    };

    dtvd_tuner_tuner_com_cancel_reset();

//    addr = D_DTVD_TUNER_PSCNT_LDO14;
//    dtvd_tuner_com_dev_powic_read( addr,           
//                                   &data );        

//    data = ( unsigned char )( data | D_DTVD_TUNER_PSCNT_LDO14_ON );
//    dtvd_tuner_com_dev_powic_write( addr,          
 //                                   data );        

    dtvd_tuner_com_dev_int_regist( D_DTVD_TUNER_INT_MASK_SET );

    dtvd_tuner_com_dev_int_enable();

    dtvd_tuner_timer_mdelay( 2 );

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( lna_init_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    length = D_DTVD_TUNER_REG_NO1;
    dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_RESET;
    dtvd_tuner_com_i2c_data.data = 0x00;
    ret = dtvd_tuner_com_dev_i2c_write( length,                      
                                        &dtvd_tuner_com_i2c_data );  
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_timer_mdelay( D_DTVD_TUNER_TIMER_SOFTRST_MSEC );

    length = D_DTVD_TUNER_REG_NO1;
    dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_RESET;
    dtvd_tuner_com_i2c_data.data = 0xFF;
    ret = dtvd_tuner_com_dev_i2c_write( length,                      
                                        &dtvd_tuner_com_i2c_data );  
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( lna_force_off_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( rf_soft_reset_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( rf_AVDE_init_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = dtvd_tuner_tuner_com_ofdm_init();
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    if ( ( tdtvd_tuner_core.omt_flag == D_DTVD_TUNER_ON ) &&
         ( tdtvd_tuner_nonvola.reserved4 == D_DTVD_TUNER_ON) )
    {

        ret = DTVD_TUNER_COM_DEV_I2C_WRITE( spurious_off_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            return D_DTVD_TUNER_NG;
        }
    }

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( rf_init_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = dtvd_tuner_tsprcv_start_receiving();
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_tuner_com_ofdm_init( void )
{

    DTVD_TUNER_COM_I2C_DATA_t cmd_data[] = {
      { 0x08, 0x01, { 0x00, 0x00 }} 
    , { 0x28, 0x01, { 0x00, 0x00 }} 
    , { 0x29, 0x00, { 0x00, 0x00 }}
    , { 0x2A, 0x00, { 0x00, 0x00 }}
    , { 0x2B, 0x11, { 0x00, 0x00 }}
    , { 0x04, 0x00, { 0x00, 0x00 }} 
    , { 0x05, 0x00, { 0x00, 0x00 }}
    , { 0x04, 0x01, { 0x00, 0x00 }}
    , { 0x05, 0x35, { 0x00, 0x00 }}
    , { 0x04, 0x02, { 0x00, 0x00 }}
    , { 0x05, 0x0E, { 0x00, 0x00 }}
    , { 0x04, 0x03, { 0x00, 0x00 }}
    , { 0x05, 0xD8, { 0x00, 0x00 }}
    , { 0xD0, 0x00, { 0x00, 0x00 }} 
    , { 0x01, 0x08, { 0x00, 0x00 }} 
    , { 0x04, 0x0A, { 0x00, 0x00 }} 
    , { 0x05, 0xE4, { 0x00, 0x00 }}
    , { 0x04, 0x0B, { 0x00, 0x00 }} 
    , { 0x05, 0xDD, { 0x00, 0x00 }}
    , { 0x04, 0x0E, { 0x00, 0x00 }} 
    , { 0x05, 0x00, { 0x00, 0x00 }}
    , { 0x04, 0x0F, { 0x00, 0x00 }}
    , { 0x05, 0x1E, { 0x00, 0x00 }}
    , { 0x28, 0x46, { 0x00, 0x00 }} 
    , { 0x29, 0x00, { 0x00, 0x00 }}
    , { 0x2A, 0x12, { 0x00, 0x00 }}
    , { 0x2B, 0x90, { 0x00, 0x00 }}
    , { 0x28, 0x6A, { 0x00, 0x00 }} 
    , { 0x29, 0x00, { 0x00, 0x00 }}
    , { 0x2A, 0x00, { 0x00, 0x00 }}
    , { 0x2B, 0x00, { 0x00, 0x00 }}
    , { 0x28, 0x69, { 0x00, 0x00 }} 
    , { 0x29, 0x00, { 0x00, 0x00 }}
    , { 0x2A, 0x00, { 0x00, 0x00 }}
    , { 0x2B, 0x07, { 0x00, 0x00 }}
    , { 0x3B, 0x65, { 0x00, 0x00 }} 
    , { 0x3C, 0x05, { 0x00, 0x00 }}
    , { 0x3B, 0x7D, { 0x00, 0x00 }}
    , { 0x3C, 0x02, { 0x00, 0x00 }}
    , { 0x28, 0x03, { 0x00, 0x00 }} 
    , { 0x29, 0x05, { 0x00, 0x00 }}
    , { 0x2A, 0x32, { 0x00, 0x00 }}
    , { 0x2B, 0x11, { 0x00, 0x00 }}
    , { 0x25, 0xD3, { 0x00, 0x00 }} 
    , { 0x28, 0x5A, { 0x00, 0x00 }} 
    , { 0x29, 0x00, { 0x00, 0x00 }} 
    , { 0x2A, 0x00, { 0x00, 0x00 }}
    , { 0x2B, 0x08, { 0x00, 0x00 }}
    , { 0x28, 0x3D, { 0x00, 0x00 }} 
    , { 0x29, 0x00, { 0x00, 0x00 }}
    , { 0x2A, 0x00, { 0x00, 0x00 }}
    , { 0x2B, 0xC6, { 0x00, 0x00 }}
    , { 0x50, 0xDC, { 0x00, 0x00 }} 
    , { 0x51, 0x01, { 0x00, 0x00 }}
    , { 0x50, 0xDD, { 0x00, 0x00 }}
    , { 0x51, 0x00, { 0x00, 0x00 }}
    , { 0x5E, 0x01, { 0x00, 0x00 }}
    };

    return DTVD_TUNER_COM_DEV_I2C_WRITE( cmd_data );

}

signed int dtvd_tuner_tuner_com_tuner_int_disable( void )
{
    signed   int              ret;

    ret = dtvd_tuner_tuner_com_i2c_bitset(
            D_DTVD_TUNER_REG_SEARCH_IRQCTL,
            0,
            D_DTVD_TUNER_BIT_HI
    );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_tuner_com_sequence_init( void )
{
    signed   int                ret;
    unsigned long               length;
    DTVD_TUNER_COM_I2C_DATA_t   dtvd_tuner_com_i2c_data;

    length = D_DTVD_TUNER_REG_NO1;
    dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_STATE_INIT;
    dtvd_tuner_com_i2c_data.data = 0x01;
    ret = dtvd_tuner_com_dev_i2c_write( length,                      
                                        &dtvd_tuner_com_i2c_data );  
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_tuner_com_iffreq_set( void )
{

    signed   int              ret;

    ret = dtvd_tuner_com_dev_i2c_write(
            D_DTVD_TUNER_IFFREQ_INIT_TBL_SIZE ,
            ( DTVD_TUNER_COM_I2C_DATA_t * )dtvd_tuner_iffreq_init_tbl
    );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_tuner_com_rficpow( void )
{

    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_tuner_com_rficpow_eco( void )
{

    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_tuner_com_rfic_send_tuneset
(

    unsigned char ch,                                          
    unsigned int kind                                          

)
{
    signed int                      ret;

    DTVD_TUNER_COM_I2C_DATA_t rc_calib_start_data[] = {
      { 0x88, 0x0D, { 0x00, 0x00 }}
    };

    DTVD_TUNER_COM_I2C_DATA_t rc_calib_verify_data[] = {
      { 0x88, 0x0C, { 0x00, 0x00 }}
    };

    DTVD_TUNER_COM_I2C_DATA_t vco_data[] = {
      { 0xE3, 0x00, { 0x00, 0x00 }}
    };

    DTVD_TUNER_COM_I2C_DATA_t pll_data[] = {
      { 0x81, 0x00, { 0x00, 0x00 }} 
    , { 0x82, 0x00, { 0x00, 0x00 }} 
    , { 0x83, 0x00, { 0x00, 0x00 }} 
    };

    unsigned char pll_data_tbl[][3] = {
      { 0x1C ,0x67 ,0x34 } 
    , { 0x0E ,0x34 ,0x2F } 
    , { 0x0E ,0x35 ,0x24 } 
    , { 0x1C ,0x6B ,0x32 } 
    , { 0x1C ,0x6C ,0x3C } 
    , { 0x0E ,0x37 ,0x23 } 
    , { 0x0E ,0x37 ,0x38 } 
    , { 0x1C ,0x70 ,0x3A } 
    , { 0x1C ,0x72 ,0x24 } 
    , { 0x0E ,0x39 ,0x37 } 
    , { 0x0E ,0x3A ,0x2C } 
    , { 0x1C ,0x76 ,0x22 } 
    , { 0x1C ,0x77 ,0x2C } 
    , { 0x1C ,0x78 ,0x36 } 
    , { 0x0E ,0x3D ,0x20 } 
    , { 0x1C ,0x7B ,0x2A } 
    , { 0x1C ,0x7C ,0x34 } 
    , { 0x0E ,0x3E ,0x3F } 
    , { 0x0E ,0x3F ,0x34 } 
    , { 0x1C ,0x80 ,0x32 } 
    , { 0x1C ,0x81 ,0x14 } 
    , { 0x0E ,0x41 ,0x33 } 
    , { 0x0E ,0x42 ,0x28 } 
    , { 0x1C ,0x85 ,0x3A } 
    , { 0x1C ,0x87 ,0x24 } 
    , { 0x0E ,0x44 ,0x27 } 
    , { 0x0E ,0x44 ,0x3C } 
    , { 0x0E ,0x45 ,0x31 } 
    , { 0x1C ,0x8C ,0x2C } 
    , { 0x1C ,0x8D ,0x36 } 
    , { 0x0E ,0x47 ,0x30 } 
    , { 0x1C ,0x90 ,0x2A } 
    , { 0x1C ,0x91 ,0x34 } 
    , { 0x0E ,0x49 ,0x2F } 
    , { 0x0E ,0x4A ,0x24 } 
    , { 0x0E ,0x4A ,0x39 } 
    , { 0x1C ,0x96 ,0x3C } 
    , { 0x1C ,0x98 ,0x26 } 
    , { 0x0E ,0x4C ,0x38 } 
    , { 0x0E ,0x4D ,0x2D } 
    , { 0x1C ,0x9C ,0x24 } 
    , { 0x1C ,0x9D ,0x2E } 
    , { 0x0E ,0x4F ,0x2C } 
    , { 0x0E ,0x50 ,0x21 } 
    , { 0x1C ,0xA1 ,0x2C } 
    , { 0x0E ,0x51 ,0x2B } 
    , { 0x0E ,0x52 ,0x20 } 
    , { 0x0E ,0x52 ,0x35 } 
    , { 0x1C ,0xA6 ,0x34 } 
    , { 0x0E ,0x53 ,0x3F } 
    };

    DTVD_TUNER_COM_I2C_DATA_t vco_calib_data[] = {
      { 0xA3, 0x9F, { 0x00, 0x00 }}
    , { 0xA4, 0x53, { 0x00, 0x00 }}
    };

    DTVD_TUNER_COM_I2C_DATA_t vga_calib_start_data[] = {
      { 0xD3, 0x80, { 0x00, 0x00 }}
    , { 0x91, 0x60, { 0x00, 0x00 }}
    , { 0x91, 0x62, { 0x00, 0x00 }}
    };

    DTVD_TUNER_COM_I2C_DATA_t vga_calib_verify_data[] = {
      { 0xD3, 0x00, { 0x00, 0x00 }}
    };

    DTVD_TUNER_COM_I2C_DATA_t upper_lower_data[] = {
      { 0x09, 0x3E, { 0x00, 0x00 }}
    };

    DTVD_TUNER_COM_I2C_DATA_t agc_target_data[] = {
      { 0xF9, 0x90, { 0x00, 0x00 }}
    };

    DTVD_TUNER_COM_I2C_DATA_t lna_enable_data[] = {

      { 0xE3, 0x01, { 0x00, 0x00 }} 
    , { 0xE0, 0x01, { 0x00, 0x00 }} 

    , { 0xE1, 0x02, { 0x00, 0x00 }} 

    , { 0xE2, 0x42, { 0x00, 0x00 }} 

    , { 0xE1, 0x03, { 0x00, 0x00 }} 
    , { 0xE2, 0x04, { 0x00, 0x00 }} 
    , { 0xE1, 0x04, { 0x00, 0x00 }} 
    , { 0xE2, 0x0D, { 0x00, 0x00 }} 
    };

    DTVD_TUNER_COM_I2C_DATA_t agc_start_point_data[] = {
      { 0x28, 0x16, { 0x00, 0x00 }}
    , { 0x29, 0x00, { 0x00, 0x00 }}
    , { 0x2A, 0x00, { 0x00, 0x00 }}
    , { 0x2B, 0x1D, { 0x00, 0x00 }}
    };

    DTVD_TUNER_COM_I2C_DATA_t logic_reset_data[] = {
      { 0x70, 0xF8, { 0x00, 0x00 }}
    , { 0x70, 0xFF, { 0x00, 0x00 }}
    };

    dtvd_tuner_timer_mdelay( 2 );

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( rc_calib_start_data);

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_timer_mdelay( 1 );

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( rc_calib_verify_data);

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    if ( ch <= 24 ) {
        vco_data[0].data = 0x81;
    } else if ( ch <= 34 ) {
        vco_data[0].data = 0x83;
    } else if ( ch <= 49 ) {
        vco_data[0].data = 0x84;
    } else {
        vco_data[0].data = 0x86;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( vco_data);

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_timer_mdelay( 1 );

    pll_data[0].data = pll_data_tbl[ ch - D_DTVD_TUNER_CHANNEL_MIN ][0];
    pll_data[1].data = pll_data_tbl[ ch - D_DTVD_TUNER_CHANNEL_MIN ][1];
    pll_data[2].data = pll_data_tbl[ ch - D_DTVD_TUNER_CHANNEL_MIN ][2];

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( pll_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( vco_calib_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_timer_mdelay( 5 );

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( vga_calib_start_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_timer_mdelay( 1 );

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( vga_calib_verify_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    if ( ch == 33 ) {
        upper_lower_data[0].data = 0x3A;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( upper_lower_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( agc_target_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    if ( kind != D_DTVD_TUNER_TUNE_KIND_NORMAL)
    {
        lna_enable_data[0].data = 0x00; 
        lna_enable_data[1].data = 0x00; 
    }

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( lna_enable_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    if ( kind == D_DTVD_TUNER_TUNE_KIND_SCAN)
    {
        agc_start_point_data[3].data = 0xC0;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( agc_start_point_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( logic_reset_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( seq_start_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_tuner_com_sequence_start( void )
{
    signed   int                ret;
    unsigned long               length;
    DTVD_TUNER_COM_I2C_DATA_t   dtvd_tuner_com_i2c_data;

    length = D_DTVD_TUNER_REG_NO1;
    dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_STATE_INIT;
    dtvd_tuner_com_i2c_data.data = 0x00;
    ret = dtvd_tuner_com_dev_i2c_write( length,                      
                                        &dtvd_tuner_com_i2c_data );  
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_tuner_com_search_set( void )
{

    return;
}

signed int dtvd_tuner_tuner_com_tuner_int_clear( void )
{
    signed   int              ret;
    signed   int              count;
    unsigned long             length;

    unsigned char             data;

    DTVD_TUNER_COM_I2C_DATA_t dtvd_tuner_com_i2c_data;

    for ( count = 0; count < D_DTVD_TUNER_TUNER_INT_SET_NUM ; count++ )
    {

        length = D_DTVD_TUNER_REG_NO1;
        dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_SEARCH_IRQCTL;
        dtvd_tuner_com_i2c_data.data = 0x1E;
        ret = dtvd_tuner_com_dev_i2c_write(
                length,                      
                &dtvd_tuner_com_i2c_data     
        );
        if( ret != D_DTVD_TUNER_OK )
        {
            return D_DTVD_TUNER_NG;
        }

        length = D_DTVD_TUNER_REG_NO1;
        dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_SEARCH_IRQCTL;
        dtvd_tuner_com_i2c_data.data = 0x0E;
        ret = dtvd_tuner_com_dev_i2c_write(
                length,                      
                &dtvd_tuner_com_i2c_data     
        );
        if( ret != D_DTVD_TUNER_OK )
        {
            return D_DTVD_TUNER_NG;
        }



        dtvd_tuner_com_dev_gpio_read( D_DTVD_TUNER_INT_PORT, &data );
        if( data == 0x01 )
        {

            break;
        }
    }
    if ( count == D_DTVD_TUNER_TUNER_INT_SET_NUM )
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_INT_CLEAR,
                                  DTVD_TUNER_TUNER_COM,
                                  (unsigned long)count, 0, 0, 0, 0, 0 );

        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_tuner_com_tuner_int_enable( void )
{
    signed   int              ret;

    ret = dtvd_tuner_tuner_com_i2c_bitset(
            D_DTVD_TUNER_REG_SEARCH_IRQCTL,
            0,
            D_DTVD_TUNER_BIT_LOW
    );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_tuner_com_search_start( void )
{
    signed   int              ret;
    unsigned long             length;
    DTVD_TUNER_COM_I2C_DATA_t dtvd_tuner_com_i2c_data;

    length = D_DTVD_TUNER_REG_NO1;
	
//[2011/09/21 simt-leijun]{
    dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_SEARCH_CTRL;
    dtvd_tuner_com_i2c_data.data = 0x10;
    ret = dtvd_tuner_com_dev_i2c_write(
            length,                      
            &dtvd_tuner_com_i2c_data     
    );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }
//[2011/09/21 simt-leijun]}
	
    dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_SEARCH_CTRL;
    dtvd_tuner_com_i2c_data.data = 0x00;
    ret = dtvd_tuner_com_dev_i2c_write(
            length,                      
            &dtvd_tuner_com_i2c_data     
    );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    length = D_DTVD_TUNER_REG_NO1;
    dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_SEARCH_CTRL;
	
//[2011/09/21 simt-leijun]{	
//    dtvd_tuner_com_i2c_data.data = 0x01;
    dtvd_tuner_com_i2c_data.data = 0x02;
//[2011/09/21 simt-leijun]}

    ret = dtvd_tuner_com_dev_i2c_write(
            length,                      
            &dtvd_tuner_com_i2c_data     
    );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_tuner_com_searchflg_judge( unsigned int *result )
{
    signed   int            ret;
    unsigned char           addr;
    unsigned long           length;
    unsigned char           data;

    if ( result == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TUNER_COM,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    *result = D_DTVD_TUNER_FALSE;
    ret = D_DTVD_TUNER_OK;

    length = D_DTVD_TUNER_REG_NO1;
    addr = D_DTVD_TUNER_REG_SEARCH_IRQ;
    data = 0x00;
    ret = dtvd_tuner_com_dev_i2c_read( length, &addr, &data );
    if( ret != D_DTVD_TUNER_OK )
    {
        *result = D_DTVD_TUNER_FALSE;
        return D_DTVD_TUNER_NG;
    }

    data = data & 0x01;

    if( data == 0x01 )
    {
        *result = D_DTVD_TUNER_TRUE;
    }
    else if ( data == 0x00 )
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_INT_UNKNOWN,
                                  DTVD_TUNER_TUNER_COM,
                                  data, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }
    else
    {

        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_INT_UNKNOWN,
                                  DTVD_TUNER_TUNER_COM,
                                  data, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;
}

//[2011/09/21 simt-leijun]{
extern signed int check_channel_scan_out(char *search_channels);
//[2011/09/21 simt-leijun]}
signed int dtvd_tuner_tuner_com_search_result_read( unsigned int *result )
{
    signed   int                ret;
    unsigned long               length;
    DTVD_TUNER_COM_I2C_DATA_t   dtvd_tuner_com_i2c_data;
    unsigned char               addr;
    unsigned char               data;
//[2011/09/21 simt-leijun]{
    int i;
    unsigned char search_channel[7];
//[2011/09/21 simt-leijun]}

    if ( result == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TUNER_COM,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    *result = D_DTVD_TUNER_FALSE;

    length = D_DTVD_TUNER_REG_NO1;

//[2011/09/21 simt-leijun]{
    for (i = 0; i < 7; i++)
    {
        dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_SEARCH_SUBA;
        dtvd_tuner_com_i2c_data.data = D_DTVD_TUNER_REG_SEARCH_SUBA_CHANNEL1 + i;
        ret = dtvd_tuner_com_dev_i2c_write( length,                      
                                            &dtvd_tuner_com_i2c_data );  
        if( ret != D_DTVD_TUNER_OK )
        {
            return D_DTVD_TUNER_NG;
        }

        length = D_DTVD_TUNER_REG_NO1;
        addr = D_DTVD_TUNER_REG_SEARCH_SUBD;
        data = 0x00;
        ret = dtvd_tuner_com_dev_i2c_read( length, &addr, &search_channel[i]);
        if( ret != D_DTVD_TUNER_OK )
        {
            return D_DTVD_TUNER_NG;
        }
    }

    ret = check_channel_scan_out(search_channel);
    if (ret == D_DTVD_TUNER_OK)
    {
        *result = D_DTVD_TUNER_TRUE;  
    }
    else
    {
         *result = D_DTVD_TUNER_FALSE;
    }
    return D_DTVD_TUNER_OK;

//[2011/09/21 simt-leijun]}
#if 0
    dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_SEARCH_SUBA;
    dtvd_tuner_com_i2c_data.data = D_DTVD_TUNER_REG_SEARCH_SUBA_CHANNEL1;
    ret = dtvd_tuner_com_dev_i2c_write( length,                      
                                        &dtvd_tuner_com_i2c_data );  
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    length = D_DTVD_TUNER_REG_NO1;
    addr = D_DTVD_TUNER_REG_SEARCH_SUBD;
    data = 0x00;
    ret = dtvd_tuner_com_dev_i2c_read( length, &addr, &data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    if( ( data & D_DTVD_TUNER_SEARCH_CHECK_MASK ) == D_DTVD_TUNER_SEARCH_CHECK_MASK )
    {
        *result = D_DTVD_TUNER_TRUE;
    }
    else
    {
        *result = D_DTVD_TUNER_FALSE;
    }

    return D_DTVD_TUNER_OK;
#endif

}

signed int dtvd_tuner_tuner_com_search_endset( void )
{
    signed   int                ret;
    unsigned long               length;
    DTVD_TUNER_COM_I2C_DATA_t   dtvd_tuner_com_i2c_data;

    length = D_DTVD_TUNER_REG_NO1;
    dtvd_tuner_com_i2c_data.adr  = D_DTVD_TUNER_REG_SEARCH_CTRL;
    dtvd_tuner_com_i2c_data.data = 0x00;
    ret = dtvd_tuner_com_dev_i2c_write( length,                      
                                        &dtvd_tuner_com_i2c_data );  
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}

void dtvd_tuner_tuner_com_term( void )
{

    dtvd_tuner_com_dev_int_disable();

    dtvd_tuner_com_dev_int_unregist();

    dtvd_tuner_timer_sleep_on_timeout( D_DTVD_TUNER_TIMER_HARDRST_MSEC );

}

void dtvd_tuner_tuner_com_infoinit( void )
{

    tdtvd_tuner_tuner.wait_search_time = (unsigned int)(        
            tdtvd_tuner_nonvola.wait_search_int *               
            D_DTVD_TUNER_COEFFICIENT_SEARCH_TIME);              

    tdtvd_tuner_tuner.wait_sync_time = (unsigned int)(          
            tdtvd_tuner_nonvola.wait_mnt_sync *                 
            D_DTVD_TUNER_COEFFICIENT_SYNC_TIME);                

    tdtvd_tuner_tuner.wait_ts_time =                            
            tdtvd_tuner_nonvola.wait_ts;                        

    tdtvd_tuner_tuner.mode_on =                                 
            tdtvd_tuner_nonvola.mode_on;                        

    tdtvd_tuner_tuner.tmcc_on =                                 
            tdtvd_tuner_nonvola.tmcc_on;                        

    tdtvd_tuner_tuner.mode_dec_cnt =                            
            tdtvd_tuner_nonvola.mode_dec_count;                 

    tdtvd_tuner_tuner.search_success_cnt =                      
            tdtvd_tuner_nonvola.search_success_count;           

    tdtvd_tuner_tuner.search_success_thr =                      
            tdtvd_tuner_nonvola.search_success_thr;             

    tdtvd_tuner_tuner.search_time =                             
            tdtvd_tuner_nonvola.search_time;                    

    tdtvd_tuner_tuner.scan_mode_dec_cnt =                       
            tdtvd_tuner_nonvola.scan_mode_dec_count;            

    tdtvd_tuner_tuner.scan_search_success_cnt =                 
            tdtvd_tuner_nonvola.scan_search_success_count;      

    tdtvd_tuner_tuner.wait_refclk_time = (unsigned int)(        
            ( tdtvd_tuner_nonvola.wait_refclk *                 
              D_DTVD_TUNER_COEFFICIENT_REFCLK_TIME) + 10 );     

    tdtvd_tuner_tuner.search_thres =                            
            tdtvd_tuner_nonvola.search_thres;                   

    tdtvd_tuner_tuner.wait_lock1 = ( signed long )(             
            ( tdtvd_tuner_nonvola.wait_lock1 *                  
              D_DTVD_TUNER_COEFFICIENT_LOCKWAIT_TIME ) + 10 );  
    tdtvd_tuner_tuner.wait_lock2 = ( signed long )(             
            ( tdtvd_tuner_nonvola.wait_lock2 *                  
              D_DTVD_TUNER_COEFFICIENT_LOCKWAIT_TIME ) + 10 );  
    tdtvd_tuner_tuner.tuning_cnt_flg =                          
            D_DTVD_TUNER_TUNING_1ST_TIME_TRUE;                  

    return;
}

signed int dtvd_tuner_tuner_com_vaint_factor_clear( void )
{
    signed   int                ret;
    unsigned long               length;
    unsigned long               count;
    DTVD_TUNER_COM_I2C_DATA_t   dtvd_tuner_com_i2c_data[2];

    for ( count = 0;
          count < D_DTVD_TUNER_VAINT_FACTOR_CLEAR_TBL_SIZE;
          count += 2 )
    {
        length = 2;
        dtvd_tuner_memcpy(dtvd_tuner_com_i2c_data,
                         &dtvd_tuner_vaint_factor_clear_tbl[count],
                          sizeof(dtvd_tuner_com_i2c_data),
                          sizeof(dtvd_tuner_com_i2c_data));

        ret = dtvd_tuner_com_dev_i2c_write(
                length,
                dtvd_tuner_com_i2c_data
        );
        if( ret != D_DTVD_TUNER_OK )
        {
            return D_DTVD_TUNER_NG;
        }
    }

    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_tuner_com_tuner_reset( void )
{
    signed   int                ret;

    DTVD_TUNER_COM_I2C_DATA_t cmd_dat[] = {
      { 0xD0, 0x00, { 0x00, 0x00 }} 
    , { 0x73, 0x00, { 0x00, 0x00 }} 
    };

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( cmd_dat );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( rf_soft_reset_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( rf_init_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( seq_start_data);

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_tuner_com_tuner_stop( void )
{

    DTVD_TUNER_COM_I2C_DATA_t cmd_dat[] = {
      { 0x08, 0x01, { 0x00, 0x00 }} 
    , { 0xE3, 0x00, { 0x00, 0x00 }} 
    , { 0x73, 0x01, { 0x00, 0x00 }} 
    , { 0xD0, 0x33, { 0x00, 0x00 }} 
    };

    return DTVD_TUNER_COM_DEV_I2C_WRITE( cmd_dat);

}

signed int dtvd_tuner_tuner_com_set_mode_tmcc_info(
    unsigned int kind
)
{
    signed   int                ret;

    DTVD_TUNER_COM_I2C_DATA_t cmd_dat[] = {
      { 0x07, 0x00, { 0x00, 0x00 }} 
    , { 0xF0, 0x00, { 0x00, 0x00 }} 
    };

    if ( kind == D_DTVD_TUNER_TUNE_KIND_SCAN )
    {
        cmd_dat[0].data = tdtvd_tuner_tuner.mode_on;
        cmd_dat[1].data = tdtvd_tuner_tuner.tmcc_on;
    }

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( cmd_dat );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_tuner_com_search_count_set(
    unsigned int kind
)
{
    signed   int                ret;
    unsigned long               length;
    DTVD_TUNER_COM_I2C_DATA_t   dtvd_tuner_com_i2c_data[2];

    DTVD_TUNER_COM_I2C_DATA_t cmd_dat[] = {
      { 0x40, 0x35, { 0x00, 0x00 }} 
    , { 0x41, 0x00, { 0x00, 0x00 }}
    , { 0x40, 0x0D, { 0x00, 0x00 }} 
    , { 0x41, 0x00, { 0x00, 0x00 }}
    };

    dtvd_tuner_com_i2c_data[ 0 ].adr  = D_DTVD_TUNER_REG_SEARCH_MDCNT;
    dtvd_tuner_com_i2c_data[ 1 ].adr  = D_DTVD_TUNER_REG_SEARCH_CNT;
    if ( kind == D_DTVD_TUNER_TUNE_KIND_SEARCH )
    {   
        dtvd_tuner_com_i2c_data[ 0 ].data = tdtvd_tuner_tuner.mode_dec_cnt;
        dtvd_tuner_com_i2c_data[ 1 ].data = tdtvd_tuner_tuner.search_success_cnt;
    }
    else
    {   
        dtvd_tuner_com_i2c_data[ 0 ].data = tdtvd_tuner_tuner.scan_mode_dec_cnt;
        dtvd_tuner_com_i2c_data[ 1 ].data = tdtvd_tuner_tuner.scan_search_success_cnt;
    }

    length = 2;
    ret = dtvd_tuner_com_dev_i2c_write(
        length,
        dtvd_tuner_com_i2c_data
    );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    cmd_dat[1].data          = tdtvd_tuner_tuner.search_success_thr;
    cmd_dat[3].data          = tdtvd_tuner_tuner.search_time;

    ret = DTVD_TUNER_COM_DEV_I2C_WRITE( cmd_dat );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}



extern unsigned char get_scan_channel_num(void);
signed int dtvd_tuner_tuner_com_search_resreg_set( void )
{
    signed   int                ret;
    unsigned long               length;
    DTVD_TUNER_COM_I2C_DATA_t   dtvd_tuner_com_i2c_data[1];
//[2011/09/21 simt-leijun]{
    unsigned char channel = get_scan_channel_num();
    dtvd_tuner_com_i2c_data[ 0 ].adr  = D_DTVD_TUNER_REG_SEARCH_START;
    dtvd_tuner_com_i2c_data[ 0 ].data = channel;

    length = 1;
    ret = dtvd_tuner_com_dev_i2c_write(
        length,
        dtvd_tuner_com_i2c_data
    );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }


    dtvd_tuner_com_i2c_data[ 0 ].adr  = D_DTVD_TUNER_REG_SEARCH_END;
    dtvd_tuner_com_i2c_data[ 0 ].data = channel;

    length = 1;
    ret = dtvd_tuner_com_dev_i2c_write(
        length,
        dtvd_tuner_com_i2c_data
    );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }
//[2011/09/21 simt-leijun]}

    dtvd_tuner_com_i2c_data[ 0 ].adr  = D_DTVD_TUNER_REG_SEARCH_CHANNEL;
    dtvd_tuner_com_i2c_data[ 0 ].data = channel;

    length = 1;
    ret = dtvd_tuner_com_dev_i2c_write(
        length,
        dtvd_tuner_com_i2c_data
    );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_tuner_com_set_eco
(
    unsigned char mode      
)
{
    signed int ret;

    ret = D_DTVD_TUNER_OK;

    if( mode == D_DTVD_TUNER_ON )
    {
        switch( tdtvd_tuner_tuner.eco_mode )
        {
            case D_DTVD_TUNER_ECO_MODE_OFF:
                ret = dtvd_tuner_tuner_com_shift_eco_mode();
                break;

            case D_DTVD_TUNER_ECO_MODE_OFF_SHIFT:
                ret = dtvd_tuner_tuner_com_chg_eco_mode();
                break;

            case D_DTVD_TUNER_ECO_MODE_ON:
            case D_DTVD_TUNER_ECO_MODE_ON_SHIFT:
                break;

            default:

                DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                          DTVD_TUNER_TUNER_COM,
                                          0, 0, 0, 0, 0, 0 );
                return D_DTVD_TUNER_NG;
        }
    }

    else if(  mode == D_DTVD_TUNER_OFF )
    {
        switch( tdtvd_tuner_tuner.eco_mode )
        {
            case D_DTVD_TUNER_ECO_MODE_ON:
                ret = dtvd_tuner_tuner_com_shift_normal_mode();
                break;

            case D_DTVD_TUNER_ECO_MODE_ON_SHIFT:
                ret = dtvd_tuner_tuner_com_chg_normal_mode();
                break;

            case D_DTVD_TUNER_ECO_MODE_OFF:
            case D_DTVD_TUNER_ECO_MODE_OFF_SHIFT:
                break;

            default:

                DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                          DTVD_TUNER_TUNER_COM,
                                          0, 0, 0, 0, 0, 0 );
                return D_DTVD_TUNER_NG;
        }
    }
    else
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_TUNER_COM,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;
}

static signed int dtvd_tuner_tuner_com_shift_eco_mode( void )
{
    signed int ret;

    ret = dtvd_tuner_tuner_com_rficpow_eco();
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_ECO,
                            D_DTVD_TUNER_TIMER_ECO_MSEC );

    tdtvd_tuner_tuner.eco_mode = D_DTVD_TUNER_ECO_MODE_ON_SHIFT;

    return D_DTVD_TUNER_OK;
}

static signed int dtvd_tuner_tuner_com_chg_eco_mode( void )
{
    signed int ret;

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_ECO );

    ret = dtvd_tuner_tuner_com_rficpow_eco();
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_ECO,
                            D_DTVD_TUNER_TIMER_ECO_MSEC );

    tdtvd_tuner_tuner.eco_mode = D_DTVD_TUNER_ECO_MODE_ON_SHIFT;

    return D_DTVD_TUNER_OK;
}

static signed int dtvd_tuner_tuner_com_shift_normal_mode( void )
{
    signed int ret;

    ret = dtvd_tuner_tuner_com_rficpow();
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_ECO,
                            D_DTVD_TUNER_TIMER_ECO_MSEC );

    tdtvd_tuner_tuner.eco_mode = D_DTVD_TUNER_ECO_MODE_OFF_SHIFT;

    return D_DTVD_TUNER_OK;
}

static signed int dtvd_tuner_tuner_com_chg_normal_mode( void )
{
    signed int ret;

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_ECO );

    ret = dtvd_tuner_tuner_com_rficpow();
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_ECO,
                            D_DTVD_TUNER_TIMER_ECO_MSEC );

    tdtvd_tuner_tuner.eco_mode = D_DTVD_TUNER_ECO_MODE_OFF_SHIFT;

    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_tuner_com_timeout_eco( void )
{
    unsigned char flag;

    flag = tdtvd_tuner_tuner.eco_mode;

    if( tdtvd_tuner_tuner.eco_mode == D_DTVD_TUNER_ECO_MODE_ON_SHIFT )
    {

        flag = D_DTVD_TUNER_ECO_MODE_ON;
    }
    else if( tdtvd_tuner_tuner.eco_mode == D_DTVD_TUNER_ECO_MODE_OFF_SHIFT )
    {

        flag = D_DTVD_TUNER_ECO_MODE_OFF;
    }
    else
    {
        ;
    }

    tdtvd_tuner_tuner.eco_mode = flag;

    return ;
}

void dtvd_tuner_tuner_com_shift_stop( void )
{
    unsigned char   next_mode;

    if( tdtvd_tuner_tuner.eco_mode == D_DTVD_TUNER_ECO_MODE_OFF )
    {
        next_mode = D_DTVD_TUNER_ECO_MODE_OFF;
    }
    else if( tdtvd_tuner_tuner.eco_mode == D_DTVD_TUNER_ECO_MODE_ON )
    {
        next_mode = D_DTVD_TUNER_ECO_MODE_ON;
    }
    else if( tdtvd_tuner_tuner.eco_mode == D_DTVD_TUNER_ECO_MODE_OFF_SHIFT )
    {
        next_mode = D_DTVD_TUNER_ECO_MODE_OFF;

        dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_ECO );
    }
    else if( tdtvd_tuner_tuner.eco_mode == D_DTVD_TUNER_ECO_MODE_ON_SHIFT )
    {
        next_mode = D_DTVD_TUNER_ECO_MODE_ON;

        dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_ECO );
    }
    else
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_TUNER_COM,
                                  0, 0, 0, 0, 0, 0 );
        return ;
    }

    tdtvd_tuner_tuner.eco_mode = next_mode;

    return ;
}

signed int dtvd_tuner_tuner_com_modegi_get( void )
{

    signed   int            ret;
    unsigned char           addr;
    unsigned long           length;
    unsigned char           data;
    unsigned int            wk;
    unsigned int            mode;
    unsigned int            gi;
    unsigned char           *set_mode;
    unsigned char           *set_gi;

    ret = D_DTVD_TUNER_OK;

    set_mode = &tdtvd_tuner_monitor.tune.adj.mode;
    set_gi   = &tdtvd_tuner_monitor.tune.adj.gi;

    length = D_DTVD_TUNER_REG_NO1;
    addr = D_DTVD_TUNER_REG_MODE_STAT;
    data = 0x00;
    ret = dtvd_tuner_com_dev_i2c_read( length, &addr, &data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    wk = (unsigned int)data;
    gi   = ( wk & 0x03 );
    mode = ( wk & 0x0C ) >> 2 ;

    switch ( gi )
    {
        case 0x00: *set_gi = D_DTVD_TUNER_GI_1_4;  break;
        case 0x01: *set_gi = D_DTVD_TUNER_GI_1_8;  break;
        case 0x02: *set_gi = D_DTVD_TUNER_GI_1_16; break;
        case 0x03: *set_gi = D_DTVD_TUNER_GI_NONE; break;
        default  :
            return D_DTVD_TUNER_NG;
    }

    switch ( mode )
    {
        case 0x00: *set_mode = D_DTVD_TUNER_TSMODE_NONE;  break;
        case 0x01: *set_mode = D_DTVD_TUNER_TSMODE_MODE2; break;
        case 0x02: *set_mode = D_DTVD_TUNER_TSMODE_MODE3; break;
        default  :
            return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}

void dtvd_tuner_tuner_com_wait_lock( void )      
{                                                
    signed long wait_lock;                       

    wait_lock = tdtvd_tuner_tuner.wait_lock2;    

    if( tdtvd_tuner_tuner.tuning_cnt_flg         
        == D_DTVD_TUNER_TUNING_1ST_TIME_TRUE )   
    {                                            
        wait_lock = tdtvd_tuner_tuner.wait_lock1;
        tdtvd_tuner_tuner.tuning_cnt_flg         
            = D_DTVD_TUNER_TUNING_1ST_TIME_FALSE;
    }                                            

    dtvd_tuner_timer_sleep_on_timeout(           
        wait_lock );                             
}                                                

signed int dtvd_tuner_tuner_com_i2c_bitset
(
    unsigned char addr,
    unsigned int  bit,
    unsigned char kind
)
{
    signed int ret;
    unsigned long length;
    unsigned char data;
    unsigned int  dmy=0x01;
    unsigned int  wk;                               
    DTVD_TUNER_COM_I2C_DATA_t dtvd_tuner_com_i2c_data;

    ret = D_DTVD_TUNER_OK;

    length = D_DTVD_TUNER_REG_NO1;
    data = 0x00;
    ret = dtvd_tuner_com_dev_i2c_read( length, &addr, &data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    wk   = (unsigned int )data;
    if ( kind == D_DTVD_TUNER_BIT_HI )
    {   
        data = (unsigned char)( wk |    ( dmy << bit )   );
    }
    else
    {   
        data = (unsigned char)( wk & ( ~( dmy << bit ) ) );
    }
    dtvd_tuner_com_i2c_data.data = data;

    length = D_DTVD_TUNER_REG_NO1;
    dtvd_tuner_com_i2c_data.adr = addr;
    ret = dtvd_tuner_com_dev_i2c_write( length,
                              &dtvd_tuner_com_i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_tuner_com_rfic_sequencer   
(                                                
    unsigned char kind                           
)                                                
{                                                
    signed int ret;                              
    DTVD_TUNER_COM_I2C_DATA_t send_data;         

    send_data.adr = D_DTVD_TUNER_RF_REG_MC_RG1;  

    switch( kind )                               
    {                                            
        case D_DTVD_TUNER_SEQ_RESET :            
            send_data.data =                     
                  D_DTVD_TUNER_RF_DAT_SEQ_RESET; 
            break;                               
        case D_DTVD_TUNER_SEQ_START :            
            send_data.data =                     
                  D_DTVD_TUNER_RF_DAT_SEQ_START; 
            break;                               
        default :                                
            return D_DTVD_TUNER_NG;              
    }                                            
    ret = dtvd_tuner_com_dev_i2c_rfic_write(     
            1, &send_data );                     

    return ret;                                  
}                                                

void dtvd_tuner_tuner_com_cancel_reset( void )
{

    dtvd_tuner_com_dev_gpio_write(
        ( unsigned long )D_DTVD_TUNER_PORT_NTV_RST,
        0x01);

      dtvd_tuner_timer_sleep_on_timeout( D_DTVD_TUNER_RESET_OFF_TIMER );

    return;
}

void dtvd_tuner_tuner_com_reset( void )
{

    dtvd_tuner_com_dev_gpio_write(
        ( unsigned long )D_DTVD_TUNER_PORT_NTV_RST,
        0x00);

    return;
}

