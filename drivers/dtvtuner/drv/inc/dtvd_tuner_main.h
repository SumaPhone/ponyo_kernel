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

#ifndef _DTVD_TUNER_MAIN_H_
#define _DTVD_TUNER_MAIN_H_

#define D_DTVD_TUNER_MSG_OUTSIDE      0       
#define D_DTVD_TUNER_MSG_INSIDE       1       

#define D_DTVD_TUNER_TIMER_RUN        1       
#define D_DTVD_TUNER_TIMER_STOP       0       

#define D_DTVD_TUNER_TIMER_MAX_NUM          24

#define D_DTVD_TUNER_TIMER_TIMERID_MAX      0xFF

#define D_DTVD_TUNER_TIMER_BIT_MAX          ( 0x00000001 << ( D_DTVD_TUNER_TIMER_MAX - 1 ) )

#define D_DTVD_TUNER_TIMER_KIND_START_BIT   ( 0x80000000 >> ( D_DTVD_TUNER_TIMER_MAX_NUM - 1 ) )

typedef void (*DTVD_TUNER_FUNC_t)( void );

typedef signed int (*DTVD_TUNER_CMD_FUNC_t)( void );

typedef struct _DTVD_TUNER_TIMER_LIST_t {
    unsigned char       state;          
    unsigned char       reserved[3];    
#ifdef DTVD_NO_HRTIMER
    struct timer_list   info;           
#else
    struct hrtimer      info;           
    unsigned long       data;           
#endif
} DTVD_TUNER_TIMER_LIST_t;

typedef struct _DTVD_TUNER_TIMER_t {
    unsigned long              timer_id;           
    DTVD_TUNER_TIMER_LIST_t     list[D_DTVD_TUNER_TIMER_MAX]; 
} DTVD_TUNER_TIMER_t;

typedef struct _DTVD_TUNER_MAIN_t {
    DTVD_TUNER_TIMER_t  timer;                      
    DTVD_TUNER_RCVMSG_t inevt[D_DTVD_TUNER_INEVT_CUE_NUM]; 
} DTVD_TUNER_MAIN_t;

typedef struct _DTVD_TUNER_EVENT_ANLZ_TBL_t {
    unsigned int command;               
    DTVD_TUNER_CMD_FUNC_t func;           
} DTVD_TUNER_EVENT_ANLZ_TBL_t;

typedef struct _DTVD_TUNER_EVENT_DELIVER_TBL_t {
    signed int command;                 
    signed int to_id;                   
} DTVD_TUNER_EVENT_DELIVER_TBL_t;

typedef struct _DTVD_TUNER_EVENT_TBL_t {
    unsigned int blkid;                 
    DTVD_TUNER_EVENT_ANLZ_TBL_t* analize; 
} DTVD_TUNER_EVENT_TBL_t;

typedef struct _DTVD_TUNER_ANLZ_TBL_t {
    signed int key;                     
    signed int evtcode;                 
    unsigned short ctlid;               
} DTVD_TUNER_ANLZ_TBL_t;

#if 0

typedef struct _DTVD_TUNER_EVENT_TRANS_TBL_t {
    signed int event;                 
    DTVD_TUNER_TRANS_FUNC_t func;       
} DTVD_TUNER_EVENT_TRANS_TBL_t;
#endif

typedef struct _DTVD_TUNER_ANLZ_TIMER_TBL_t {
    signed int evtcode;                 
    unsigned short ctlid;               
} DTVD_TUNER_ANLZ_TIMER_TBL_t;

typedef struct _DTVD_TUNER_MTX_t {
    signed int evtcode;                 
    const DTVD_TUNER_FUNC_t funcp;        
} DTVD_TUNER_MTX_t;

typedef struct _DTVD_TUNER_MTX_TBL_t {
    unsigned int ctlid;                 
    DTVD_TUNER_MTX_t** mtxp;              
} DTVD_TUNER_MTX_TBL_t;

const DTVD_TUNER_EVENT_TBL_t* dtvd_tuner_main_anlz_get_table( void );
const DTVD_TUNER_MTX_TBL_t* dtvd_tuner_main_mtx_get_table( void );

extern DTVD_TUNER_MAIN_t tdtvd_tuner_main;

#endif 

