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

void dtvd_tuner_cn_mtx_003_msg_core_measure( void )
{
    DTVD_TUNER_INEVT_CN_CORE_MEASURE_t *data = &tdtvd_tuner_msg.buff.in_cn_core_measure;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_cn.seq_id = data->seq_id;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_mtx_003_msg_core_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_MEASURE_CN );

    dtvd_tuner_cn_inevt_core_stop();

    dtvd_tuner_cn_com_measure_init();

    dtvd_tuner_cn_com_measure_monitor_init();

    dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_mtx_003_timeout_cn_second( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_cn.stabilize_times++;

    ret = dtvd_tuner_cn_com_read();

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_cn_inevt_core_deverr();

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_cn_com_average();

    if( tdtvd_tuner_nonvola.cn_ave_count > (unsigned short)tdtvd_tuner_cn.stabilize_times )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_MEASURE_CN,
                                tdtvd_tuner_cn.measure_time );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        dtvd_tuner_cn_com_average_set();

        dtvd_tuner_cn_com_watch_timer_start();

        dtvd_tuner_cn_inevt_core_measure();

        dtvd_tuner_cn_inevt_autoeco_notice( tdtvd_tuner_cn.cn_old_ave );

        dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_004 );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

