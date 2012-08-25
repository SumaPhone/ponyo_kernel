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

void dtvd_tuner_tmcc_mtx_000_msg_tuner_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tmcc_com_init();

    dtvd_tuner_tmcc_com_set_status( D_DTVD_TUNER_TMCC_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

