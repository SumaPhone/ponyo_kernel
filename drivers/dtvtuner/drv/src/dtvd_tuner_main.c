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
#include "dtvd_tuner_main.h"
#include "dtvd_tuner_core.h"
#include "dtvd_tuner_ant.h"
#include "dtvd_tuner_tuner.h"
#include "dtvd_tuner_agc.h"
#include "dtvd_tuner_tmcc.h"
#include "dtvd_tuner_cn.h"
#include "dtvd_tuner_ber.h"
#include "dtvd_tuner_state.h"
#include "dtvd_tuner_autoeco.h"
#include "dtvd_tuner_driver.h"

static void dtvd_tuner_main_init( void );
static void dtvd_tuner_main_exevt_rcv( void );
static void dtvd_tuner_main_inevt_rcv( void );
static signed int dtvd_tuner_main_exevt_analyze( void );
static signed int dtvd_tuner_main_inevt_analyze( void );
static DTVD_TUNER_FUNC_t dtvd_tuner_main_matrix( signed int );
static DTVD_TUNER_FUNC_t dtvd_tuner_main_get_matrix( const DTVD_TUNER_MTX_t*, signed int );
static void dtvd_tuner_main_nop_event( void );

extern void dtvd_tuner_timer_pwron_halfway( void );

int dtvd_tuner_main
(
    void* param                         
)
{
    signed int              evtcode;
    DTVD_TUNER_FUNC_t       procp;
    DTVD_TUNER_CONTROL_t    *info = &tdtvd_tuner_info;

    dtvd_tuner_main_init();

    for(;;)
    {

        if( atomic_read( &info->event_flg ) == 0 )
        {

            wait_event_interruptible( tdtvd_tuner_info.wait_queue,
                                      atomic_read(&info->event_flg) );

        }

        if( atomic_read( &info->event_flg ) == 0 )
        {
            DTVD_DEBUG_MSG_INFO("missing signal\n");
            continue;
        }

        dtvd_tuner_main_exevt_rcv();

        DTVD_DEBUG_MSG_INFO("dtvd recv event from_id(%d) command(%d)\n", tdtvd_tuner_msg.fromid, tdtvd_tuner_msg.command);

        if( (tdtvd_tuner_msg.fromid  == D_DTVD_TUNER_BLK_TIMER) && 
            (tdtvd_tuner_msg.command == D_DTVD_TUNER_CMD_TIMER_PWR_HALFTIMEOUT) )
        {

            dtvd_tuner_timer_pwron_halfway();
            continue;
        }

        evtcode = dtvd_tuner_main_exevt_analyze();

        do
        {
            DTVD_DEBUG_MSG_INFO( "evtcode=%d\n", evtcode );

            procp = dtvd_tuner_main_matrix( evtcode );
            if( procp != D_DTVD_TUNER_NULL )
            {

                (*procp)();
            }
            else
            {

                DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_STATE,
                                          DTVD_TUNER_MAIN,
                                          (unsigned long)evtcode,
                                          ((unsigned long)tdtvd_tuner_core.status << 16 )  | (unsigned long)tdtvd_tuner_ant.status ,
                                          ((unsigned long)tdtvd_tuner_tuner.status << 16 ) | (unsigned long)tdtvd_tuner_agc.status ,
                                          ((unsigned long)tdtvd_tuner_tmcc.status << 16 )  | (unsigned long)tdtvd_tuner_cn.status ,
                                          ((unsigned long)tdtvd_tuner_ber.status << 16 )   | (unsigned long)tdtvd_tuner_state.status ,

                                          (unsigned long)tdtvd_tuner_autoeco.status << 16 );
	      printk("******procp == D_DTVD_TUNER_NULL\n");
                dtvd_tuner_core_wakeup(0xFFFFFFFF);
            }

            dtvd_tuner_main_inevt_rcv();

            evtcode = dtvd_tuner_main_inevt_analyze();
        }
        while( evtcode != D_DTVD_TUNER_NOEVENT );
    }

    return 0;
}

static void dtvd_tuner_main_init( void )
{
    DTVD_TUNER_CONTROL_t *info = &tdtvd_tuner_info;

    daemonize( "dtvtuner");

    atomic_set( &info->event_flg, 0 );

    info->block_flg = 0x00;

    init_waitqueue_head( &info->wait_queue );
    init_waitqueue_head( &info->wait_queue_block );

    dtvd_tuner_msg_init( ( D_DTVD_TUNER_EXEVT_CUE_NUM + 1 ),
                         sizeof( DTVD_TUNER_EX_MSG_DATA_t ) );

    dtvd_tuner_memset( &tdtvd_tuner_monitor,
                       0x00,
                       sizeof( DTVD_TUNER_MONITOR_INFO_t ),
                       sizeof( DTVD_TUNER_MONITOR_INFO_t ) );

    dtvd_tuner_memset( &tdtvd_tuner_main,
                       0x00,
                       sizeof( DTVD_TUNER_MAIN_t ),
                       sizeof( DTVD_TUNER_MAIN_t ) );

    dtvd_tuner_timer_init();

    dtvd_tuner_core_init();
    dtvd_tuner_ant_init();
    dtvd_tuner_tuner_init();
    dtvd_tuner_agc_init();
    dtvd_tuner_tmcc_init();
    dtvd_tuner_cn_init();
    dtvd_tuner_ber_init();
    dtvd_tuner_state_init();
    dtvd_tuner_autoeco_init();

    dtvd_tuner_tsprcv_init();

    return;
}

static void dtvd_tuner_main_exevt_rcv( void )
{
    DTVD_TUNER_EX_MSG_DATA_t exevt;
    DTVD_TUNER_RCVMSG_t* msgp = &tdtvd_tuner_msg;

    dtvd_tuner_memset( &exevt, 0x00,
                       sizeof(DTVD_TUNER_EX_MSG_DATA_t),
                       sizeof(DTVD_TUNER_EX_MSG_DATA_t) );

    msgp->available = D_DTVD_TUNER_FALSE;

    dtvd_tuner_msg_rcv( &exevt );

    dtvd_tuner_memset( msgp, 0x00,
                       sizeof(DTVD_TUNER_RCVMSG_t),
                       sizeof(DTVD_TUNER_RCVMSG_t) );
    msgp->fromid = exevt.fromid;
    msgp->command = exevt.command;
    dtvd_tuner_memmove( &msgp->buff, &exevt.buf,
                        sizeof(DTVD_TUNER_MSG_u),
                        sizeof(DTVD_TUNER_MSG_u) );
    return;
}

static void dtvd_tuner_main_inevt_rcv( void )
{
    DTVD_TUNER_RCVMSG_t* msgp = &tdtvd_tuner_msg;

    if( tdtvd_tuner_main.inevt[0].available != D_DTVD_TUNER_FALSE )
    {

        dtvd_tuner_memset( msgp, 0x00,
                           sizeof(DTVD_TUNER_RCVMSG_t),
                           sizeof(DTVD_TUNER_RCVMSG_t) );
        msgp->available = D_DTVD_TUNER_TRUE;
        msgp->toid      = tdtvd_tuner_main.inevt[0].toid;
        msgp->fromid    = tdtvd_tuner_main.inevt[0].fromid;
        msgp->command   = tdtvd_tuner_main.inevt[0].command;
        dtvd_tuner_memmove( &msgp->buff,
                            &tdtvd_tuner_main.inevt[0].buff,
                            sizeof(DTVD_TUNER_MSG_u),
                            sizeof(DTVD_TUNER_MSG_u) );

        dtvd_tuner_memmove( &tdtvd_tuner_main.inevt[0],
                            &tdtvd_tuner_main.inevt[1],
                            sizeof(DTVD_TUNER_RCVMSG_t[D_DTVD_TUNER_INEVT_CUE_NUM-1]),
                            sizeof(DTVD_TUNER_RCVMSG_t[D_DTVD_TUNER_INEVT_CUE_NUM-1]) );
        tdtvd_tuner_main.inevt[D_DTVD_TUNER_INEVT_CUE_NUM - 1].available = D_DTVD_TUNER_FALSE;
    }
    else
    {
        msgp->available = D_DTVD_TUNER_FALSE;
    }
    return;
}

static signed int dtvd_tuner_main_exevt_analyze( void )
{
    const DTVD_TUNER_EVENT_TBL_t* blkp;
    const DTVD_TUNER_EVENT_ANLZ_TBL_t* cmdp;
    signed int evtcode;

    blkp = dtvd_tuner_main_anlz_get_table();
    if( blkp == D_DTVD_TUNER_NULL )
    {
        return D_DTVD_TUNER_NOEVENT;
    }
    cmdp = D_DTVD_TUNER_NULL;
    while( blkp->blkid != D_DTVD_TUNER_BLK_UNDEF )
    {
        if( blkp->blkid == tdtvd_tuner_msg.fromid )
        {
            cmdp = blkp->analize;
            break;
        }
        blkp++;
    }

    evtcode = D_DTVD_TUNER_NOEVENT;
    if( cmdp != D_DTVD_TUNER_NULL )
    {
        while( cmdp->func != D_DTVD_TUNER_NULL )
        {
            if( cmdp->command == (unsigned int)tdtvd_tuner_msg.command )
            {
                evtcode = (*cmdp->func)();
                break;
            }
            cmdp++;
        }
    }

    return evtcode;
}

static signed int dtvd_tuner_main_inevt_analyze( void )
{
    signed int evtcode;

    if( tdtvd_tuner_msg.available != D_DTVD_TUNER_FALSE )
    {
        evtcode = tdtvd_tuner_msg.command;
    }
    else
    {
        evtcode = D_DTVD_TUNER_NOEVENT;
    }
    return evtcode;
}

static DTVD_TUNER_FUNC_t dtvd_tuner_main_matrix
(
    signed int evtcode                  
)
{
    const DTVD_TUNER_MTX_TBL_t* mtxp;
    DTVD_TUNER_FUNC_t procp = D_DTVD_TUNER_NULL;
    signed int state;

    if( evtcode == D_DTVD_TUNER_EVENT_NOP )
    {
        procp = (DTVD_TUNER_FUNC_t)dtvd_tuner_main_nop_event;
        return procp;
    }

    switch( tdtvd_tuner_msg.toid )
    {
        case D_DTVD_TUNER_CTL_CORE:         
            state = dtvd_tuner_core_get_state();
            break;
        case D_DTVD_TUNER_CTL_ANT:          
            state = dtvd_tuner_ant_get_state();
            break;
        case D_DTVD_TUNER_CTL_TUNER:        
            state = dtvd_tuner_tuner_get_state();
            break;
        case D_DTVD_TUNER_CTL_WATCH_AGC:    
            state = dtvd_tuner_agc_get_state();
            break;
        case D_DTVD_TUNER_CTL_WATCH_TMCC:   
            state = dtvd_tuner_tmcc_get_state();
            break;
        case D_DTVD_TUNER_CTL_WATCH_CN:     
            state = dtvd_tuner_cn_get_state();
            break;
        case D_DTVD_TUNER_CTL_WATCH_BER:    
            state = dtvd_tuner_ber_get_state();
            break;
        case D_DTVD_TUNER_CTL_WATCH_STATE:  
            state = dtvd_tuner_state_get_state();
            break;

        case D_DTVD_TUNER_CTL_AUTOECO:      
            state = dtvd_tuner_autoeco_get_state();
            break;

        default:
            return D_DTVD_TUNER_NULL;
    }

    mtxp = dtvd_tuner_main_mtx_get_table();
    if( mtxp == D_DTVD_TUNER_NULL )
    {
        return D_DTVD_TUNER_NULL;
    }
    while( mtxp->ctlid != D_DTVD_TUNER_CTL_UNDEF )
    {
        if( mtxp->ctlid == tdtvd_tuner_msg.toid )
        {
            procp = dtvd_tuner_main_get_matrix( mtxp->mtxp[state], evtcode );
            break;
        }
        mtxp++;
    }

    return procp;
}

static DTVD_TUNER_FUNC_t dtvd_tuner_main_get_matrix
(
    const DTVD_TUNER_MTX_t* mtxp,       
    signed int evtcode                  
)
{
    DTVD_TUNER_FUNC_t procp = D_DTVD_TUNER_NULL;

    if( mtxp == D_DTVD_TUNER_NULL )
    {
        return procp;
    }

    while( mtxp->evtcode != D_DTVD_TUNER_NOEVENT )
    {
        if( mtxp->evtcode == evtcode )
        {
            if( mtxp->funcp != D_DTVD_TUNER_NULL )
            {
                procp = mtxp->funcp;
            }
            else
            {
                procp = (DTVD_TUNER_FUNC_t)dtvd_tuner_main_nop_event;
            }
            break;
        }
        mtxp++;
    }

    return procp;
}

void dtvd_tuner_main_inevt_send_msg
(
    signed int evtcode,                 
    unsigned short toid,                
    unsigned short fromid,              
    void* body,                         
    unsigned int length                 
)
{
    DTVD_TUNER_RCVMSG_t* send = tdtvd_tuner_main.inevt;
    signed int i;

    if( body == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_MAIN,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    for( i = 0; i < D_DTVD_TUNER_INEVT_CUE_NUM; i++ )
    {

        if( send->available == D_DTVD_TUNER_FALSE )
        {
            break;
        }
        send++;
    }
    if( i == D_DTVD_TUNER_INEVT_CUE_NUM )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_MAIN,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    dtvd_tuner_memset( send, 0x00,
                       sizeof(DTVD_TUNER_RCVMSG_t),
                       sizeof(DTVD_TUNER_RCVMSG_t) );
    send->available = D_DTVD_TUNER_TRUE;
    send->toid      = toid;
    send->fromid    = fromid;
    send->command   = evtcode;
    dtvd_tuner_memmove( &send->buff, body, length, sizeof(DTVD_TUNER_MSG_u) );

    return;
}

static void dtvd_tuner_main_nop_event( void )
{

    DTVD_DEBUG_MSG_INFO("dtvd_tuner_main_nop_event\n");
    return;
}

