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

void dtvd_tuner_autoeco_mtx_000_msg_core_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_autoeco_com_init();

    tdtvd_tuner_autoeco.autoeco_flg = D_DTVD_TUNER_OFF;

    dtvd_tuner_autoeco_com_set_status( D_DTVD_TUNER_AUTOECO_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

