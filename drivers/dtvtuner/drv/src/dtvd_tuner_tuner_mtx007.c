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

void dtvd_tuner_tuner_mtx_007_msg_core_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_START_TS );

    dtvd_tuner_tuner_inevt_state_tuner_stop();

    dtvd_tuner_tuner_inevt_tmcc_stop();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_003 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_007_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_START_TS );

    dtvd_tuner_tuner_inevt_state_tuner_stop();

    dtvd_tuner_tuner_inevt_tmcc_stop();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_007_msg_core_eco_set( void )
{
    signed int ret;
    DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_tuner_core_eco_set;

    ret = dtvd_tuner_tuner_com_set_eco( data->mode );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_START_TS );

        dtvd_tuner_tuner_inevt_core_deverr();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_tuner_mtx_007_msg_tmcc_infoget( void )
{
    DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_tuner_tmcc_infoget;

    if( data->non_std == D_DTVD_TUNER_TMCC_INFOGET_NONSTD )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_START_TS );

        dtvd_tuner_tuner_inevt_core_nostd( D_DTVD_TUNER_TUNE_NG_CAUSE6 );

        dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_006 );

    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_007_tunerdd_state_ts_start( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_START_TS,
                            tdtvd_tuner_tuner.wait_ts_time );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_007_tunerdd_state_async( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_START_TS );

    dtvd_tuner_tuner_inevt_tmcc_stop();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_008 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_007_msg_state_confirm( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

	
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_007_timeout_ts( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_core_tsout();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_007_timeout_eco( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_com_timeout_eco();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

