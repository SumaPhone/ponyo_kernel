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

signed int dtvd_tuner_tuner_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &tdtvd_tuner_tuner, 0x00,
                       sizeof(DTVD_TUNER_TUNER_INFO_t),
                       sizeof(DTVD_TUNER_TUNER_INFO_t) );
    tdtvd_tuner_tuner.status = D_DTVD_TUNER_TUNER_000;

    spin_lock_init( &tdtvd_tuner_tuner.lock );

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_tuner_get_state( void )
{
    signed int state;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    state = tdtvd_tuner_tuner.status;

    DTVD_DEBUG_MSG_EXIT();
    return state;
}

