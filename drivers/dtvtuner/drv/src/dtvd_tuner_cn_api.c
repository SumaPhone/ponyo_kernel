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

signed int dtvd_tuner_cn_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_cn.status = D_DTVD_TUNER_CN_000;
    tdtvd_tuner_cn.seq_id = 0;

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_cn_get_state( void )
{
    signed int state;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    state = tdtvd_tuner_cn.status;

    DTVD_DEBUG_MSG_EXIT();
    return state;
}

void dtvd_tuner_cn_measure_non( void )
{

    tdtvd_tuner_monitor.rx.cna.value_x.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;
    tdtvd_tuner_monitor.rx.cna.value_y.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;
    tdtvd_tuner_monitor.rx.cna.value_comp.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;

    return;
}

