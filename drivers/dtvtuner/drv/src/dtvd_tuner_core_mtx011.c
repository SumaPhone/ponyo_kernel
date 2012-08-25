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

void dtvd_tuner_core_mtx_011_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_008 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_tune_dtv_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo_omt();

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_012 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_ber_start_omt( void )
{
    DTVD_TUNER_EXEVT_MDL_BER_START_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.ex_mdl_ber_start_omt;

    dtvd_tuner_core_inevt_ber_start_omt( data->out, data->period );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_ber_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_ber_get_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.sync_flg == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_inevt_ber_measure_omt();
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

        dtvd_tuner_ber_measure_non();

        dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_BER_GET_OMT );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_cnr_start_omt( void )
{
    DTVD_TUNER_EXEVT_MDL_CNR_START_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.ex_mdl_cnr_start_omt;

    dtvd_tuner_core_inevt_cn_start_omt( data->carrier );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_cnr_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_cn_stop_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_cnr_get_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.sync_flg == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_inevt_cn_measure_omt();
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

        dtvd_tuner_cn_measure_non();

        dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_CNR_GET_OMT );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_agc_start_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_agc_start_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_agc_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_agc_measure_stop_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_agc_get_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_agc_measure_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_tuner_tsout( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_ON;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_tuner_nonstd( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.sync_flg == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_OFF;
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_tuner_sync( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_tuner_async( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.sync_flg == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_OFF;

        dtvd_tuner_core_inevt_ber_measure_cancel_omt();

    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_tuner_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_agc_measure_fst_omt( void )
{
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_agc_measure_fst_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_agc_start_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_AGC_START_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_agc_measure_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_AGC_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_agc_measure_omt( void )
{
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_agc_measure_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );
    }

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_AGC_GET_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_tmcc_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_state_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_cn_start_omt( void )
{
    DTVD_TUNER_INEVT_CORE_CN_START_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_cn_start_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );
    }

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_CNR_START_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_cn_stop_omt( void )
{
    DTVD_TUNER_INEVT_CORE_CN_STOP_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_cn_stop_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );
    }

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_CNR_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_cn_measure_omt( void )
{
    DTVD_TUNER_INEVT_CORE_CN_MEASURE_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_cn_measure_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );
    }

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_CNR_GET_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_ber_start_omt( void )
{
    DTVD_TUNER_INEVT_CORE_BER_START_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_ber_start_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );
    }

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_BER_START_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_ber_stop_omt( void )
{
    DTVD_TUNER_INEVT_CORE_BER_STOP_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_ber_stop_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );
    }

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_BER_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_ber_measure_omt( void )
{
    DTVD_TUNER_INEVT_CORE_BER_MEASURE_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_ber_measure_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );
    }

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_BER_GET_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_011_msg_autoeco_notice( void )
{
    DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_autoeco_notice;

    if ( data->notice == D_DTVD_TUNER_AUTOECO_ECO_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_inevt_tuner_eco_set_param( D_DTVD_TUNER_ON );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_inevt_tuner_eco_set_param( D_DTVD_TUNER_OFF );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_core_mtx_011_msg_autoeco_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

