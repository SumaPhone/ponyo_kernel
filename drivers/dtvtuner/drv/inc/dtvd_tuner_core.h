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

#ifndef _DTVD_TUNER_CORE_H_
#define _DTVD_TUNER_CORE_H_

#include "dtvd_tuner_autoeco.h"

#define D_DTVD_TUNER_CORE_000   0   
#define D_DTVD_TUNER_CORE_001   1   
#define D_DTVD_TUNER_CORE_002   2   
#define D_DTVD_TUNER_CORE_003   3   
#define D_DTVD_TUNER_CORE_004   4   
#define D_DTVD_TUNER_CORE_005   5   
#define D_DTVD_TUNER_CORE_006   6   
#define D_DTVD_TUNER_CORE_007   7   
#define D_DTVD_TUNER_CORE_008   8   
#define D_DTVD_TUNER_CORE_009   9   
#define D_DTVD_TUNER_CORE_010   10  
#define D_DTVD_TUNER_CORE_011   11  
#define D_DTVD_TUNER_CORE_012   12  
#define D_DTVD_TUNER_CORE_013   13  
#define D_DTVD_TUNER_CORE_014   14  

#define D_DTVD_TUNER_CORE_CH_GRP_NUM        8       
#define D_DTVD_TUNER_CORE_NORMAL_CH_GRP_A   0x13    
#define D_DTVD_TUNER_CORE_NORMAL_CH_GRP_B   0x19    
#define D_DTVD_TUNER_CORE_NORMAL_CH_GRP_C   0x1F    
#define D_DTVD_TUNER_CORE_NORMAL_CH_GRP_D   0x25    
#define D_DTVD_TUNER_CORE_NORMAL_CH_GRP_E   0x2A    
#define D_DTVD_TUNER_CORE_NORMAL_CH_GRP_F   0x30    
#define D_DTVD_TUNER_CORE_NORMAL_CH_GRP_G   0x37    
#define D_DTVD_TUNER_CORE_ECO_CH_GRP_A      0x13    
#define D_DTVD_TUNER_CORE_ECO_CH_GRP_B      0x19    
#define D_DTVD_TUNER_CORE_ECO_CH_GRP_C      0x1F    
#define D_DTVD_TUNER_CORE_ECO_CH_GRP_D      0x25    
#define D_DTVD_TUNER_CORE_ECO_CH_GRP_E      0x2A    
#define D_DTVD_TUNER_CORE_ECO_CH_GRP_F      0x30    
#define D_DTVD_TUNER_CORE_ECO_CH_GRP_G      0x37    

typedef struct _DTVD_TUNER_CORE_AGCTHR_t {
    unsigned char   ch_min;     
    unsigned char   ch_max;     
    unsigned char   agc_thr1;   
    unsigned char   agc_thr2;   
} DTVD_TUNER_CORE_AGCTHR_t;

typedef struct _DTVD_TUNER_CORE_INFO_t {
    signed int                  status;             

    struct file     *pipe_handle;                   

    DTVD_TUNER_CHANNEL_t        tune_ch;            
    DTVD_TUNER_ADJUST_t         tune_adj;           
    unsigned long               tune_seq_id;        
    unsigned char               tune_kind;          
    unsigned char               sync_flg;           
    unsigned char               tune_cause;         
    unsigned char               reserved1;          
    unsigned long               cn_seq_id;          
    unsigned long               tune_start_jiff;    
    unsigned long               tune_end_jiff;      
    unsigned char               omt_flag;           
    unsigned char               omt_tune_kind;      

    unsigned char               eco_flag;           
    unsigned char               reserved2;          
    unsigned char               stop_state_fst;     
    unsigned char               end_retry_flag;     
    unsigned char               reserved[2];        

    unsigned int                cn_qpsk1_2_m;       
    unsigned int                cn_qpsk1_2_h;       
    unsigned int                cn_qpsk2_3_m;       
    unsigned int                cn_qpsk2_3_h;       
    unsigned int                cn_16qam1_2_m;      
    unsigned int                cn_16qam1_2_h;      
    unsigned int                cn_qpsk1_2_eco_m;   
    unsigned int                cn_qpsk1_2_eco_h;   
    unsigned int                cn_qpsk2_3_eco_m;   
    unsigned int                cn_qpsk2_3_eco_h;   
    unsigned int                cn_16qam1_2_eco_m;  
    unsigned int                cn_16qam1_2_eco_h;  

    DTVD_TUNER_CORE_AGCTHR_t    agc_normal[D_DTVD_TUNER_CORE_CH_GRP_NUM]; 
    DTVD_TUNER_CORE_AGCTHR_t    agc_eco[D_DTVD_TUNER_CORE_CH_GRP_NUM];    

} DTVD_TUNER_CORE_INFO_t;

void dtvd_tuner_core_com_init( void );
void dtvd_tuner_core_com_init_agcthr( void );
void dtvd_tuner_core_com_init_cnthr( void );
void dtvd_tuner_core_com_set_status( signed int );
void dtvd_tuner_core_init_tuneflg( void );
void dtvd_tuner_core_set_tuneinfo( signed int );
unsigned long dtvd_tuner_core_get_cn_seq_id( void );
void dtvd_tuner_core_set_ng_cause( unsigned char );
signed int dtvd_tuner_core_jdg_agc( unsigned char, unsigned char, unsigned char );
void dtvd_tuner_core_deverr( void );
unsigned char dtvd_tuner_core_get_rx_level( void );
void dtvd_tuner_core_sync_time_end( void );
void dtvd_tuner_core_set_tuneinfo_omt( void );
void dtvd_tuner_core_set_eco_flag( void );

void dtvd_tuner_core_trans_msg_tune_ok( unsigned char );
void dtvd_tuner_core_trans_msg_tune_ng( unsigned char );
void dtvd_tuner_core_trans_msg_cn( unsigned char );
void dtvd_tuner_core_trans_msg_sync( void );
void dtvd_tuner_core_trans_msg_async( void );
void dtvd_tuner_core_trans_msg_deverr( void );
void dtvd_tuner_core_inevt_ant_init( void );
void dtvd_tuner_core_inevt_ant_end( unsigned char );
void dtvd_tuner_core_inevt_ant_set( void );
void dtvd_tuner_core_inevt_tuner_pwron( void );
void dtvd_tuner_core_inevt_tuner_pwroff( void );
void dtvd_tuner_core_inevt_tuner_init( void );
void dtvd_tuner_core_inevt_tuner_end( void );
void dtvd_tuner_core_inevt_tuner_tune_dtv( void );
void dtvd_tuner_core_inevt_tuner_stop( void );
void dtvd_tuner_core_inevt_agc_init( void );
void dtvd_tuner_core_inevt_agc_end( void );
void dtvd_tuner_core_inevt_agc_fst_measure( void );
void dtvd_tuner_core_inevt_agc_measure( void );
void dtvd_tuner_core_inevt_agc_stop( void );
void dtvd_tuner_core_inevt_cn_init( void );
void dtvd_tuner_core_inevt_cn_end( void );
void dtvd_tuner_core_inevt_cn_measure( unsigned long );
void dtvd_tuner_core_inevt_cn_stop( void );
void dtvd_tuner_core_inevt_ber_init( void );
void dtvd_tuner_core_inevt_ber_end( void );
void dtvd_tuner_core_inevt_ber_start( void );
void dtvd_tuner_core_inevt_ber_stop( void );
void dtvd_tuner_core_trans_msg_pre_tune( void );
void dtvd_tuner_core_inevt_cn_set( void );
void dtvd_tuner_core_inevt_ber_set( void );
void dtvd_tuner_core_inevt_tuner_init_omt( void );
void dtvd_tuner_core_inevt_tuner_stop_omt( void );
void dtvd_tuner_core_inevt_tuner_tune_dtv_omt( void );
void dtvd_tuner_core_inevt_ant_init_omt( void );
void dtvd_tuner_core_inevt_agc_init_omt( void );
void dtvd_tuner_core_inevt_agc_fst_measure_omt( void );
void dtvd_tuner_core_inevt_agc_start_omt( void );
void dtvd_tuner_core_inevt_agc_stop_omt( void );
void dtvd_tuner_core_inevt_agc_measure_omt( void );
void dtvd_tuner_core_inevt_agc_measure_stop_omt( void );
void dtvd_tuner_core_inevt_cn_init_omt( void );
void dtvd_tuner_core_inevt_cn_start_omt( unsigned char );
void dtvd_tuner_core_inevt_cn_stop_omt( void );
void dtvd_tuner_core_inevt_cn_measure_omt( void );
void dtvd_tuner_core_inevt_ber_init_omt( void );
void dtvd_tuner_core_inevt_ber_measure_cancel_omt( void );
void dtvd_tuner_core_inevt_ber_start_omt( unsigned char, unsigned short );
void dtvd_tuner_core_inevt_ber_stop_omt( void );
void dtvd_tuner_core_inevt_ber_measure_omt( void );
void dtvd_tuner_core_inevt_tuner_eco_set( void );

void dtvd_tuner_core_inevt_tuner_eco_set_param( unsigned char );
void dtvd_tuner_core_inevt_autoeco_init( void );
void dtvd_tuner_core_inevt_autoeco_end( void );
void dtvd_tuner_core_inevt_autoeco_start( void );
void dtvd_tuner_core_inevt_autoeco_stop( void );

void dtvd_tuner_core_inevt_autoeco_start_omt( void );
void dtvd_tuner_core_inevt_autoeco_stop_omt( void );
void dtvd_tuner_core_set_result( signed int );
void dtvd_tuner_core_set_tune_result( signed int );
void dtvd_tuner_core_wakeup( unsigned long );

void dtvd_tuner_core_trans_mdl_send_msg( unsigned short, void*, unsigned int );

extern DTVD_TUNER_CORE_INFO_t tdtvd_tuner_core;

#endif 

