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
#include "dtvd_tuner_tuner.h"

static void dtvd_tuner_timer_handler_callfunc( unsigned long, unsigned char * );

void dtvd_tuner_timer_pwron_halfway( void );

static void dtvd_tuner_timer_pwron( void );
static void dtvd_tuner_timer_tasklet( unsigned long );

static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet0, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet1, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet2, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet3, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet4, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet5, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet6, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet7, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet8, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet9, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet10, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet11, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet12, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet13, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet14, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet15, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet16, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet17, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet18, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet19, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet20, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet21, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet22, dtvd_tuner_timer_tasklet, 0 );
static DECLARE_TASKLET( tdtvd_tuner_timer_tasklet23, dtvd_tuner_timer_tasklet, 0 );

static struct tasklet_struct *tdtvd_tuner_timer_tasklet[ D_DTVD_TUNER_TIMER_MAX_NUM ] = {
    &tdtvd_tuner_timer_tasklet0,
    &tdtvd_tuner_timer_tasklet1,
    &tdtvd_tuner_timer_tasklet2,
    &tdtvd_tuner_timer_tasklet3,
    &tdtvd_tuner_timer_tasklet4,
    &tdtvd_tuner_timer_tasklet5,
    &tdtvd_tuner_timer_tasklet6,
    &tdtvd_tuner_timer_tasklet7,
    &tdtvd_tuner_timer_tasklet8,
    &tdtvd_tuner_timer_tasklet9,
    &tdtvd_tuner_timer_tasklet10,
    &tdtvd_tuner_timer_tasklet11,
    &tdtvd_tuner_timer_tasklet12,
    &tdtvd_tuner_timer_tasklet13,
    &tdtvd_tuner_timer_tasklet14,
    &tdtvd_tuner_timer_tasklet15,
    &tdtvd_tuner_timer_tasklet16,
    &tdtvd_tuner_timer_tasklet17,
    &tdtvd_tuner_timer_tasklet18,
    &tdtvd_tuner_timer_tasklet19,
    &tdtvd_tuner_timer_tasklet20,
    &tdtvd_tuner_timer_tasklet21,
    &tdtvd_tuner_timer_tasklet22,
    &tdtvd_tuner_timer_tasklet23
};

#ifndef DTVD_NO_HRTIMER
static enum hrtimer_restart dtvd_tuner_timer_func00(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func01(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func02(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func03(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func04(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func05(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func06(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func07(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func08(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func09(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func10(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func11(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func12(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func13(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func14(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func15(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func16(struct hrtimer *timer);
static enum hrtimer_restart dtvd_tuner_timer_func17(struct hrtimer *timer);

static enum hrtimer_restart		(*tdtvd_tuner_timer_timer_function[ D_DTVD_TUNER_TIMER_MAX ])(struct hrtimer *) = {
    dtvd_tuner_timer_func00,
    dtvd_tuner_timer_func01,
    dtvd_tuner_timer_func02,
    dtvd_tuner_timer_func03,
    dtvd_tuner_timer_func04,
    dtvd_tuner_timer_func05,
    dtvd_tuner_timer_func06,
    dtvd_tuner_timer_func07,
    dtvd_tuner_timer_func08,
    dtvd_tuner_timer_func09,
    dtvd_tuner_timer_func10,
    dtvd_tuner_timer_func11,
    dtvd_tuner_timer_func12,
    dtvd_tuner_timer_func13,
    dtvd_tuner_timer_func14,
    dtvd_tuner_timer_func15,
    dtvd_tuner_timer_func16,
    dtvd_tuner_timer_func17
};

#endif 

static DECLARE_WAIT_QUEUE_HEAD(head);

void dtvd_tuner_timer_init( void )
{
    int i;

    DTVD_TUNER_TIMER_t *timer_info = &tdtvd_tuner_main.timer;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    timer_info->timer_id = 0;

    for( i=0; i<D_DTVD_TUNER_TIMER_MAX; i++)
    {
        timer_info->list[i].state = D_DTVD_TUNER_TIMER_STOP;
#ifdef DTVD_NO_HRTIMER
        init_timer( &timer_info->list[i].info );
#else
        hrtimer_init( &timer_info->list[i].info, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
        timer_info->list[i].info.function = tdtvd_tuner_timer_timer_function[i];
#endif
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_timer_start
(
    unsigned long  funcid,      
    unsigned int   count        
)
{
    unsigned long data;

    volatile unsigned long temp_jiffies;   
    unsigned long main_exp;
    unsigned long sub_exp;

    DTVD_TUNER_TIMER_t *timer_info = &tdtvd_tuner_main.timer;

    DTVD_DEBUG_MSG_ENTER( (int)funcid, count, 0 );

    if( funcid >= D_DTVD_TUNER_TIMER_MAX )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TIMER,
                                  funcid, count, 0, 0, 0, 0 );
        return;
    }

    dtvd_tuner_timer_stop( funcid );

    if( timer_info->timer_id < D_DTVD_TUNER_TIMER_TIMERID_MAX )
    {
        timer_info->timer_id++;

    }
    else
    {
        timer_info->timer_id = 1;
    }
    data = timer_info->timer_id;

    data = data | ( (unsigned long)D_DTVD_TUNER_TIMER_KIND_START_BIT << funcid );

    if( count == 0 )
    {

        timer_info->list[funcid].state = D_DTVD_TUNER_TIMER_RUN;
#ifdef DTVD_NO_HRTIMER
        timer_info->list[funcid].info.data = data;
#else
        timer_info->list[funcid].data = data;
#endif

        dtvd_tuner_timer_handler( data );

        return;
    }

    temp_jiffies = jiffies;
#ifdef DTVD_NO_HRTIMER

    sub_exp = count % 10;
    main_exp = count / 10;
    main_exp *= 10;

    timer_info->list[funcid].info.expires = temp_jiffies + ( HZ * main_exp / 1000  );
    timer_info->list[funcid].info.data = data;
    timer_info->list[funcid].info.function = dtvd_tuner_timer_handler;

    add_timer( &timer_info->list[funcid].info );
#else

    sub_exp = count % 1000;
    main_exp = count / 1000;

    timer_info->list[funcid].data = data;

    hrtimer_start( &timer_info->list[funcid].info, ktime_set(main_exp,sub_exp * NSEC_PER_MSEC ), HRTIMER_MODE_REL);

#endif 

    timer_info->list[funcid].state = D_DTVD_TUNER_TIMER_RUN;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_timer_stop
(
    unsigned long funcid               
)
{
    signed int ret;

    DTVD_TUNER_TIMER_t *timer_info = &tdtvd_tuner_main.timer;

    DTVD_DEBUG_MSG_ENTER( (int)funcid, 0, 0 );

    if( funcid >= D_DTVD_TUNER_TIMER_MAX )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TIMER,
                                  funcid, 0, 0, 0, 0, 0 );
        return;
    }

    if( timer_info->list[funcid].state == D_DTVD_TUNER_TIMER_STOP )
    {
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

#ifdef DTVD_NO_HRTIMER
    ret = del_timer( &timer_info->list[funcid].info );
#else
	ret = hrtimer_cancel( &timer_info->list[funcid].info );
#endif 

    if( ret < 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_SYSTEM,
                                  DTVD_TUNER_TIMER,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    timer_info->list[funcid].state = D_DTVD_TUNER_TIMER_STOP;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_timer_handler
(
    unsigned long data               
)
{
    unsigned int      cnt;
    unsigned long   funcid;
    unsigned long   mask;
    unsigned char   need_toevt=D_DTVD_TUNER_TRUE;

    DTVD_DEBUG_MSG_ENTER( (int)data, 0, 0 );

    funcid = data >> ( 32 - D_DTVD_TUNER_TIMER_MAX_NUM );
    if( funcid > D_DTVD_TUNER_TIMER_BIT_MAX )
    {
        DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_TIMER,
                                  funcid, 0, 0, 0, 0, 0 );
        return;
    }

    dtvd_tuner_timer_handler_callfunc( funcid , &need_toevt);

    if( need_toevt != D_DTVD_TUNER_TRUE )
    {

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    for( cnt=0; cnt<D_DTVD_TUNER_TIMER_MAX; cnt++ )
    {
        mask = (unsigned long)( 0x0001 << cnt );
        if( ( funcid & mask ) == mask )
        {

            tdtvd_tuner_timer_tasklet[cnt]->data = data;
            tasklet_schedule( tdtvd_tuner_timer_tasklet[cnt] );
            break;
        }
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_timer_tasklet
(
    unsigned long data      
)
{
    DTVD_TUNER_MSG_u    body;
    unsigned int        len;

    DTVD_DEBUG_MSG_ENTER( (int)data, 0, 0 );

    len = sizeof(DTVD_TUNER_EXEVT_TIMEOUT_t);
    dtvd_tuner_memset( &body, 0x00,
                       len,
                       sizeof(DTVD_TUNER_MSG_u) );

    body.ex_timeout.timer_id = data;

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_TIMER,
                         D_DTVD_TUNER_CMD_TIMER_TIMEOUT,
                         &body, len );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_timer_mdelay
(
    unsigned long   count        
)
{
    DTVD_DEBUG_MSG_ENTER( (int)count, 0, 0 );

    if( count > 10 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TIMER,
                                  count, 0, 0, 0, 0, 0 );
        return;
    }

    if( count != 0 )
    {
        mdelay( count );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_timer_sleep_on_timeout
(
    signed long   count        
)
{
    signed long             res;
    DTVD_TUNER_CONTROL_t    *info = &tdtvd_tuner_info;

    int event_flag = 0;        

    DTVD_DEBUG_MSG_ENTER( (int)count, 0, 0 );

    if( count <= 0 )
    {
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    res = count;
    while( res > 0 )
    {

        res = wait_event_interruptible_timeout( info->wait_queue, event_flag !=0 ,(res * HZ / 1000) );
        event_flag = 0;
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_timer_handler_callfunc
(
    unsigned long   funcid,         
    unsigned char   *need_toevt     
)
{
    unsigned long   mask;

    if( need_toevt == NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TIMER,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    *need_toevt = D_DTVD_TUNER_TRUE;

    mask = 0;
    set_bit( D_DTVD_TUNER_TIMER_PWRON_HALFWAY, (void *)&mask );

    if( ( funcid & mask ) == mask )
    {

            {
                DTVD_TUNER_MSG_u    body;
                unsigned int        len;

                len = sizeof(DTVD_TUNER_EXEVT_TIMEOUT_t);
                dtvd_tuner_memset( &body, 0x00,
                                   len,
                                   sizeof(DTVD_TUNER_MSG_u) );

                dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_TIMER,
                                     D_DTVD_TUNER_CMD_TIMER_PWR_HALFTIMEOUT,
                                     &body, len );
            }

        *need_toevt = D_DTVD_TUNER_FALSE;

    }

    mask = 0;
    set_bit( D_DTVD_TUNER_TIMER_PWRON, (void *)&mask );

    if( ( funcid & mask ) == mask )
    {

        dtvd_tuner_timer_pwron();

        *need_toevt = D_DTVD_TUNER_TRUE;

    }

    return ;
}

void dtvd_tuner_timer_pwron_halfway( void )

{

    unsigned long addr;
    unsigned char data;

    addr = D_DTVD_TUNER_PSCNT_LDO7;
    dtvd_tuner_com_dev_powic_read( addr,           
                                   &data );        

    data =  (unsigned char)  ( data | D_DTVD_TUNER_PSCNT_LDO7_ON );
    dtvd_tuner_com_dev_powic_write( addr,          
                                    data );        

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_PWRON,
                            D_DTVD_TUNER_TIMER_PWRON2_MSEC );

    return ;
}

static void dtvd_tuner_timer_pwron( void )
{

    return ;
}

#ifndef DTVD_NO_HRTIMER

static enum hrtimer_restart dtvd_tuner_timer_func00(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[0].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func01(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[1].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func02(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[2].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func03(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[3].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func04(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[4].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func05(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[5].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func06(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[6].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func07(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[7].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func08(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[8].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func09(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[9].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func10(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[10].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func11(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[11].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func12(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[12].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func13(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[13].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func14(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[14].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func15(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[15].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func16(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[16].data );
    return HRTIMER_NORESTART;
}
static enum hrtimer_restart dtvd_tuner_timer_func17(struct hrtimer *timer)
{
	dtvd_tuner_timer_handler(tdtvd_tuner_main.timer.list[17].data );
    return HRTIMER_NORESTART;
}
#endif 

