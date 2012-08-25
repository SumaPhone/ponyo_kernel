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
#include "dtvd_tuner_cn.h"

void dtvd_tuner_cn_mtx_001_msg_core_end( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_monitor.rx.cna.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_cn_inevt_core_end();

    dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_000 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_mtx_001_msg_core_set( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_cn_mtx_001_msg_core_measure( void )
{
    signed int ret;

    DTVD_TUNER_INEVT_CN_CORE_MEASURE_t *data = &tdtvd_tuner_msg.buff.in_cn_core_measure;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_cn_com_start_set();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_cn_inevt_core_deverr();

        DTVD_DEBUG_MSG_EXIT();

        return;

    }

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_MEASURE_CN_FST,
                            tdtvd_tuner_cn.wait_time );

    tdtvd_tuner_cn.stabilize_times = 0;

    tdtvd_tuner_cn.seq_id = data->seq_id;

    dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_mtx_001_msg_core_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_cn_inevt_core_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_mtx_001_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_monitor.rx.cna.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_cn_inevt_core_stop_omt( D_DTVD_TUNER_OK );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_mtx_001_msg_core_start_omt( void )
{
    signed int ret;
    DTVD_TUNER_INEVT_CN_CORE_START_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_cn_core_start_omt;

    ret = dtvd_tuner_cn_com_symcount_read();

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_cn_inevt_core_start_omt( D_DTVD_TUNER_NG );

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_cn_com_symcount_chg_timer( data->carrier );

    ret = dtvd_tuner_cn_com_symcount_set( data->carrier );

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_cn_inevt_core_start_omt( D_DTVD_TUNER_NG );

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_cn_inevt_core_start_omt( D_DTVD_TUNER_OK );

    dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_005 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

