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
#include "dtvd_tuner_ber.h"

void dtvd_tuner_ber_inevt_core_init( void )
{
    DTVD_TUNER_INEVT_CORE_BER_INIT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_BER_INIT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_BER_INIT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_inevt_core_end( void )
{
    DTVD_TUNER_INEVT_CORE_BER_END_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_BER_END_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_BER_END,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_inevt_core_stop( void )
{
    DTVD_TUNER_INEVT_CORE_BER_STOP_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_BER_STOP_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_BER_STOP,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_inevt_core_deverr( void )
{
    DTVD_TUNER_INEVT_CORE_BER_DEVERR_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_BER_DEVERR_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_BER_DEVERR,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_inevt_core_init_omt( void )
{
    DTVD_TUNER_INEVT_CORE_BER_INIT_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_BER_INIT_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_BER_INIT_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_inevt_core_start_omt
(
    signed int result       
)
{
    DTVD_TUNER_INEVT_CORE_BER_START_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_BER_START_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_BER_START_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_inevt_core_stop_omt
(
    signed int result       
)
{
    DTVD_TUNER_INEVT_CORE_BER_STOP_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_BER_STOP_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_BER_STOP_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_ber_inevt_core_measure_omt
(
    signed int result       
)
{
    DTVD_TUNER_INEVT_CORE_BER_MEASURE_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_BER_MEASURE_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_BER_MEASURE_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

