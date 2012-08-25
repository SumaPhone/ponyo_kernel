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

void dtvd_tuner_core_mtx_013_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_STOP_OMT );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_tune_dtv_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_ber_start_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_BER_START_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_ber_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_BER_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_ber_get_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_BER_GET_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_cnr_start_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_CNR_START_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_cnr_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_CNR_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_cnr_get_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_CNR_GET_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_agc_start_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_AGC_START_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_agc_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_AGC_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_agc_get_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_AGC_GET_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_msg_agc_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_tuner_stop_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_msg_cn_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_agc_stop_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_013_msg_ber_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

        dtvd_tuner_core_inevt_autoeco_stop_omt();

    dtvd_tuner_core_inevt_cn_stop_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

