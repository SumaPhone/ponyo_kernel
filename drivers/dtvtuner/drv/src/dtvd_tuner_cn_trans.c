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
#include "dtvd_tuner_cn.h"

void dtvd_tuner_cn_inevt_core_init( void )
{
    DTVD_TUNER_INEVT_CORE_CN_INIT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_CN_INIT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_CN_INIT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_inevt_core_end( void )
{
    DTVD_TUNER_INEVT_CORE_CN_END_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_CN_END_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_CN_END,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_inevt_core_measure( void )
{
    DTVD_TUNER_INEVT_CORE_CN_MEASURE_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_CN_MEASURE_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    data.seq_id = tdtvd_tuner_cn.seq_id;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_CN_MEASURE,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_inevt_core_stop( void )
{
    DTVD_TUNER_INEVT_CORE_CN_STOP_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_CN_STOP_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_CN_STOP,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_inevt_core_deverr( void )
{
    DTVD_TUNER_INEVT_CORE_CN_DEVERR_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_CN_DEVERR_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_CN_DEVERR,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_inevt_core_init_omt
(
    signed int result       
)
{
    DTVD_TUNER_INEVT_CORE_CN_STOP_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CN_CORE_INIT_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_CN_INIT_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_inevt_core_start_omt
(
    signed int result       
)
{
    DTVD_TUNER_INEVT_CORE_CN_START_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_CN_START_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_CN_START_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_inevt_core_stop_omt
(
    signed int result       
)
{
    DTVD_TUNER_INEVT_CORE_CN_STOP_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_CN_STOP_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_CN_STOP_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_inevt_core_measure_omt
(
    signed int result       
)
{
    DTVD_TUNER_INEVT_CORE_CN_MEASURE_OMT_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_CN_MEASURE_OMT_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_CN_MEASURE_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_inevt_autoeco_notice
(
    DTVD_TUNER_MEASURE_VALUE_t cn       
)
{
    DTVD_TUNER_INEVT_AUTOECO_CN_NOTICE_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_AUTOECO_CN_NOTICE_t );

    DTVD_DEBUG_MSG_ENTER( cn.up, cn.low, cn.ext );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.cn = cn;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AUTOECO_CN_NOTICE,
                                    D_DTVD_TUNER_CTL_AUTOECO,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

