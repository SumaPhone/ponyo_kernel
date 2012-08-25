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

#ifndef _DTVD_TUNER_CN_H_
#define _DTVD_TUNER_CN_H_

#define D_DTVD_TUNER_CN_000     0   
#define D_DTVD_TUNER_CN_001     1   
#define D_DTVD_TUNER_CN_002     2   
#define D_DTVD_TUNER_CN_003     3   
#define D_DTVD_TUNER_CN_004     4   
#define D_DTVD_TUNER_CN_005     5   
#define D_DTVD_TUNER_CN_006     6   

#define D_DTVD_TUNER_CN_CYCLE_TIME          0

typedef struct _DTVD_TUNER_CN_INFO_t {
    signed int        status;                 
    unsigned long     seq_id;                 
    unsigned int      wait_time;              
    unsigned int      measure_time;           
    unsigned int      cycle_time;             
    unsigned short    ave_count;              
    unsigned char     alpha;                  
    unsigned char     beta;                   
    unsigned char     gamma;                  
    unsigned char     stabilize_times;        
    unsigned char     reserved[2];            
    DTVD_TUNER_MEASURE_VALUE_t    cn_old;     
    DTVD_TUNER_MEASURE_VALUE_t    cn_old_ave; 
    DTVD_TUNER_MEASURE_VALUE_t    cn_new;     
    unsigned char     symcount;               
    unsigned char     reserved2[3];           
} DTVD_TUNER_CN_INFO_t;

typedef struct _DTVD_TUNER_CN_SYMTOTIME_t {
    unsigned int       timer;
    unsigned char      symcount;
    unsigned char      reserved[3];
} DTVD_TUNER_CN_SYMTOTIME_t;
void dtvd_tuner_cn_com_set_status( signed int );
signed int dtvd_tuner_cn_com_read( void );
void dtvd_tuner_cn_com_watch_timer_start( void );
signed int dtvd_tuner_cn_com_start_set( void );
void dtvd_tuner_cn_com_init( void );
void dtvd_tuner_cn_com_average( void );
void dtvd_tuner_cn_com_average_set( void );
signed int dtvd_tuner_cn_com_symcount_set( unsigned char );
signed int dtvd_tuner_cn_com_symcount_read( void );

void dtvd_tuner_cn_com_result_set( void );

void dtvd_tuner_cn_com_measure_init( void );
void dtvd_tuner_cn_com_measure_monitor_init( void );

void dtvd_tuner_cn_com_symcount_chg_timer( unsigned char );
void dtvd_tuner_cn_com_measure_time_set(void);

void dtvd_tuner_cn_inevt_core_init( void );
void dtvd_tuner_cn_inevt_core_end( void );
void dtvd_tuner_cn_inevt_core_measure( void );
void dtvd_tuner_cn_inevt_core_stop( void );
void dtvd_tuner_cn_inevt_core_deverr( void );
void dtvd_tuner_cn_inevt_core_init_omt( signed int );
void dtvd_tuner_cn_inevt_core_start_omt( signed int );
void dtvd_tuner_cn_inevt_core_stop_omt( signed int );
void dtvd_tuner_cn_inevt_core_measure_omt( signed int );

void dtvd_tuner_cn_inevt_autoeco_notice( DTVD_TUNER_MEASURE_VALUE_t );

extern DTVD_TUNER_CN_INFO_t tdtvd_tuner_cn;

#endif 

