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

void dtvd_tuner_ant_inevt_core_init( void )
{
    DTVD_TUNER_INEVT_CORE_ANT_INIT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_ANT_INIT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_ANT_INIT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_ANT,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ant_inevt_core_end( void )
{
    DTVD_TUNER_INEVT_CORE_ANT_END_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_ANT_END_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_ANT_END,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_ANT,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ant_inevt_core_init_omt( void )
{
    DTVD_TUNER_INEVT_CORE_ANT_INIT_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_ANT_INIT_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_ANT_INIT_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_ANT,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

