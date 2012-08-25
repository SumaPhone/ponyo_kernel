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
#include "dtvd_tuner_ant.h"

void dtvd_tuner_ant_mtx_001_msg_core_end( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_ant.ch = 0;

    dtvd_tuner_ant_inevt_core_end();

    dtvd_tuner_ant_com_set_status( D_DTVD_TUNER_ANT_000 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ant_mtx_001_msg_core_set( void )
{
    DTVD_TUNER_INEVT_ANT_CORE_SET_t *data = &tdtvd_tuner_msg.buff.in_ant_core_set;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_ant.ch = data->ch;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ant_mtx_001_msg_core_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ant_mtx_001_msg_core_init_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

