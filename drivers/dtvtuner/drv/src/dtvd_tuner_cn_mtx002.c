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

void dtvd_tuner_cn_mtx_002_msg_core_measure( void )
{
    DTVD_TUNER_INEVT_CN_CORE_MEASURE_t *data = &tdtvd_tuner_msg.buff.in_cn_core_measure;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_cn.seq_id = data->seq_id;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_mtx_002_msg_core_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_MEASURE_CN_FST );

    dtvd_tuner_cn_inevt_core_stop();

    dtvd_tuner_cn_com_measure_init();

    dtvd_tuner_cn_com_measure_monitor_init();

    dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_mtx_002_timeout_cn( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_cn_com_read();

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_cn_inevt_core_deverr();

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    tdtvd_tuner_monitor.rx.cna.state = D_DTVD_TUNER_MEASURE_STATE_ON;

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_MEASURE_CN,
                            tdtvd_tuner_cn.measure_time );

    dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_003 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

