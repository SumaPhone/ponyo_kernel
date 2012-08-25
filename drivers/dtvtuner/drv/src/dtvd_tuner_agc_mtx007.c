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

void dtvd_tuner_agc_mtx_007_msg_core_start_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_agc_inevt_core_start_omt( D_DTVD_TUNER_OK );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_agc_mtx_007_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_monitor.rx.agc.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_agc_inevt_core_stop_omt( D_DTVD_TUNER_OK );

    dtvd_tuner_agc_com_set_status( D_DTVD_TUNER_AGC_001 );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_agc_mtx_007_msg_core_measure_omt( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_agc_com_read();

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_agc_inevt_core_measure_omt( D_DTVD_TUNER_NG );

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    tdtvd_tuner_monitor.rx.agc.state = D_DTVD_TUNER_MEASURE_STATE_ON;

    dtvd_tuner_agc_inevt_core_measure_omt( D_DTVD_TUNER_OK );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_agc_mtx_007_msg_core_measure_stop_omt( void )
{

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_agc_inevt_core_measure_stop_omt( D_DTVD_TUNER_OK );

    dtvd_tuner_agc_com_set_status( D_DTVD_TUNER_AGC_008 );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

