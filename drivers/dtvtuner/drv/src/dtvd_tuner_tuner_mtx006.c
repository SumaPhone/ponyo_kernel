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

void dtvd_tuner_tuner_mtx_006_msg_core_stop( void )
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

void dtvd_tuner_tuner_mtx_006_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_state_tuner_stop();

    dtvd_tuner_tuner_inevt_tmcc_stop();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_006_msg_core_eco_set( void )
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

void dtvd_tuner_tuner_mtx_006_msg_tmcc_infoget( void )
{
    DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_tuner_tmcc_infoget;

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_SYNC );

    tdtvd_tuner_tuner.sync_wait_flg = D_DTVD_TUNER_OFF; 

    if( data->non_std == D_DTVD_TUNER_TMCC_INFOGET_NONSTD )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_nostd( D_DTVD_TUNER_TUNE_NG_CAUSE5 );

    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

	    
	    tdtvd_tuner_monitor.tune.sync_state = D_DTVD_TUNER_SYNC_STATE_SYNC;

        dtvd_tuner_tuner_inevt_core_tmcc();

        dtvd_tuner_tuner_inevt_state_tuner_confirm( D_DTVD_TUNER_STATE_CONFIRM_TSOUT );

        dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_007 );
    }
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_006_tunerdd_state_async( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_tmcc_stop();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_008 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_006_msg_state_confirm( void )
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

void dtvd_tuner_tuner_mtx_006_timeout_sync( void )
{

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_tuner.sync_wait_flg = D_DTVD_TUNER_OFF; 

    dtvd_tuner_tuner_inevt_state_tuner_confirm( D_DTVD_TUNER_STATE_CONFIRM_SYNC );

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_tuner_mtx_006_timeout_eco( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_com_timeout_eco();

    DTVD_DEBUG_MSG_EXIT();
    return;

}

