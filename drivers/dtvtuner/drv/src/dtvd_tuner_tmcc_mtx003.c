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

void dtvd_tuner_tmcc_mtx_003_msg_tuner_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_WATCH_TMCC );

    dtvd_tuner_tmcc_inevt_tuner_stop();

    dtvd_tuner_tmcc_com_set_status( D_DTVD_TUNER_TMCC_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tmcc_mtx_003_timeout_tmcc_watch( void )
{
    signed int      ret;
    unsigned char   std;
    signed int      result;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_tmcc_com_get_tmcc( &result );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tmcc_inevt_core_deverr();
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    if( result == D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        std = dtvd_tuner_tmcc_com_jdg_std();
        if( std == D_DTVD_TUNER_TRUE )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            dtvd_tuner_tmcc_inevt_tuner_infoget( D_DTVD_TUNER_TMCC_INFOGET_STANDARD );
        }
        else
        {
            DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

            dtvd_tuner_tmcc_inevt_tuner_infoget( D_DTVD_TUNER_TMCC_INFOGET_NONSTD );
        }
    }

    dtvd_tuner_tmcc_com_watch_timer_start();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

