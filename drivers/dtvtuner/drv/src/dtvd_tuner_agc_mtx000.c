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
#include "dtvd_tuner_agc.h"

void dtvd_tuner_agc_mtx_000_msg_core_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_agc_com_get_timer_value();

    dtvd_tuner_agc_com_set_status( D_DTVD_TUNER_AGC_001 );

    dtvd_tuner_agc_inevt_core_init();

    DTVD_DEBUG_MSG_EXIT();

    return;
}

void dtvd_tuner_agc_mtx_000_msg_core_init_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_agc_com_get_timer_value();

    dtvd_tuner_agc_com_set_status( D_DTVD_TUNER_AGC_001 );

    dtvd_tuner_agc_inevt_core_init_omt();

    DTVD_DEBUG_MSG_EXIT();

    return;
}

