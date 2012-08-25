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

void dtvd_tuner_core_mtx_002_end( void )
{

    DTVD_TUNER_EXEVT_MDL_END_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.ex_mdl_end;

    tdtvd_tuner_core.end_retry_flag = data->mode;

    dtvd_tuner_core_inevt_autoeco_end();

    dtvd_tuner_core_inevt_ber_end();

    dtvd_tuner_core_inevt_cn_end();

    dtvd_tuner_core_inevt_agc_end();

    dtvd_tuner_core_inevt_tuner_end();

    tdtvd_tuner_core.omt_flag = D_DTVD_TUNER_OFF;

    tdtvd_tuner_core.eco_flag = D_DTVD_TUNER_OFF;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_002_tune_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.omt_flag == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_CORE_MTX002,
                                  0, 0, 0, 0, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_core_init_tuneflg();

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV );

    if( tdtvd_tuner_core.stop_state_fst == D_DTVD_TUNER_OFF )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_inevt_ant_init();

        dtvd_tuner_core_inevt_tuner_init();
    }

    tdtvd_tuner_core.stop_state_fst = D_DTVD_TUNER_OFF;

    dtvd_tuner_core_inevt_ant_set();

    dtvd_tuner_core_inevt_tuner_tune_dtv();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_003 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_002_scan_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.omt_flag == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_CORE_MTX002,
                                  0, 0, 0, 0, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_core_init_tuneflg();

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV );

    if( tdtvd_tuner_core.stop_state_fst == D_DTVD_TUNER_OFF )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_inevt_ant_init();

        dtvd_tuner_core_inevt_tuner_init();
    }

    tdtvd_tuner_core.stop_state_fst = D_DTVD_TUNER_OFF;

    dtvd_tuner_core_inevt_ant_set();

    dtvd_tuner_core_inevt_tuner_tune_dtv();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_004 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_002_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.omt_flag == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_CORE_MTX002,
                                  0, 0, 0, 0, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_STOP );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_002_msg_ant_end( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_END );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_002_msg_tuner_end( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ant_end( tdtvd_tuner_core.end_retry_flag );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_002_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.omt_flag == D_DTVD_TUNER_OFF )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_CORE_MTX002,
                                  0, 0, 0, 0, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_002_tune_dtv_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.omt_flag == D_DTVD_TUNER_OFF )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_CORE_MTX002,
                                  0, 0, 0, 0, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_core_init_tuneflg();

    dtvd_tuner_core_set_tuneinfo_omt();

    if( tdtvd_tuner_core.stop_state_fst == D_DTVD_TUNER_OFF )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_inevt_ant_init_omt();

        dtvd_tuner_core_inevt_tuner_init_omt();
    }

    tdtvd_tuner_core.stop_state_fst = D_DTVD_TUNER_OFF;

    dtvd_tuner_core_inevt_ant_set();

    dtvd_tuner_core_inevt_tuner_tune_dtv_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_010 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_002_eco_set( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_eco_flag();

    dtvd_tuner_core_inevt_tuner_eco_set();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

