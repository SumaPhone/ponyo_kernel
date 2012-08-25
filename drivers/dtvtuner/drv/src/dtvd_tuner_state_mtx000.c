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
#include "dtvd_tuner_state.h"

static void dtvd_tuner_state_mtx_000_com_init( void );

void dtvd_tuner_state_mtx_000_msg_tuner_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_state_mtx_000_com_init();

    dtvd_tuner_state_com_set_status( D_DTVD_TUNER_STATE_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_state_mtx_000_com_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_state.cycle_time =
        (unsigned int)( tdtvd_tuner_nonvola.state_monitor_cycle * D_DTVD_TUNER_COEFFICIENT_STATE_TIME );

    tdtvd_tuner_state.state_old = D_DTVD_TUNER_STATE_S0;

    tdtvd_tuner_state.state_new = D_DTVD_TUNER_STATE_S0;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

