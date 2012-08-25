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
#include "dtvd_tuner_tuner.h"

void dtvd_tuner_tuner_mtx_010_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_tmcc_stop();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

