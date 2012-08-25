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

void dtvd_tuner_core_mtx_012_tunerdd_pre( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_init_tuneflg();

    dtvd_tuner_core_inevt_ant_init_omt();

    dtvd_tuner_core_inevt_tuner_init_omt();

    dtvd_tuner_core_inevt_ant_set();

    dtvd_tuner_core_inevt_tuner_tune_dtv_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_010 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_012_msg_tuner_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_trans_msg_pre_tune();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_012_msg_tuner_deverr_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_012_msg_agc_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_tuner_stop_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_012_msg_cn_stop_omt( void )
{
    DTVD_TUNER_INEVT_CORE_CN_STOP_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_cn_stop_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_inevt_agc_stop_omt();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_012_msg_ber_stop_omt( void )
{
    DTVD_TUNER_INEVT_CORE_BER_STOP_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_ber_stop_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_inevt_autoeco_stop_omt();

        dtvd_tuner_core_inevt_cn_stop_omt();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

