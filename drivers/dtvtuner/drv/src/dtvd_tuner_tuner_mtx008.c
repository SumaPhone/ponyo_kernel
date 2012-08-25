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
#include "dtvd_tuner_tuner.h"

void dtvd_tuner_tuner_mtx_008_msg_core_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_SYNC );

    tdtvd_tuner_tuner.sync_wait_flg = D_DTVD_TUNER_OFF; 

    dtvd_tuner_tuner_inevt_state_tuner_stop();

    dtvd_tuner_tuner_inevt_tmcc_stop();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_003 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_008_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_state_tuner_stop();

    dtvd_tuner_tuner_inevt_tmcc_stop();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_008_msg_core_eco_set( void )
{
    signed int ret;
    DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_tuner_core_eco_set;

    ret = dtvd_tuner_tuner_com_set_eco( data->mode );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_tuner_mtx_008_msg_tmcc_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_monitor.tune.sync_state = D_DTVD_TUNER_SYNC_STATE_ASYNC;

    if ( tdtvd_tuner_tuner.sync_wait_flg == D_DTVD_TUNER_OFF )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_async();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_008_tunerdd_state_mode_ok( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_tuner_com_modegi_get();
    if( ret != D_DTVD_TUNER_OK )
    {
    DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_tuner_mtx_008_tunerdd_state_sync( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_core_sync();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_008_tunerdd_state_tmcc_ok( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_tmcc_infoget();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_006 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_008_msg_state_confirm( void )
{
    DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_tuner_state_confirm;

    if( data->result == D_DTVD_TUNER_NG )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );
        dtvd_tuner_tuner_inevt_core_tune_ng( D_DTVD_TUNER_TUNE_NG_CAUSE4 );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_008_timeout_sync( void )
{

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_tuner.sync_wait_flg = D_DTVD_TUNER_OFF; 

    dtvd_tuner_tuner_inevt_state_tuner_confirm( D_DTVD_TUNER_STATE_CONFIRM_SYNC );

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_tuner_mtx_008_timeout_eco( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_com_timeout_eco();

    DTVD_DEBUG_MSG_EXIT();
    return;

}

