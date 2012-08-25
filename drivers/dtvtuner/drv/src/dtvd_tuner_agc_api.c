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

signed int dtvd_tuner_agc_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_agc.status = D_DTVD_TUNER_AGC_000;

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_agc_get_state( void )
{
    signed int state;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    state = tdtvd_tuner_agc.status;

    DTVD_DEBUG_MSG_EXIT();
    return state;
}

