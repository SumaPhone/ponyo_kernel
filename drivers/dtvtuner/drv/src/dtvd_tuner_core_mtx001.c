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

void dtvd_tuner_core_mtx_001_pwroff( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_tuner_pwroff();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_001_init( void )
{
    DTVD_TUNER_EXEVT_MDL_INIT_t *data = &tdtvd_tuner_msg.buff.ex_mdl_init;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.pipe_handle = data->pipe_handle;

    tdtvd_tuner_core.omt_flag = D_DTVD_TUNER_OFF;

    tdtvd_tuner_core.eco_flag = D_DTVD_TUNER_OFF;

    dtvd_tuner_core_inevt_autoeco_init();

    dtvd_tuner_core_inevt_ber_init();

    dtvd_tuner_core_inevt_cn_init();

    dtvd_tuner_core_inevt_agc_init();

    dtvd_tuner_core_inevt_ant_init();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_001_msg_ant_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_tuner_init();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_001_msg_tuner_pwroff( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_PWROFF );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_000 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_001_msg_tuner_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.stop_state_fst = D_DTVD_TUNER_ON;

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_INIT );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_001_msg_tuner_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_INIT );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_001_init_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.omt_flag = D_DTVD_TUNER_ON;

    dtvd_tuner_core_inevt_ber_init_omt();

    dtvd_tuner_core_inevt_cn_init_omt();

    dtvd_tuner_core_inevt_agc_init_omt();

    dtvd_tuner_core_inevt_ant_init_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_001_msg_ant_init_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_tuner_init_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_001_msg_tuner_init_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.stop_state_fst = D_DTVD_TUNER_ON;

    tdtvd_tuner_core.eco_flag = D_DTVD_TUNER_OFF;

    dtvd_tuner_core_inevt_autoeco_init();

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_INIT_OMT );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_001_msg_tuner_deverr_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_INIT_OMT );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

