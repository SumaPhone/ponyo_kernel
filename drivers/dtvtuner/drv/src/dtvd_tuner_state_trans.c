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
#include "dtvd_tuner_state.h"

void dtvd_tuner_state_trans_msg_mode_ok( void )
{
    unsigned int            len;
    DTVD_TUNER_MSG_u        body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof(DTVD_TUNER_EXEVT_TUNERDD_STATE_MODE_OK_t);
    dtvd_tuner_memset( &body, 0x00,
                       len, sizeof(DTVD_TUNER_MSG_u) );

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_TUNERDD,
                         D_DTVD_TUNER_CMD_TUNERDD_STATE_MODE_OK,
                         (void*)&body,
                         len );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_state_trans_msg_sync( void )
{
    unsigned int            len;
    DTVD_TUNER_MSG_u        body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof(DTVD_TUNER_EXEVT_TUNERDD_STATE_SYNC_t);
    dtvd_tuner_memset( &body, 0x00,
                       len, sizeof(DTVD_TUNER_MSG_u) );

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_TUNERDD,
                         D_DTVD_TUNER_CMD_TUNERDD_STATE_SYNC,
                         (void*)&body,
                         len );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_state_trans_msg_tmcc( void )
{
    unsigned int            len;
    DTVD_TUNER_MSG_u        body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof(DTVD_TUNER_EXEVT_TUNERDD_STATE_TMCC_OK_t);
    dtvd_tuner_memset( &body, 0x00,
                       len, sizeof(DTVD_TUNER_MSG_u) );

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_TUNERDD,
                         D_DTVD_TUNER_CMD_TUNERDD_STATE_TMCC_OK,
                         (void*)&body,
                         len );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_state_trans_msg_tsout( void )
{
    unsigned int            len;
    DTVD_TUNER_MSG_u        body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof(DTVD_TUNER_EXEVT_TUNERDD_STATE_TS_START_t);
    dtvd_tuner_memset( &body, 0x00,
                       len, sizeof(DTVD_TUNER_MSG_u) );

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_TUNERDD,
                         D_DTVD_TUNER_CMD_TUNERDD_STATE_TS_START,
                         (void*)&body,
                         len );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_state_trans_msg_async( void )
{
    unsigned int            len;
    DTVD_TUNER_MSG_u        body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof(DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC_t);
    dtvd_tuner_memset( &body, 0x00,
                       len, sizeof(DTVD_TUNER_MSG_u) );

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_TUNERDD,
                         D_DTVD_TUNER_CMD_TUNERDD_STATE_ASYNC,
                         (void*)&body,
                         len );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_state_inevt_tuner_confirm
(
    signed int  result      
)
{
    DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM_t  data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.result = result;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_WATCH_STATE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_state_inevt_core_deverr( void )
{
    DTVD_TUNER_INEVT_CORE_STATE_DEVERR_t data;
    unsigned int length = sizeof( DTVD_TUNER_INEVT_CORE_STATE_DEVERR_t );

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CORE_STATE_DEVERR,
                                    D_DTVD_TUNER_CTL_CORE,
                                    D_DTVD_TUNER_CTL_WATCH_STATE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

