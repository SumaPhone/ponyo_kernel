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

#ifndef _DTVD_TUNER_BER_H_
#define _DTVD_TUNER_BER_H_

#define D_DTVD_TUNER_BER_000    0   
#define D_DTVD_TUNER_BER_001    1   
#define D_DTVD_TUNER_BER_002    2   
#define D_DTVD_TUNER_BER_003    3   
#define D_DTVD_TUNER_BER_004    4   
#define D_DTVD_TUNER_BER_005    5   
#define D_DTVD_TUNER_BER_006    6   

#define D_DTVD_TUNER_BER_TIMER_COUNT        30

#define D_DTVD_TUNER_BER_CYCLE_TIME          0

typedef struct _DTVD_TUNER_BER_INFO_t {
    signed int      status;         
    unsigned int    wait_time;      
    unsigned int    measure_time;   
    unsigned int    cycle_time;     
    unsigned short  count_max;      
    unsigned short  confirm_times;  
} DTVD_TUNER_BER_INFO_t;

void dtvd_tuner_ber_com_set_status( signed int );
signed int dtvd_tuner_ber_com_read( void );
void dtvd_tuner_ber_com_watch_timer_start( void );
signed int dtvd_tuner_ber_com_read_flag( unsigned char * );
signed int dtvd_tuner_ber_com_start_set( void );
signed int dtvd_tuner_ber_com_start_set_ext_omt( void );
signed int dtvd_tuner_ber_com_stop_set_ext_omt( void );
void dtvd_tuner_ber_com_init( void );
signed int dtvd_tuner_ber_com_start_set_init( unsigned short * );

void dtvd_tuner_ber_inevt_core_init( void );
void dtvd_tuner_ber_inevt_core_end( void );
void dtvd_tuner_ber_inevt_core_stop( void );
void dtvd_tuner_ber_inevt_core_deverr( void );
void dtvd_tuner_ber_inevt_core_init_omt( void );
void dtvd_tuner_ber_inevt_core_start_omt( signed int );
void dtvd_tuner_ber_inevt_core_stop_omt( signed int );
void dtvd_tuner_ber_inevt_core_measure_omt( signed int );

extern DTVD_TUNER_BER_INFO_t tdtvd_tuner_ber;

#endif 

