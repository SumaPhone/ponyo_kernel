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

void dtvd_tuner_core_mtx_010_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_008 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_tune_dtv_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo_omt();

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_012 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_agc_start_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_agc_start_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_agc_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_agc_measure_stop_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_agc_get_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_agc_measure_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_tuner_tune_set( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_tuner_tune_set;

    if( data->result == D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        if( tdtvd_tuner_core.omt_tune_kind == D_DTVD_TUNER_DTVTUNE_SYNC_OFF )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

            dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
        }

        dtvd_tuner_core_inevt_agc_fst_measure_omt();
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

            dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

            dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_tuner_tsout( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.omt_tune_kind == D_DTVD_TUNER_DTVTUNE_SYNC_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_ON;

        if( tdtvd_tuner_core.tune_cause == D_DTVD_TUNER_TUNE_NG_NONE )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

            dtvd_tuner_core_set_tune_result( D_DTVD_TUNER_OK );

        dtvd_tuner_core_inevt_autoeco_start_omt();

            dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
        }

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_011 );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_tuner_tune_ng( void )
{
    unsigned char cause;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    cause = tdtvd_tuner_msg.buff.in_core_tuner_tune_ng.cause;

    if( tdtvd_tuner_core.omt_tune_kind == D_DTVD_TUNER_DTVTUNE_SYNC_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        if( tdtvd_tuner_core.tune_cause == D_DTVD_TUNER_TUNE_NG_NONE )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

            dtvd_tuner_core_set_tune_result( D_DTVD_TUNER_NG );

            dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
        }

        dtvd_tuner_core_set_ng_cause( cause );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_tuner_nonstd( void )
{
    unsigned char cause;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    cause = tdtvd_tuner_msg.buff.in_core_tuner_nonstd.kind;

    if( tdtvd_tuner_core.omt_tune_kind == D_DTVD_TUNER_DTVTUNE_SYNC_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        if( tdtvd_tuner_core.tune_cause == D_DTVD_TUNER_TUNE_NG_NONE )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

            dtvd_tuner_core_set_tune_result( D_DTVD_TUNER_NG );

            dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
        }

        dtvd_tuner_core_set_ng_cause( cause );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_tuner_sync( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_tuner_async( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.omt_tune_kind == D_DTVD_TUNER_DTVTUNE_SYNC_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        if( tdtvd_tuner_core.tune_cause == D_DTVD_TUNER_TUNE_NG_NONE )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

            dtvd_tuner_core_set_tune_result( D_DTVD_TUNER_NG );

            dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
        }

        dtvd_tuner_core_set_ng_cause( D_DTVD_TUNER_TUNE_NG_CAUSE7 );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_tuner_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.omt_tune_kind == D_DTVD_TUNER_DTVTUNE_SYNC_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        if( tdtvd_tuner_core.tune_cause == D_DTVD_TUNER_TUNE_NG_NONE )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

            dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
        }
    }

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_tuner_deverr_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_agc_measure_fst_omt( void )
{
    signed int ret;
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_agc_measure_fst_omt;

    if( data->result != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        if( tdtvd_tuner_core.omt_tune_kind == D_DTVD_TUNER_DTVTUNE_SYNC_ON )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            if( tdtvd_tuner_core.tune_cause == D_DTVD_TUNER_TUNE_NG_NONE )
            {
                DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

                dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

                dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
            }
        }

        dtvd_tuner_core_inevt_ber_stop_omt();

        dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

        if( tdtvd_tuner_core.omt_tune_kind == D_DTVD_TUNER_DTVTUNE_SYNC_ON )
        {
            DTVD_DEBUG_MSG_ENTER( 5, 0, 0 );

            ret = dtvd_tuner_core_jdg_agc( D_DTVD_TUNER_TUNE_KIND_NORMAL,
                                           data->x_val,
                                           data->y_val );
            if( ret != D_DTVD_TUNER_OK )
            {
                DTVD_DEBUG_MSG_ENTER( 6, 0, 0 );

                if( tdtvd_tuner_core.tune_cause == D_DTVD_TUNER_TUNE_NG_NONE )
                {
                    DTVD_DEBUG_MSG_ENTER( 7, 0, 0 );

                    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

                    dtvd_tuner_core_set_tune_result( D_DTVD_TUNER_NG );

                    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
                }

                dtvd_tuner_core_set_ng_cause( D_DTVD_TUNER_TUNE_NG_CAUSE1 );
            }
        }
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_agc_start_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_AGC_START_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_agc_measure_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_result( D_DTVD_TUNER_OK );

    dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_AGC_STOP_OMT );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_agc_measure_omt( void )
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

void dtvd_tuner_core_mtx_010_msg_tmcc_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.tune_cause == D_DTVD_TUNER_TUNE_NG_NONE )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
    }

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_010_msg_state_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.tune_cause == D_DTVD_TUNER_TUNE_NG_NONE )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_result( D_DTVD_TUNER_NG );

        dtvd_tuner_core_wakeup( D_DTVD_TUNER_WAKEUP_TUNE_OMT );
    }

    dtvd_tuner_core_inevt_ber_stop_omt();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_013 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

