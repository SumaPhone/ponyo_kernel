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
#include "dtvd_tuner_autoeco.h"

void dtvd_tuner_autoeco_inevt_core_notice
(
    unsigned char notice       
)
{
    DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.notice = notice;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_AUTOECO,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_autoeco_inevt_core_deverr( void )
{
    DTVD_TUNER_INEVT_CORE_AUTOECO_DEVERR_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AUTOECO_DEVERR_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AUTOECO_DEVERR,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_AUTOECO,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

