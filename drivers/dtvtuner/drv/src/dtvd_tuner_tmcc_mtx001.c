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
#include "dtvd_tuner_tmcc.h"

void dtvd_tuner_tmcc_mtx_001_msg_tuner_end( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tmcc_com_set_status( D_DTVD_TUNER_TMCC_000 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tmcc_mtx_001_msg_tuner_infoget( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tmcc_com_get_timer_start();

    dtvd_tuner_tmcc_com_set_status( D_DTVD_TUNER_TMCC_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tmcc_mtx_001_msg_tuner_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tmcc_inevt_tuner_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

