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

void dtvd_tuner_tuner_mtx_009_msg_tmcc_stop( void )
{
    signed   int                ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_tuner_com_tuner_stop();

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr_omt();

    }
    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_tuner_inevt_core_stop_omt();

    }

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_003 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

