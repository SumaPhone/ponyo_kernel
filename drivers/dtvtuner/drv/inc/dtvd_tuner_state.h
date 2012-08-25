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

#ifndef _DTVD_TUNER_STATE_H_
#define _DTVD_TUNER_STATE_H_

#define D_DTVD_TUNER_STATE_000  0   
#define D_DTVD_TUNER_STATE_001  1   
#define D_DTVD_TUNER_STATE_002  2   

#define D_DTVD_TUNER_STATE_S0   0   
#define D_DTVD_TUNER_STATE_S1   1   
#define D_DTVD_TUNER_STATE_S2   2   
#define D_DTVD_TUNER_STATE_S3   3   
#define D_DTVD_TUNER_STATE_S4   4   
#define D_DTVD_TUNER_STATE_S5   5   
#define D_DTVD_TUNER_STATE_S6   6   
#define D_DTVD_TUNER_STATE_S7   7   
#define D_DTVD_TUNER_STATE_S8   8   
#define D_DTVD_TUNER_STATE_S9   9   

#define D_DTVD_TUNER_STATE_S_MAX  10

#define D_DTVD_TUNER_STATE_CONFIRM_SYNC     0   
#define D_DTVD_TUNER_STATE_CONFIRM_TSOUT    1   

#define D_DTVD_TUNER_STATE_CONFIRM_MAX      2   

#define D_DTVD_TUNER_STATE_TBL_WATCH        0   
#define D_DTVD_TUNER_STATE_TBL_SYNC         1   
#define D_DTVD_TUNER_STATE_TBL_TSOUT        2   

#define D_DTVD_TUNER_STATE_TBL_MAX          3

#define D_DTVD_TUNER_STATE_ACT_NONE     0x00000000  
#define D_DTVD_TUNER_STATE_ACT_MODE     0x00000001  
#define D_DTVD_TUNER_STATE_ACT_SYNC     0x00000002  
#define D_DTVD_TUNER_STATE_ACT_TMCC     0x00000004  
#define D_DTVD_TUNER_STATE_ACT_TSOUT    0x00000008  
#define D_DTVD_TUNER_STATE_ACT_ASYNC    0x00000010  
#define D_DTVD_TUNER_STATE_ACT_ERR      0x80000000  

typedef struct _DTVD_TUNER_STATE_WATCH_INFO_t {
    signed int      status;         
    unsigned int    cycle_time;     
    unsigned int    state_old;      
    unsigned int    state_new;      
} DTVD_TUNER_STATE_WATCH_INFO_t;

typedef struct _DTVD_TUNER_STATE_ACT_REQUEST_t {
    unsigned int next_state;              
    unsigned int event;                   
} DTVD_TUNER_STATE_ACT_REQUEST_t;

typedef DTVD_TUNER_STATE_ACT_REQUEST_t DTVD_TUNER_STATE_ACT_REQUEST_MTX_t
    [D_DTVD_TUNER_STATE_S_MAX][D_DTVD_TUNER_STATE_S_MAX];

void dtvd_tuner_state_com_set_status( signed int );
signed int dtvd_tuner_state_com_read( void );
void dtvd_tuner_state_com_watch_timer_start( void );
void dtvd_tuner_state_com_watch_timer_stop( void );
void dtvd_tuner_state_com_state_init( void );
void dtvd_tuner_state_com_action( unsigned char );

void dtvd_tuner_state_inevt_tuner_confirm( signed int );
void dtvd_tuner_state_inevt_core_deverr( void );
void dtvd_tuner_state_trans_msg_mode_ok( void );
void dtvd_tuner_state_trans_msg_sync( void );
void dtvd_tuner_state_trans_msg_tmcc( void );
void dtvd_tuner_state_trans_msg_tsout( void );
void dtvd_tuner_state_trans_msg_async( void );

extern DTVD_TUNER_STATE_WATCH_INFO_t tdtvd_tuner_state;

#endif 

