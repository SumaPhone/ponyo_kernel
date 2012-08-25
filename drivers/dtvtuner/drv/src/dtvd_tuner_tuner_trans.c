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

void dtvd_tuner_tuner_inevt_core_pwron( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_PWRON_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_PWRON_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_PWRON,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_pwroff( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_PWROFF_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_PWROFF_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_PWROFF,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_init( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_INIT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_INIT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_INIT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_end( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_END_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_END_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_END,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_tune_set( unsigned int result )
{
    DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_tmcc( void )
{
    DTVD_TUNER_INEVT_TMCC_TUNER_INFOGET_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TMCC_TUNER_INFOGET_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_TMCC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_tsout( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_TMCC_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_TMCC_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_TSOUT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_tune_ng
(
    unsigned char cause     
)
{
    DTVD_TUNER_INEVT_CORE_TUNER_TUNE_NG_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_TUNE_NG_t);
    
    printk("********ng**cause=%d**%s\n", cause, __func__);
    DTVD_DEBUG_MSG_ENTER( cause, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    data.cause = cause;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_TUNE_NG,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_nostd
(
    unsigned char kind      
)
{
    DTVD_TUNER_INEVT_CORE_TUNER_NONSTD_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_NONSTD_t);

    DTVD_DEBUG_MSG_ENTER( kind, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    data.kind = kind;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_NONSTD,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_sync( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_SYNC_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_SYNC_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_SYNC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_async( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_ASYNC_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_ASYNC_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_ASYNC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_stop( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_STOP_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_STOP_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_STOP,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_deverr( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_tmcc_tuner_init( void )
{
    DTVD_TUNER_INEVT_TMCC_TUNER_INIT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TMCC_TUNER_INIT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TMCC_TUNER_INIT,
                                    D_DTVD_TUNER_CTL_WATCH_TMCC,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_tmcc_tuner_end( void )
{
    DTVD_TUNER_INEVT_TMCC_TUNER_END_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TMCC_TUNER_END_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TMCC_TUNER_END,
                                    D_DTVD_TUNER_CTL_WATCH_TMCC,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_tmcc_infoget( void )
{
    DTVD_TUNER_INEVT_TMCC_TUNER_INFOGET_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TMCC_TUNER_INFOGET_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TMCC_TUNER_INFOGET,
                                    D_DTVD_TUNER_CTL_WATCH_TMCC,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_tmcc_stop( void )
{
    DTVD_TUNER_INEVT_TMCC_TUNER_STOP_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TMCC_TUNER_STOP_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TMCC_TUNER_STOP,
                                    D_DTVD_TUNER_CTL_WATCH_TMCC,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_init_omt( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_INIT_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_INIT_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_INIT_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_stop_omt( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_STOP_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_STOP_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_STOP_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_core_deverr_omt( void )
{
    DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_state_tuner_init( void )
{
    DTVD_TUNER_INEVT_STATE_TUNER_INIT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_STATE_TUNER_INIT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_STATE_TUNER_INIT,
                                    D_DTVD_TUNER_CTL_WATCH_STATE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_state_tuner_end( void )
{
    DTVD_TUNER_INEVT_STATE_TUNER_END_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_STATE_TUNER_END_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_STATE_TUNER_END,
                                    D_DTVD_TUNER_CTL_WATCH_STATE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_inevt_state_tuner_start( void )
{
    DTVD_TUNER_INEVT_STATE_TUNER_START_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_STATE_TUNER_START_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_STATE_TUNER_START,
                                    D_DTVD_TUNER_CTL_WATCH_STATE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_tuner_inevt_state_tuner_stop( void )
{
    DTVD_TUNER_INEVT_STATE_TUNER_STOP_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_STATE_TUNER_STOP_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_STATE_TUNER_STOP,
                                    D_DTVD_TUNER_CTL_WATCH_STATE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_tuner_inevt_state_tuner_confirm( unsigned char kind )
{
    DTVD_TUNER_INEVT_STATE_TUNER_CONFIRM_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_STATE_TUNER_CONFIRM_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.kind = kind;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_STATE_TUNER_CONFIRM,
                                    D_DTVD_TUNER_CTL_WATCH_STATE,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    &data, length );

    DTVD_DEBUG_MSG_EXIT();
    return;

}

