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

#ifndef _DTVD_TUNER_TSPRCV_H_
#define _DTVD_TUNER_TSPRCV_H_

#define D_DTVD_TUNER_TSPRCV_THREAD  "tunerrcv"      

#define D_DTVD_TUNER_TSP_FRAME              192       
#define D_DTVD_TUNER_TSP_LENGTH    ( D_DTVD_TUNER_TSP_FRAME * 20 )   
#define D_DTVD_TUNER_TSP_RINGBUFFER_MAX     24           

enum dvd_tuner_tsprcv_mode {
    D_DTVD_TUNER_TSPRCV_MODE_STOP = 0,   
    D_DTVD_TUNER_TSPRCV_MODE_1RCV,       
    D_DTVD_TUNER_TSPRCV_MODE_RECV        
};

enum dtvd_tuner_tsp_dma_flag {
    D_DTVD_TUNER_TSP_DMA_FLAG_OFF = 0,  
    D_DTVD_TUNER_TSP_DMA_FLAG_ON        
};

typedef struct _DTVD_TUNER_TSP_DMA_t {
    void        *vptr;              
    dma_addr_t  dma_handle;         
    int         flag;               
} DTVD_TUNER_TSP_DMA_t;

typedef struct _DTVD_TUNER_TSP_RINGBUFFER_t{
    spinlock_t              lock;                   
    int                     write_idx;              
    int                     read_idx;               
    DTVD_TUNER_TSP_DMA_t    dma[D_DTVD_TUNER_TSP_RINGBUFFER_MAX]; 
} DTVD_TUNER_TSP_RINGBUFFER_t;

typedef struct _DTVD_TUNER_TSPRCV_CTRL_t {
    unsigned char   mode;           
    unsigned char   reqstop;        
    unsigned long   tspcount;       
    unsigned long   overcount;      
} DTVD_TUNER_TSPRCV_CTRL_t;

void dtvd_tuner_tsprcv_init( void );
signed int dtvd_tuner_tsprcv_create_recv_thread( void );
void  dtvd_tuner_tsprcv_stop_recv_thread( void );

signed int dtvd_tuner_tsprcv_start_receiving( void );
void dtvd_tuner_tsprcv_stop_receiving( void );
signed int dtvd_tuner_tsprcv_read_tsp( char __user *buffer, size_t count );

signed int dtvd_tuner_tsprcv_create_ringbuffer( void );
void dtvd_tuner_tsprcv_delete_ringbuffer( void );

DTVD_TUNER_TSP_DMA_t * dtvd_tuner_tsprcv_write_dma_from_ringbuffer( int next ); 
DTVD_TUNER_TSP_DMA_t * dtvd_tuner_tsprcv_read_dma_from_ringbuffer( int next ); 

void dtvd_tuner_tsprcv_trans_msg_deverr( void );
void dtvd_tuner_tsprcv_trans_msg_overflow( void );

extern  struct task_struct *tdtvd_tuner_tsprcv_thread; 

extern  DTVD_TUNER_TSP_RINGBUFFER_t  tdtvd_tuner_tsp_ringbuffer;
extern  DTVD_TUNER_TSPRCV_CTRL_t     tdtvd_tuner_tsprcv_ctrl;

#endif 

