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

void dtvd_tuner_core_mtx_009_tune_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_009_scan_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_009_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_STOP );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_009_msg_agc_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_tuner_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_009_msg_cn_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_agc_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_009_msg_ber_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_cn_stop();

    dtvd_tuner_core_inevt_autoeco_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

