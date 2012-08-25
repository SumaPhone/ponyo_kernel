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

void dtvd_tuner_agc_mtx_001_msg_core_end( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_agc_inevt_core_end();

    dtvd_tuner_agc_com_set_status( D_DTVD_TUNER_AGC_000 );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_agc_mtx_001_msg_core_measure_fst( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_AGC,
                            tdtvd_tuner_agc.agc_wait );

    dtvd_tuner_agc_com_set_status( D_DTVD_TUNER_AGC_002 );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_agc_mtx_001_msg_core_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_monitor.rx.agc.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_agc_inevt_core_stop();

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_agc_mtx_001_msg_core_measure_fst_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_AGC,
                            tdtvd_tuner_agc.agc_wait );

    dtvd_tuner_agc_com_set_status( D_DTVD_TUNER_AGC_005 );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_agc_mtx_001_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_monitor.rx.agc.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_agc_inevt_core_stop_omt( D_DTVD_TUNER_OK );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

