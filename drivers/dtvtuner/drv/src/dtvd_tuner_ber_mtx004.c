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

void dtvd_tuner_ber_mtx_004_msg_core_get_omt( void )
{

    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_ber_com_start_set();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_ber_inevt_core_measure_omt( D_DTVD_TUNER_NG );

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    tdtvd_tuner_ber.confirm_times = 0;

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_MEASURE_BER,
                            tdtvd_tuner_ber.measure_time );

    dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_005 );

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_ber_mtx_004_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_ber_inevt_core_stop_omt( D_DTVD_TUNER_OK );

    dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

