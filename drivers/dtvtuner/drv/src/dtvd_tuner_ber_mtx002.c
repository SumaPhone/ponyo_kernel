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

void dtvd_tuner_ber_mtx_002_msg_core_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_MEASURE_BER_FST );

    tdtvd_tuner_monitor.rx.ber.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_ber_inevt_core_stop();

    dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_mtx_002_timeout_ber_fst( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_ber.confirm_times = 0;

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_MEASURE_BER,
                            tdtvd_tuner_ber.measure_time );

    dtvd_tuner_ber_com_set_status( D_DTVD_TUNER_BER_003 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

