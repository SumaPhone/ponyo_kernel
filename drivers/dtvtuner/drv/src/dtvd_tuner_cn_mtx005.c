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

void dtvd_tuner_cn_mtx_005_msg_core_stop_omt( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_cn_com_symcount_set( tdtvd_tuner_cn.symcount );

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_cn_inevt_core_stop_omt( D_DTVD_TUNER_NG );
    }
    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_cn_inevt_core_stop_omt( D_DTVD_TUNER_OK );
    }

    dtvd_tuner_cn_com_measure_time_set();

    dtvd_tuner_cn_com_measure_init();

    dtvd_tuner_cn_com_measure_monitor_init();

    dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_mtx_005_msg_core_measure_omt( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_cn_com_start_set();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_cn_inevt_core_measure_omt( D_DTVD_TUNER_NG );

        dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_001 );

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_MEASURE_CN,
                            tdtvd_tuner_cn.measure_time );

    dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_006 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

