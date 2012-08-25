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

void dtvd_tuner_tmcc_inevt_tuner_stop( void )
{
    DTVD_TUNER_INEVT_TUNER_TMCC_STOP_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_TUNER_TMCC_STOP_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_TMCC_STOP,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_WATCH_TMCC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tmcc_inevt_tuner_infoget
(
    unsigned char   std     
)
{
    DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET_t );

    DTVD_DEBUG_MSG_ENTER( std, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    data.non_std = std;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_WATCH_TMCC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tmcc_inevt_core_deverr( void )
{
    DTVD_TUNER_INEVT_CORE_TMCC_DEVERR_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_TMCC_DEVERR_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TMCC_DEVERR,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_TMCC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

