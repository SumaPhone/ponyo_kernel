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

void dtvd_tuner_agc_mtx_004_msg_core_measure( void )
{
    signed int ret;

        DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_agc_com_read();

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_WATCH_AGC );

        dtvd_tuner_agc_inevt_core_deverr();

        DTVD_DEBUG_MSG_EXIT();

        return;
    }

    dtvd_tuner_agc_inevt_core_measure();

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_agc_mtx_004_msg_core_stop( void )
{

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_WATCH_AGC );

    tdtvd_tuner_monitor.rx.agc.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_agc_inevt_core_stop();

    dtvd_tuner_agc_com_set_status( D_DTVD_TUNER_AGC_001 );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_agc_mtx_004_timeout_agc_watch( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_agc_com_read();

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_agc_inevt_core_deverr();

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_agc_com_watch_timer_start();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

