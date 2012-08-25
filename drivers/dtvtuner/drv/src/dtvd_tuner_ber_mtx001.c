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

void dtvd_tuner_ber_mtx_001_msg_core_end( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_ber_inevt_core_end();

    dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_000 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_mtx_001_msg_core_set( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_ber_mtx_001_msg_core_stop( void )
{

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_ber_inevt_core_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_mtx_001_msg_core_start( void )
{

   signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_ber_com_start_set();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_ber_inevt_core_deverr();

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    tdtvd_tuner_monitor.rx.ber.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_MEASURE_BER_FST,
		                    tdtvd_tuner_ber.wait_time );

    dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_mtx_001_msg_core_start_omt( void )
{
    signed int ret;
    DTVD_TUNER_INEVT_BER_CORE_START_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_ber_core_start_omt;

    if( data->out == D_DTVD_TUNER_BER_MEASURE_OUT_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        ret = dtvd_tuner_ber_com_start_set_ext_omt();
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            dtvd_tuner_ber_inevt_core_start_omt( D_DTVD_TUNER_NG );

            DTVD_DEBUG_MSG_EXIT();
            return;
        }

        dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_006 );
    }
    else
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        ret = dtvd_tuner_ber_com_start_set_init( &data->period );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

            dtvd_tuner_ber_inevt_core_start_omt( D_DTVD_TUNER_NG );

            DTVD_DEBUG_MSG_EXIT();
            return;
        }

        dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_004 );
    }

    dtvd_tuner_ber_inevt_core_start_omt( D_DTVD_TUNER_OK );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_mtx_001_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_monitor.rx.ber.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_ber_inevt_core_stop_omt( D_DTVD_TUNER_OK );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

