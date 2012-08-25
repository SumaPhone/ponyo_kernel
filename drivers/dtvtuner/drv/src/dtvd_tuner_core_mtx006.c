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

void dtvd_tuner_core_mtx_006_tune_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_007 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_scan_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_007 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_008 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_msg_tuner_tsout( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_ON;

    dtvd_tuner_core_trans_msg_sync();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_msg_tuner_nonstd( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.sync_flg == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_OFF;

        dtvd_tuner_core_trans_msg_async();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_msg_tuner_sync( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_msg_tuner_async( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.sync_flg == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_OFF;

        dtvd_tuner_core_trans_msg_async();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_msg_tuner_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_msg_agc_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_msg_tmcc_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_msg_state_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_006_eco_set( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_eco_flag();

    dtvd_tuner_core_inevt_tuner_eco_set();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

