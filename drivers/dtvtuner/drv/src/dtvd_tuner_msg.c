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

static signed int dtvd_tuner_msg_write( DTVD_TUNER_EX_MSG_DATA_t*, DTVD_TUNER_EX_MSG_QUEUE_t* );
static signed int dtvd_tuner_msg_read( DTVD_TUNER_EX_MSG_DATA_t*, DTVD_TUNER_EX_MSG_QUEUE_t* );

static spinlock_t lock_msgwrite;

void dtvd_tuner_msg_init
(
    unsigned long   max,    
    unsigned long   size    
)
{
    DTVD_TUNER_EX_MSG_QUEUE_t *queue = &tdtvd_tuner_que;

    DTVD_DEBUG_MSG_ENTER( (int)max, (int)size, 0 );

    if( max > (D_DTVD_TUNER_EXEVT_CUE_NUM + 1) )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_MSG,
                                  0, 0, 0, 0, 0, 0 );
    }

    if( size > sizeof(DTVD_TUNER_EX_MSG_DATA_t) )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_MSG,
                                  0, 0, 0, 0, 0, 0 );
    }

    queue->max      = max;
    queue->size     = size;
    queue->pwrite   = queue->buf;
    queue->pread    = queue->buf;

    spin_lock_init( &lock_msgwrite );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_msg_send
(
    unsigned short      fromid,         
    unsigned short      command,        
    void*               body,           
    unsigned int        len             
)
{
    signed int ret;
    DTVD_TUNER_EX_MSG_DATA_t msg;

    DTVD_DEBUG_MSG_ENTER( fromid, command, len );

    if( body == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_MSG,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    dtvd_tuner_memset( &msg, 0x00,
                       sizeof(DTVD_TUNER_EX_MSG_DATA_t),
                       sizeof(DTVD_TUNER_EX_MSG_DATA_t) );

    msg.fromid = fromid;
    msg.command = command;

    dtvd_tuner_memmove( &msg.buf, body, len, sizeof(DTVD_TUNER_MSG_u) );

    ret = dtvd_tuner_msg_write( &msg, &tdtvd_tuner_que );
    if( ret != 0 )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_MSG,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    atomic_inc( &tdtvd_tuner_info.event_flg );

    wake_up_interruptible( &tdtvd_tuner_info.wait_queue );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

signed int dtvd_tuner_msg_rcv
(
    DTVD_TUNER_EX_MSG_DATA_t* msg   
)
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( msg == NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_MSG,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = dtvd_tuner_msg_read( msg, &tdtvd_tuner_que );
    if( ret != 0 )
    {

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    atomic_dec( &tdtvd_tuner_info.event_flg );

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

static signed int dtvd_tuner_msg_write
(
    DTVD_TUNER_EX_MSG_DATA_t*  req,     
    DTVD_TUNER_EX_MSG_QUEUE_t* queue    
)
{
    DTVD_TUNER_EX_MSG_DATA_t *pread;
    unsigned long   flags = 0;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( req == NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_MSG,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }
    if( queue == NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_MSG,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    spin_lock_irqsave( &lock_msgwrite, flags );

    pread = queue->pread;
    if( (queue->pwrite + 1) >= (queue->buf + queue->max) )
    {

        if( pread == queue->buf )
        {

            spin_unlock_irqrestore( &lock_msgwrite, flags );
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
        else
        {

            dtvd_tuner_memcpy( queue->pwrite, req, queue->size, queue->size );
            queue->pwrite = queue->buf;
        }
    }
    else
    {

        if( pread == (queue->pwrite + 1) )
        {

            spin_unlock_irqrestore( &lock_msgwrite, flags );
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
        else
        {

            dtvd_tuner_memcpy( queue->pwrite, req, queue->size, queue->size );
            ++(queue->pwrite);
        }
    }

    spin_unlock_irqrestore( &lock_msgwrite, flags );

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

static signed int dtvd_tuner_msg_read
(
    DTVD_TUNER_EX_MSG_DATA_t*  req,     
    DTVD_TUNER_EX_MSG_QUEUE_t* queue    
)
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( req == NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_MSG,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }
    if( queue == NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_MSG,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    if( queue->pwrite == queue->pread )
    {

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    else
    {

        dtvd_tuner_memcpy( req, queue->pread, queue->size, queue->size );
        if( (queue->pread + 1) >= (queue->buf + queue->max) )
        {

            queue->pread = queue->buf;
        }
        else
        {

            ++(queue->pread);
        }
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_OK;
    }
}

