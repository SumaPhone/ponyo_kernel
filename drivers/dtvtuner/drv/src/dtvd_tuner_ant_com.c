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

void dtvd_tuner_ant_com_set_status
(
    signed int next_state                               
)
{
    DTVD_DEBUG_MSG_ENTER( next_state, 0, 0 );

    tdtvd_tuner_ant.status = next_state;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

