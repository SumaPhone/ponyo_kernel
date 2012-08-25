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

#ifndef _DTVD_TUNER_AGC_H_
#define _DTVD_TUNER_AGC_H_

#define D_DTVD_TUNER_AGC_000    0   
#define D_DTVD_TUNER_AGC_001    1   
#define D_DTVD_TUNER_AGC_002    2   
#define D_DTVD_TUNER_AGC_003    3   
#define D_DTVD_TUNER_AGC_004    4   
#define D_DTVD_TUNER_AGC_005    5   
#define D_DTVD_TUNER_AGC_006    6   
#define D_DTVD_TUNER_AGC_007    7   
#define D_DTVD_TUNER_AGC_008    8   

typedef struct _DTVD_TUNER_AGC_INFO_t {
    signed int      status;         
    unsigned int    agc_wait;       
    unsigned int    agc_cycle;      
} DTVD_TUNER_AGC_INFO_t;

void dtvd_tuner_agc_com_set_status( signed int );
signed int dtvd_tuner_agc_com_read( void );
void dtvd_tuner_agc_com_watch_timer_start( void );
void dtvd_tuner_agc_com_get_timer_value( void );

void dtvd_tuner_agc_inevt_core_init( void );
void dtvd_tuner_agc_inevt_core_end( void );
void dtvd_tuner_agc_inevt_core_measure_fst( void );
void dtvd_tuner_agc_inevt_core_measure( void );
void dtvd_tuner_agc_inevt_core_stop( void );
void dtvd_tuner_agc_inevt_core_deverr( void );
void dtvd_tuner_agc_inevt_core_init_omt( void );
void dtvd_tuner_agc_inevt_core_measure_fst_omt( signed int );
void dtvd_tuner_agc_inevt_core_start_omt( signed int );
void dtvd_tuner_agc_inevt_core_stop_omt( signed int );
void dtvd_tuner_agc_inevt_core_measure_omt( signed int );
void dtvd_tuner_agc_inevt_core_measure_stop_omt( signed int );

extern DTVD_TUNER_AGC_INFO_t tdtvd_tuner_agc;

#endif 

