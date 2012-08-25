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

static void dtvd_tuner_state_mtx_002_com_jdg( unsigned char, signed int* );

void dtvd_tuner_state_mtx_002_msg_tuner_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_state_com_watch_timer_stop();

    dtvd_tuner_state_com_state_init();

    dtvd_tuner_state_com_set_status( D_DTVD_TUNER_STATE_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_state_mtx_002_msg_tuner_confirm( void )
{
    signed int      ret;

    signed int      result = D_DTVD_TUNER_NG;

    unsigned char   tbl_kind;
    DTVD_TUNER_INEVT_STATE_TUNER_CONFIRM_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );
    data = &tdtvd_tuner_msg.buff.in_state_tuner_confirm;
    ret = dtvd_tuner_state_com_read();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_state_com_watch_timer_stop();

        dtvd_tuner_state_inevt_core_deverr();

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_state_mtx_002_com_jdg( data->kind, &result );
    switch( data->kind )
    {
        case D_DTVD_TUNER_STATE_CONFIRM_SYNC:
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            tbl_kind = D_DTVD_TUNER_STATE_TBL_SYNC;
            break;
        case D_DTVD_TUNER_STATE_CONFIRM_TSOUT:
            DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

            tbl_kind = D_DTVD_TUNER_STATE_TBL_TSOUT;
            break;
        default:
            DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

            tbl_kind = D_DTVD_TUNER_STATE_TBL_SYNC;

            DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                      DTVD_TUNER_STATE_MTX002,
                                      (unsigned long)tbl_kind, 0, 0, 0, 0, 0 );
            break;
    }

    dtvd_tuner_state_com_action( tbl_kind );

    dtvd_tuner_state_inevt_tuner_confirm( result );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_state_mtx_002_timeout_state( void )
{
    signed int      ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_state_com_read();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_state_inevt_core_deverr();

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_state_com_action( D_DTVD_TUNER_STATE_TBL_WATCH );

    dtvd_tuner_state_com_watch_timer_start();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_state_mtx_002_com_jdg
(
    unsigned char   kind,   
    signed int*     result  
)
{

    static const signed int jdg_tbl[D_DTVD_TUNER_STATE_CONFIRM_MAX][D_DTVD_TUNER_STATE_S_MAX] =
    {

        {
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_OK,    
            D_DTVD_TUNER_OK     
        },

        {
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_NG,    
            D_DTVD_TUNER_OK     
        }
    };

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( result == D_DTVD_TUNER_NULL )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_STATE_MTX002,
                                  0, 0, 0, 0, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }
    if( kind >= D_DTVD_TUNER_STATE_CONFIRM_MAX )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_STATE_MTX002,
                                  (unsigned long)kind, 0, 0, 0, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    if( tdtvd_tuner_state.state_new >= D_DTVD_TUNER_STATE_S_MAX )
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_STATE_MTX002,
                                  (unsigned long)tdtvd_tuner_state.state_new, 0, 0, 0, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    *result = jdg_tbl[kind][tdtvd_tuner_state.state_new];

    DTVD_DEBUG_MSG_EXIT();
    return;
}

