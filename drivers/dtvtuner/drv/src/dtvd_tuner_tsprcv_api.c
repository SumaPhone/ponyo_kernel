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

void dtvd_tuner_tsprcv_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &tdtvd_tuner_tsprcv_ctrl, 0x00,
                       sizeof( DTVD_TUNER_TSPRCV_CTRL_t ),
                       sizeof( DTVD_TUNER_TSPRCV_CTRL_t ) );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

signed int dtvd_tuner_tsprcv_start_receiving( void )
{
    signed int  ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_tsprcv_ctrl.mode != 0 )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_OK;
    }

    dtvd_tuner_memset( &tdtvd_tuner_tsprcv_ctrl, 0x00,
                       sizeof( DTVD_TUNER_TSPRCV_CTRL_t ),
                       sizeof( DTVD_TUNER_TSPRCV_CTRL_t ) );

    ret = dtvd_tuner_com_dev_tsif_start();
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = dtvd_tuner_tsprcv_create_ringbuffer();
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    ret = dtvd_tuner_tsprcv_create_recv_thread();
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();

    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_tsprcv_stop_receiving( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );
    if( tdtvd_tuner_tsprcv_ctrl.mode != 0 )
    {

        tdtvd_tuner_tsprcv_ctrl.reqstop = 1;

        dtvd_tuner_com_dev_tsif_read_cancel();
        dtvd_tuner_com_dev_tsif_stop();

        dtvd_tuner_tsprcv_stop_recv_thread();


        dtvd_tuner_tsprcv_delete_ringbuffer();
        tdtvd_tuner_tsprcv_ctrl.mode = 0;
    }

    DTVD_DEBUG_MSG_EXIT();

    return;
}

signed int dtvd_tuner_tsprcv_read_tsp
(
    char __user    *buffer,         
    size_t         count            
)
{
    DTVD_TUNER_TSP_DMA_t *dma_adr;  
    unsigned int   len;

    DTVD_DEBUG_MSG_ENTER( count, 0, 0 );

    if( buffer == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TSPRCV_API,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    dma_adr = dtvd_tuner_tsprcv_read_dma_from_ringbuffer( 0 );
    if( dma_adr == D_DTVD_TUNER_NULL )
    {
        return 0;
    }

    len = count;
    if( count > D_DTVD_TUNER_TSP_LENGTH )
    {
        len = D_DTVD_TUNER_TSP_LENGTH;
    }

    if( copy_to_user( (void __user *)buffer, ( const void * )dma_adr->vptr, len) )
    {
        len = D_DTVD_TUNER_NG; 
    }

    dtvd_tuner_tsprcv_read_dma_from_ringbuffer( 1 );

    DTVD_DEBUG_MSG_EXIT();

    return len;
}

