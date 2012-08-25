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

void dtvd_tuner_cn_mtx_000_msg_core_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_monitor.rx.cna.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_cn_com_init();

    dtvd_tuner_cn_com_measure_init();

    dtvd_tuner_cn_com_measure_monitor_init();

    dtvd_tuner_cn_inevt_core_init();

    dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_mtx_000_msg_core_init_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_monitor.rx.cna.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    dtvd_tuner_cn_com_init();

    dtvd_tuner_cn_com_measure_init();

    dtvd_tuner_cn_com_measure_monitor_init();

    dtvd_tuner_cn_inevt_core_init_omt( D_DTVD_TUNER_OK );

    dtvd_tuner_cn_com_set_status( D_DTVD_TUNER_CN_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

