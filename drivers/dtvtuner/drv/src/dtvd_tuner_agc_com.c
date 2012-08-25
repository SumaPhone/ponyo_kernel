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
#include "dtvd_tuner_agc.h"

void dtvd_tuner_agc_com_set_status
(
    signed int next_state                               
)
{
    DTVD_DEBUG_MSG_ENTER( next_state, 0, 0 );

    tdtvd_tuner_agc.status = next_state;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

signed int dtvd_tuner_agc_com_read( void )
{
    signed int ret;
    unsigned long length;
    unsigned char addr;
    unsigned char data;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    i2c_data.adr = D_DTVD_TUNER_REG_AGC_SUBA;
    i2c_data.data= D_DTVD_TUNER_REG_AGC_SUBA_IFAGCDAC;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_AGC_SUBD;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       &data  );        

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    tdtvd_tuner_monitor.rx.agc.value_x = data;
    tdtvd_tuner_monitor.rx.agc.value_y = data;

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

void dtvd_tuner_agc_com_watch_timer_start( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_WATCH_AGC,
                            tdtvd_tuner_agc.agc_cycle );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_com_get_timer_value( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_agc.agc_wait = tdtvd_tuner_nonvola.agc_wait;

    tdtvd_tuner_agc.agc_cycle = ( unsigned int )( tdtvd_tuner_nonvola.agc_cycle *
                                  D_DTVD_TUNER_COEFFICIENT_AGC_CYCLE_TIME );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

