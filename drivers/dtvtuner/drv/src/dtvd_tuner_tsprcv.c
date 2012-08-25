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
#include <linux/kthread.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>

static  int dtvd_tuner_tsprcv_recv_thread_main( void *param );   

#define NEXTPTR(cnt)          ( (((tdtvd_tuner_tsp_ringbuffer.write_idx+(cnt))%D_DTVD_TUNER_TSP_RINGBUFFER_MAX) == tdtvd_tuner_tsp_ringbuffer.read_idx)?          0:(&tdtvd_tuner_tsp_ringbuffer.dma[(tdtvd_tuner_tsp_ringbuffer.write_idx+(cnt))%D_DTVD_TUNER_TSP_RINGBUFFER_MAX]))

static int dtvd_tuner_tsprcv_check_and_recovery(char frameSyncData)
{
    unsigned long           flags;
    int retval = D_DTVD_TUNER_FALSE;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if (frameSyncData != 0x47)
    {
        retval = D_DTVD_TUNER_TRUE;

        spin_lock_irqsave(&tdtvd_tuner_tsp_ringbuffer.lock, flags);
        tdtvd_tuner_tsp_ringbuffer.write_idx = 0;
        tdtvd_tuner_tsp_ringbuffer.read_idx = 0;
        spin_unlock_irqrestore(&tdtvd_tuner_tsp_ringbuffer.lock, flags);

        dtvd_tuner_com_dev_tsif_stop();
        dtvd_tuner_com_dev_tsif_start();
    }

    DTVD_DEBUG_MSG_EXIT();

    return retval;
}

signed int dtvd_tuner_tsprcv_create_recv_thread( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_tsprcv_thread = kthread_create( dtvd_tuner_tsprcv_recv_thread_main,
                                               (void *)&tdtvd_tuner_tsprcv_ctrl,
                                               D_DTVD_TUNER_TSPRCV_THREAD);
    if( IS_ERR(tdtvd_tuner_tsprcv_thread) )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TSPRCV,
                                  PTR_ERR(tdtvd_tuner_tsprcv_thread), 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    wake_up_process(tdtvd_tuner_tsprcv_thread);

    DTVD_DEBUG_MSG_EXIT();

    return D_DTVD_TUNER_OK;
}

void  dtvd_tuner_tsprcv_stop_recv_thread( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    kthread_stop( tdtvd_tuner_tsprcv_thread );

    DTVD_DEBUG_MSG_EXIT();

    return;
}

static  int dtvd_tuner_tsprcv_recv_thread_main
(
 void *param     
 )
{
	DTVD_TUNER_TSPRCV_CTRL_t    *ctrl ;     
	DTVD_TUNER_TSP_DMA_t        *dma ;      
	DTVD_TUNER_TSP_DMA_t        *dma_next ; 
	DTVD_TUNER_TSP_DMA_t        *dma_next2; 

	void *dma_handle[2];
	unsigned int                rcvlength[2];

	int ret;

	DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

	if( param == D_DTVD_TUNER_NULL )
	{

		DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
				DTVD_TUNER_TSPRCV,
				0, 0, 0, 0, 0, 0 );
		return -1;
	}

	for (;;)
	{
		ctrl = (DTVD_TUNER_TSPRCV_CTRL_t*)param;

		dma = dtvd_tuner_tsprcv_write_dma_from_ringbuffer( 0 );
		dma_next = NEXTPTR(1);

		ctrl->mode = D_DTVD_TUNER_TSPRCV_MODE_RECV ;
		ctrl->tspcount = 0;
		ctrl->overcount = 0;

		DTVD_DEBUG_MSG_INFO( "***tsp rcv start***\n" );

		dma->flag = D_DTVD_TUNER_TSP_DMA_FLAG_ON;            
		dma_next->flag = D_DTVD_TUNER_TSP_DMA_FLAG_ON;       

		dma_handle[0] = dma->vptr;
		dma_handle[1] = dma_next->vptr;



		rcvlength[0] = D_DTVD_TUNER_TSP_LENGTH;  
		rcvlength[1] = D_DTVD_TUNER_TSP_LENGTH;
		ret = dtvd_tuner_com_dev_tsif_read_start(dma_handle, rcvlength);

		for(;;)
		{
			if(ret != D_DTVD_TUNER_OK)
			{
				return -1;
			}

			if(ctrl->reqstop)
			{
printk ("%s-%d\n", __FILE__, __LINE__);

				break;      
			}
			if((ctrl->tspcount % 100)==0){
				DTVD_DEBUG_MSG_INFO( "***tsp rcv***\n" );
			}
			ctrl->tspcount++;       

			if(dma_next != D_DTVD_TUNER_NULL)
			{

				dma->flag = D_DTVD_TUNER_TSP_DMA_FLAG_OFF;       

				dma_next2 = NEXTPTR(2);
				if( dma_next2 != D_DTVD_TUNER_NULL )
				{

					dtvd_tuner_tsprcv_write_dma_from_ringbuffer( 1 );

					if (dtvd_tuner_tsprcv_check_and_recovery(((char*)dma->vptr)[0]) != D_DTVD_TUNER_FALSE)
					{
						break;
					}

					dma = dma_next;         
					dma_next = dma_next2;
					dma_next->flag = D_DTVD_TUNER_TSP_DMA_FLAG_ON;       
					ret = dtvd_tuner_com_dev_tsif_read(dma_next->vptr,
							D_DTVD_TUNER_TSP_LENGTH);
				}else{

					dtvd_tuner_tsprcv_write_dma_from_ringbuffer( 1 );

					ctrl->overcount++;      
					DTVD_DEBUG_MSG_INFO( "***tsp rcv overflow(N:%ld)***\n", ctrl->overcount );
					dtvd_tuner_tsprcv_trans_msg_overflow();  

					if (dtvd_tuner_tsprcv_check_and_recovery(((char*)dma->vptr)[0]) != D_DTVD_TUNER_FALSE)
					{
						break;
					}

					dma = dma_next;
					dma_next = D_DTVD_TUNER_NULL;
					ret = dtvd_tuner_com_dev_tsif_read(dma->vptr,
							D_DTVD_TUNER_TSP_LENGTH);
				}

			}else{

				dma_next = NEXTPTR(1);
				if( dma_next != D_DTVD_TUNER_NULL)
				{

					dma_next->flag = D_DTVD_TUNER_TSP_DMA_FLAG_ON;       
					ret = dtvd_tuner_com_dev_tsif_read(dma_next->vptr,
							D_DTVD_TUNER_TSP_LENGTH);

					if (dtvd_tuner_tsprcv_check_and_recovery(((char*)dma->vptr)[0]) != D_DTVD_TUNER_FALSE)
					{
						break;
					}

				}else{

					ctrl->overcount++;      
					if((ctrl->overcount%100) == 0){
						DTVD_DEBUG_MSG_INFO( "***tsp rcv overflow(C:%ld)***\n", ctrl->overcount );
					}

					ret = dtvd_tuner_com_dev_tsif_read(dma->vptr,
							D_DTVD_TUNER_TSP_LENGTH);

					if (dtvd_tuner_tsprcv_check_and_recovery(((char*)dma->vptr)[0]) != D_DTVD_TUNER_FALSE)
					{
						break;
					}
				}
			}
		}

		dtvd_tuner_com_dev_tsif_read_stop();

		if(ctrl->reqstop)
		{

printk ("%s-%d \n", __FILE__, __LINE__);
			break;      
		}
	}

	for(;;) 
	{

		if(kthread_should_stop())
		{

printk ("%s-%d \n", __FILE__, __LINE__);
			break;      
		}
	}

	DTVD_DEBUG_MSG_EXIT();

	return 0;      
}

signed int dtvd_tuner_tsprcv_create_ringbuffer( void )
{
    int     i;
    void    *ptr;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( (void*)&tdtvd_tuner_tsp_ringbuffer,      
                        0,                                      
                        sizeof(tdtvd_tuner_tsp_ringbuffer),     
                        sizeof(tdtvd_tuner_tsp_ringbuffer));    

    spin_lock_init( &tdtvd_tuner_tsp_ringbuffer.lock );

    for( i=0; i<D_DTVD_TUNER_TSP_RINGBUFFER_MAX; i++)
    {
        ptr = dma_alloc_coherent(D_DTVD_TUNER_NULL,             
                                D_DTVD_TUNER_TSP_LENGTH,        
                                &tdtvd_tuner_tsp_ringbuffer.dma[i].dma_handle, 
                                GFP_KERNEL);                    
        if( ptr == D_DTVD_TUNER_NULL )
        {

            DTVD_TUNER_SYSERR_RANK_B( D_DTVD_TUNER_SYSERR_DRV_I2C_READ,
                                  DTVD_TUNER_TSPRCV,
                                  ptr, i, D_DTVD_TUNER_TSP_LENGTH, &tdtvd_tuner_tsp_ringbuffer.dma[i], 0, 0 );

            dtvd_tuner_tsprcv_delete_ringbuffer();     

            return D_DTVD_TUNER_NG;
        }
        tdtvd_tuner_tsp_ringbuffer.dma[i].vptr = ptr;
    }

    DTVD_DEBUG_MSG_EXIT();

    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_tsprcv_delete_ringbuffer( void )
{
    int     i;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    for( i=0; i<D_DTVD_TUNER_TSP_RINGBUFFER_MAX; i++)
    {
        if( tdtvd_tuner_tsp_ringbuffer.dma[i].vptr != D_DTVD_TUNER_NULL)
        {

            dma_free_coherent(D_DTVD_TUNER_NULL,                    
                              D_DTVD_TUNER_TSP_LENGTH,              
                              tdtvd_tuner_tsp_ringbuffer.dma[i].vptr,          
                              tdtvd_tuner_tsp_ringbuffer.dma[i].dma_handle);   
        }
        tdtvd_tuner_tsp_ringbuffer.dma[i].vptr = D_DTVD_TUNER_NULL; 
    }

    DTVD_DEBUG_MSG_EXIT();

    return;
}

DTVD_TUNER_TSP_DMA_t * dtvd_tuner_tsprcv_read_dma_from_ringbuffer
(
int next         
)
{
    DTVD_TUNER_TSP_DMA_t     *dma_p;
    unsigned long           flags;

    if( tdtvd_tuner_tsp_ringbuffer.write_idx == tdtvd_tuner_tsp_ringbuffer.read_idx )
    {

        return  D_DTVD_TUNER_NULL;
    }

    if( next == 0 )
    {

        dma_p = &tdtvd_tuner_tsp_ringbuffer.dma[tdtvd_tuner_tsp_ringbuffer.read_idx];
    }
    else
    {

        spin_lock_irqsave(&tdtvd_tuner_tsp_ringbuffer.lock, flags);

        tdtvd_tuner_tsp_ringbuffer.read_idx = ((tdtvd_tuner_tsp_ringbuffer.read_idx +1) % D_DTVD_TUNER_TSP_RINGBUFFER_MAX);

        spin_unlock_irqrestore(&tdtvd_tuner_tsp_ringbuffer.lock, flags);

        if( tdtvd_tuner_tsp_ringbuffer.write_idx == tdtvd_tuner_tsp_ringbuffer.read_idx )
        {

            return  D_DTVD_TUNER_NULL;
        }

        dma_p = &tdtvd_tuner_tsp_ringbuffer.dma[tdtvd_tuner_tsp_ringbuffer.read_idx];

    }

    return  dma_p;
}

DTVD_TUNER_TSP_DMA_t * dtvd_tuner_tsprcv_write_dma_from_ringbuffer
(
int next         
)
{
    DTVD_TUNER_TSP_DMA_t     *dma_p;
    unsigned long           flags;

    if( next == 0 )
    {

        dma_p = &tdtvd_tuner_tsp_ringbuffer.dma[tdtvd_tuner_tsp_ringbuffer.write_idx];
    }
    else
    {

        if( ((tdtvd_tuner_tsp_ringbuffer.write_idx +1) % D_DTVD_TUNER_TSP_RINGBUFFER_MAX) == tdtvd_tuner_tsp_ringbuffer.read_idx )
        {

            return  D_DTVD_TUNER_NULL;
        }

        spin_lock_irqsave(&tdtvd_tuner_tsp_ringbuffer.lock, flags);

        tdtvd_tuner_tsp_ringbuffer.write_idx = ((tdtvd_tuner_tsp_ringbuffer.write_idx +1) % D_DTVD_TUNER_TSP_RINGBUFFER_MAX);

        spin_unlock_irqrestore(&tdtvd_tuner_tsp_ringbuffer.lock, flags);

        dma_p = &tdtvd_tuner_tsp_ringbuffer.dma[tdtvd_tuner_tsp_ringbuffer.write_idx];

    }

    return  dma_p;
}

