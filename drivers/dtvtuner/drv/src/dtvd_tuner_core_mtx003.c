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

void dtvd_tuner_core_mtx_003_tune_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_007 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_scan_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );
    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_007 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_008 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_tuner_tune_set( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_tuner_tune_set;

    if( data->result == D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

        dtvd_tuner_core_inevt_agc_fst_measure();

        dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

        dtvd_tuner_core_deverr();

        dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE );

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_tuner_tsout( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_agc_measure();

    dtvd_tuner_core_inevt_ber_start();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_tuner_tune_ng( void )
{
    unsigned char cause;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    cause = tdtvd_tuner_msg.buff.in_core_tuner_tune_ng.cause;

    dtvd_tuner_core_trans_msg_tune_ng( cause );

    dtvd_tuner_core_set_ng_cause( cause );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_tuner_nonstd( void )
{
    unsigned char cause;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    cause = tdtvd_tuner_msg.buff.in_core_tuner_nonstd.kind;

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_inevt_cn_stop();

    dtvd_tuner_core_trans_msg_tune_ng( cause );

    dtvd_tuner_core_set_ng_cause( cause );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_tuner_sync( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_tuner_async( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_inevt_cn_stop();

    dtvd_tuner_core_trans_msg_tune_ng( D_DTVD_TUNER_TUNE_NG_CAUSE7 );

    dtvd_tuner_core_set_ng_cause( D_DTVD_TUNER_TUNE_NG_CAUSE7 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_tuner_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_tuner_errcore( void )
{
    unsigned long seq_id;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    seq_id = dtvd_tuner_core_get_cn_seq_id();
    dtvd_tuner_core_inevt_cn_measure( seq_id );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_agc_measure_fst( void )
{
    signed int ret;
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_agc_measure_fst;

    ret = dtvd_tuner_core_jdg_agc( tdtvd_tuner_core.tune_kind,
                                   data->x_val,
                                   data->y_val );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_trans_msg_tune_ng( D_DTVD_TUNER_TUNE_NG_CAUSE1 );

        dtvd_tuner_core_set_ng_cause( D_DTVD_TUNER_TUNE_NG_CAUSE1 );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_agc_measure( void )
{
    unsigned char agc;
    unsigned long seq_id;
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_agc_measure;

    tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_ON;

    if( data->x_val >= data->y_val )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        agc = data->x_val;
    }
    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        agc = data->y_val;
    }

    if( tdtvd_tuner_core.eco_flag == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        agc = data->x_val;
    }

    dtvd_tuner_core_sync_time_end();

    if ( tdtvd_tuner_core.eco_flag == D_DTVD_TUNER_OFF )
    {
        DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

        dtvd_tuner_core_inevt_autoeco_start();
    }

    dtvd_tuner_core_trans_msg_tune_ok( agc );

    seq_id = dtvd_tuner_core_get_cn_seq_id();
    dtvd_tuner_core_inevt_cn_measure( seq_id );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_014 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_agc_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_tmcc_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_cn_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_ber_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_msg_state_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_003_eco_set( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_eco_flag();

    dtvd_tuner_core_inevt_tuner_eco_set();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

