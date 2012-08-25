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

void dtvd_tuner_tuner_mtx_000_msg_core_pwron( void )
{
    unsigned long flags;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    flags = 0;
    spin_lock_irqsave( &tdtvd_tuner_tuner.lock, flags );

    dtvd_tuner_tuner_com_infoinit();

	
    dtvd_tuner_tuner_com_pwr_on_pega();
    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_PWRON_HALFWAY,
                            D_DTVD_TUNER_TIMER_PWRON1_MSEC );

    spin_unlock_irqrestore( &tdtvd_tuner_tuner.lock, flags );

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

