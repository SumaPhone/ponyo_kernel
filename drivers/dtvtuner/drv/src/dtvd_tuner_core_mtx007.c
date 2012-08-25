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

void dtvd_tuner_core_mtx_007_tune_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_007 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_007_scan_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_007 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_007_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_008 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_007_tunerdd_pre( void )
{
    DTVD_TUNER_EXEVT_TUNERDD_PRE_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.ex_tunerdd_pre;

    if( data->seq_id != tdtvd_tuner_core.tune_seq_id )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_DEBUG_MSG_INFO( "seq_id unmatch!!\n" );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_core_init_tuneflg();

    dtvd_tuner_core_inevt_ant_init();

    dtvd_tuner_core_inevt_tuner_init();

    dtvd_tuner_core_inevt_ant_set();

    dtvd_tuner_core_inevt_tuner_tune_dtv();

    if( tdtvd_tuner_core.tune_kind == D_DTVD_TUNER_TUNE_KIND_SCAN )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_004 );
    }
    else
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_003 );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_007_msg_tuner_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_trans_msg_pre_tune();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_007_msg_tuner_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );
    dtvd_tuner_core_deverr();

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_007_msg_agc_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_tuner_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_007_msg_cn_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_agc_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_007_msg_ber_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_cn_stop();

    dtvd_tuner_core_inevt_autoeco_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_007_eco_set( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_eco_flag();

    dtvd_tuner_core_inevt_tuner_eco_set();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

