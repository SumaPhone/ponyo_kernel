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
#include "dtvd_tuner_agc.h"

void dtvd_tuner_agc_inevt_core_init( void )
{
    DTVD_TUNER_INEVT_CORE_AGC_INIT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_INIT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_INIT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_end( void )
{
    DTVD_TUNER_INEVT_CORE_AGC_END_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_END_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_END,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_measure_fst( void )
{
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.x_val = tdtvd_tuner_monitor.rx.agc.value_x;         
    data.y_val = tdtvd_tuner_monitor.rx.agc.value_y;         

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_measure( void )
{
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_MEASURE_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.x_val = tdtvd_tuner_monitor.rx.agc.value_x;         
    data.y_val = tdtvd_tuner_monitor.rx.agc.value_y;         

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_stop( void )
{
    DTVD_TUNER_INEVT_CORE_AGC_STOP_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_STOP_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_STOP,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_deverr( void )
{
    DTVD_TUNER_INEVT_CORE_AGC_DEVERR_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_DEVERR_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_DEVERR,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_init_omt( void )
{
    DTVD_TUNER_INEVT_CORE_AGC_INIT_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_INIT_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_INIT_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_measure_fst_omt
(
    signed int  result      
)
{
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.x_val = tdtvd_tuner_monitor.rx.agc.value_x;         
    data.y_val = tdtvd_tuner_monitor.rx.agc.value_y;         
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_start_omt
(
    signed int  result      
)
{
    DTVD_TUNER_INEVT_CORE_AGC_START_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_START_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_START_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_stop_omt
(
    signed int  result      
)
{
    DTVD_TUNER_INEVT_CORE_AGC_STOP_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_STOP_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_STOP_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_measure_omt
(
    signed int  result      
)
{
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_MEASURE_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.x_val = tdtvd_tuner_monitor.rx.agc.value_x;         
    data.y_val = tdtvd_tuner_monitor.rx.agc.value_y;         
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_agc_inevt_core_measure_stop_omt
(
    signed int  result      
)
{
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_STOP_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_AGC_MEASURE_STOP_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_STOP_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

