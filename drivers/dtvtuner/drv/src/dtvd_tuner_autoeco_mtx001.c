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
#include "dtvd_tuner_autoeco.h"

void dtvd_tuner_autoeco_mtx_001_msg_core_end( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_autoeco_com_set_status( D_DTVD_TUNER_AUTOECO_000 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_autoeco_mtx_001_msg_core_start( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_autoeco_com_set_status( D_DTVD_TUNER_AUTOECO_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_autoeco_mtx_001_msg_core_start_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_WATCH_AUTOECO_OMT,
        tdtvd_tuner_nonvola.cn_cycle * D_DTVD_TUNER_COEFFICIENT_CN_CYCLE_TIME);

    dtvd_tuner_autoeco_com_set_status( D_DTVD_TUNER_AUTOECO_003 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

