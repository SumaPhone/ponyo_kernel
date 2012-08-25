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

void dtvd_tuner_ber_mtx_005_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_MEASURE_BER );

    dtvd_tuner_ber_inevt_core_stop_omt( D_DTVD_TUNER_OK );

    dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_mtx_005_msg_core_getstop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_MEASURE_BER );

    tdtvd_tuner_monitor.rx.ber.value.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;

    dtvd_tuner_ber_inevt_core_measure_omt( D_DTVD_TUNER_OK );

    dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_004 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_mtx_005_timeout_ber( void )
{
    unsigned char data;
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_ber_com_read_flag( &data );

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_ber_inevt_core_measure_omt( D_DTVD_TUNER_NG );

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    if( data == 0x01 )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        ret = dtvd_tuner_ber_com_read();

        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

            dtvd_tuner_ber_inevt_core_measure_omt( D_DTVD_TUNER_NG );

            DTVD_DEBUG_MSG_EXIT();
            return;
        }

        dtvd_tuner_ber_inevt_core_measure_omt( D_DTVD_TUNER_OK );

        dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_004 );

    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

        if( tdtvd_tuner_ber.count_max > tdtvd_tuner_ber.confirm_times )
        {
            DTVD_DEBUG_MSG_ENTER( 5, 0, 0 );

            tdtvd_tuner_ber.confirm_times++;

            dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_MEASURE_BER,
                                    tdtvd_tuner_ber.measure_time );
        }

        else
        {
            DTVD_DEBUG_MSG_ENTER( 6, 0, 0 );

            tdtvd_tuner_monitor.rx.ber.value.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;

            dtvd_tuner_ber_inevt_core_measure_omt( D_DTVD_TUNER_OK );

            dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_004 );
        }

    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

