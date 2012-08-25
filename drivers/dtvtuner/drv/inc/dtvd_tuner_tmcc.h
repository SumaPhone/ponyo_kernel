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

#ifndef _DTVD_TUNER_TMCC_H_
#define _DTVD_TUNER_TMCC_H_

#define D_DTVD_TUNER_TMCC_000   0   
#define D_DTVD_TUNER_TMCC_001   1   
#define D_DTVD_TUNER_TMCC_002   2   
#define D_DTVD_TUNER_TMCC_003   3   

#define D_DTVD_TUNER_TMCC_INFOGET_STANDARD      0       
#define D_DTVD_TUNER_TMCC_INFOGET_NONSTD        1       

#define D_DTVD_TUNER_TMCC_MEASURE_NUM           32

#define D_DTVD_TUNER_TIMER_TMCC_CYCLE_MSEC      1000    

typedef struct _DTVD_TUNER_TMCC_WATCH_INFO_t {
    signed int      status;         
    unsigned int    wait_time;      
    unsigned int    cycle_time;     
} DTVD_TUNER_TMCC_WATCH_INFO_t;

void dtvd_tuner_tmcc_com_set_status( signed int );
signed int dtvd_tuner_tmcc_com_read( void );
unsigned char dtvd_tuner_tmcc_com_jdg_std( void );
void dtvd_tuner_tmcc_com_watch_timer_start( void );
signed int dtvd_tuner_tmcc_com_read_valid_correct( signed int *result );
signed int dtvd_tuner_tmcc_com_set_tmcclock( unsigned char lock );
signed int dtvd_tuner_tmcc_com_get_tmcc( signed int *result );
void dtvd_tuner_tmcc_com_init( void );
void dtvd_tuner_tmcc_com_get_timer_start( void );

void dtvd_tuner_tmcc_inevt_tuner_stop( void );
void dtvd_tuner_tmcc_inevt_tuner_infoget( unsigned char );
void dtvd_tuner_tmcc_inevt_core_deverr( void );

extern DTVD_TUNER_TMCC_WATCH_INFO_t tdtvd_tuner_tmcc;

#endif 

