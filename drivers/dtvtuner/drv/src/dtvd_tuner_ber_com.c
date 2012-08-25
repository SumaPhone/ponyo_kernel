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
#include "dtvd_tuner_ber.h"

void dtvd_tuner_ber_com_set_status
(
    signed int next_state                               
)
{
    DTVD_DEBUG_MSG_ENTER( next_state, 0, 0 );

    tdtvd_tuner_ber.status = next_state;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

signed int dtvd_tuner_ber_com_read( void )
{
    signed int ret;
    unsigned long length;
    unsigned char addr;
    unsigned char data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_RSBERDT0;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       &data  );        

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    tdtvd_tuner_monitor.rx.ber.value.ext = data;

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_RSBERDT1;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       &data  );        

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    tdtvd_tuner_monitor.rx.ber.value.up = data;

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_RSBERDT2;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       &data  );        

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    tdtvd_tuner_monitor.rx.ber.value.low = data;

    tdtvd_tuner_monitor.rx.ber.value.result = D_DTVD_TUNER_GET_RESULT_OK;

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

void dtvd_tuner_ber_com_watch_timer_start( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_WATCH_BER,
                            tdtvd_tuner_ber.cycle_time );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

signed int dtvd_tuner_ber_com_read_flag( unsigned char *data )
{
    signed int ret;
    unsigned long length;
    unsigned char addr;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = D_DTVD_TUNER_OK;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_RSBERCEFLG;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       data  );         

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    return ret;
}

signed int dtvd_tuner_ber_com_start_set( void )
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    i2c_data.adr = D_DTVD_TUNER_REG_RSBERXRST;
    i2c_data.data= 0x00;

    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_RSBERXRST;
    i2c_data.data= 0x01;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_ber_com_start_set_ext_omt( void )
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= 0xD7;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= 0x81;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_ber_com_stop_set_ext_omt( void )
{

    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= 0xD7;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= 0xB9;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();

    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_ber_com_init( void )
{

    tdtvd_tuner_monitor.rx.ber.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    tdtvd_tuner_ber.wait_time = tdtvd_tuner_nonvola.ber_wait;

    tdtvd_tuner_ber.measure_time = ( unsigned int )( tdtvd_tuner_nonvola.ber_cycle * 
                                     D_DTVD_TUNER_COEFFICIENT_BER_CYCLE_TIME );

    tdtvd_tuner_ber.cycle_time = D_DTVD_TUNER_BER_CYCLE_TIME;

    tdtvd_tuner_ber.count_max = D_DTVD_TUNER_BER_TIMER_COUNT;

    tdtvd_tuner_ber.confirm_times = 0;

    return ;
}

signed int dtvd_tuner_ber_com_start_set_init( unsigned short * period )
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( period == NULL )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_RSBERON;
    i2c_data.data= 0x01;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= 0xDC;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= (unsigned char)( (unsigned short)( *period & 0xff00 ) >> 8 );
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= 0xDD;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= (unsigned char)( *period & 0x00ff );
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

