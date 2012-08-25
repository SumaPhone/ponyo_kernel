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

#ifndef _DTVD_TUNER_TUNER_H_
#define _DTVD_TUNER_TUNER_H_
#include "dtvd_tuner_tmcc.h"
#include "dtvd_tuner_state.h"

#define D_DTVD_TUNER_TUNER_000      0   
#define D_DTVD_TUNER_TUNER_001      1   
#define D_DTVD_TUNER_TUNER_002      2   
#define D_DTVD_TUNER_TUNER_003      3   
#define D_DTVD_TUNER_TUNER_004      4   
#define D_DTVD_TUNER_TUNER_005      5   
#define D_DTVD_TUNER_TUNER_006      6   
#define D_DTVD_TUNER_TUNER_007      7   
#define D_DTVD_TUNER_TUNER_008      8   
#define D_DTVD_TUNER_TUNER_009      9   
#define D_DTVD_TUNER_TUNER_010     10   

#define D_DTVD_TUNER_TIMER_PWRON1_MSEC      10  
#define D_DTVD_TUNER_TIMER_PWRON2_MSEC      10  

#define D_DTVD_TUNER_RESET_OFF_TIMER            20      

#define D_DTVD_TUNER_TIMER_SOFTRST_MSEC     2   

#define D_DTVD_TUNER_TIMER_HARDRST_MSEC     30  
#define D_DTVD_TUNER_TIMER_PWROFF1_MSEC     20  
#define D_DTVD_TUNER_TIMER_PWROFF2_MSEC     20  
#define D_DTVD_TUNER_TIMER_DATASEND_MSEC    2   
#define D_DTVD_TUNER_TIMER_ECO_MSEC         0   

#define D_DTVD_TUNER_OFDM_INIT_TBL_SIZE     34  

#define D_DTVD_TUNER_IFFREQ_INIT_TBL_SIZE   4   
#define D_DTVD_TUNER_VAINT_FACTOR_CLEAR_TBL_SIZE    8   

#define D_DTVD_TUNER_RF_INIT_TBL_SIZE       24  

#define D_DTVD_TUNER_RF_CH_TBL_SIZE         4   

#define D_DTVD_TUNER_ECO_MODE_OFF           0   
#define D_DTVD_TUNER_ECO_MODE_ON            1   
#define D_DTVD_TUNER_ECO_MODE_OFF_SHIFT     2   
#define D_DTVD_TUNER_ECO_MODE_ON_SHIFT      3   

#define D_DTVD_TUNER_TUNER_INT_SET_NUM          10      
#define D_DTVD_TUNER_TUNER_INT_CHECK_MASK       0x20    

#define D_DTVD_TUNER_BIT_HI                 1   
#define D_DTVD_TUNER_BIT_LOW                0   

#define D_DTVD_TUNER_SEARCH_CHECK_MASK   0x10   

#define D_DTVD_TUNER_TUNING_1ST_TIME_TRUE   1   
#define D_DTVD_TUNER_TUNING_1ST_TIME_FALSE  0   

#define D_DTVD_TUNER_SEQ_RESET              0   
#define D_DTVD_TUNER_SEQ_START              1   

typedef struct _DTVD_TUNER_TUNER_INFO_t {
    signed int              status;                 
    unsigned int            wait_search_time;       
    unsigned int            wait_sync_time;         
    unsigned int            wait_ts_time;           
    unsigned char           mode_on;                
    unsigned char           tmcc_on;                
    unsigned char           mode_dec_cnt;           
    unsigned char           search_success_cnt;     
    unsigned char           search_success_thr;     
    unsigned char           search_time;            
    unsigned char           scan_mode_dec_cnt;      
    unsigned char           scan_search_success_cnt;
    DTVD_TUNER_CHANNEL_t    ch;                     
    DTVD_TUNER_ADJUST_t     adj;                    
    unsigned int            wait_refclk_time;       

    unsigned char           eco_mode;               
    unsigned char           sync_wait_flg;          

    unsigned char           search_thres;           
    unsigned char           tuning_cnt_flg;         
    signed long             wait_lock1;             
    signed long             wait_lock2;             

    spinlock_t              lock;                   
} DTVD_TUNER_TUNER_INFO_t;

extern void dtvd_tuner_tuner_com_set_status
(
    signed int next_status              
);
extern void dtvd_tuner_tuner_com_pwr_on_pega( void );
extern void dtvd_tuner_tuner_com_pwr_on_tuner( void );
extern void dtvd_tuner_tuner_com_pwr_off( void );
extern signed int dtvd_tuner_tuner_com_init( void );
extern signed int dtvd_tuner_tuner_com_ofdm_init( void );
extern signed int dtvd_tuner_tuner_com_tuner_int_disable( void );
extern signed int dtvd_tuner_tuner_com_sequence_init( void );
extern signed int dtvd_tuner_tuner_com_iffreq_set( void );
extern signed int dtvd_tuner_tuner_com_rficpow( void );
extern signed int dtvd_tuner_tuner_com_rficpow_eco( void );

extern signed int dtvd_tuner_tuner_com_rfic_send_tuneset
(
    unsigned char ch,                                          
    unsigned int kind                                          
);

extern signed int dtvd_tuner_tuner_com_sequence_start( void );
extern void dtvd_tuner_tuner_com_search_set( void );
extern signed int dtvd_tuner_tuner_com_tuner_int_clear( void );

extern signed int dtvd_tuner_tuner_com_tuner_int_enable( void );
extern signed int dtvd_tuner_tuner_com_search_start( void );
extern signed int dtvd_tuner_tuner_com_searchflg_judge( unsigned int *result );
extern signed int dtvd_tuner_tuner_com_search_result_read( unsigned int *result );
extern signed int dtvd_tuner_tuner_com_search_endset( void );
extern void dtvd_tuner_tuner_com_term( void );
extern void dtvd_tuner_tuner_com_infoinit( void );
extern signed int dtvd_tuner_tuner_com_vaint_factor_clear( void );
extern signed int dtvd_tuner_tuner_com_tuner_reset( void );
extern signed int dtvd_tuner_tuner_com_tuner_stop( void );

signed int dtvd_tuner_tuner_com_set_mode_tmcc_info(unsigned int kind);

extern signed int dtvd_tuner_tuner_com_search_count_set(unsigned int kind);
extern signed int dtvd_tuner_tuner_com_search_resreg_set( void );
extern signed int dtvd_tuner_tuner_com_set_eco(unsigned char mode);
extern void dtvd_tuner_tuner_com_timeout_eco( void );
extern void dtvd_tuner_tuner_com_shift_stop( void );
extern signed int dtvd_tuner_tuner_com_modegi_get( void );
extern void dtvd_tuner_tuner_com_wait_lock( void );                    
extern signed int dtvd_tuner_tuner_com_rfic_sequencer( unsigned char );

extern signed int dtvd_tuner_tuner_com_i2c_bitset
(
    unsigned char addr,
    unsigned int  bit,
    unsigned char kind
);

void dtvd_tuner_tuner_com_cancel_reset( void );             
void dtvd_tuner_tuner_com_reset( void );                    

void dtvd_tuner_tuner_inevt_core_pwron( void );
void dtvd_tuner_tuner_inevt_core_pwroff( void );
void dtvd_tuner_tuner_inevt_core_init( void );
void dtvd_tuner_tuner_inevt_core_end( void );
void dtvd_tuner_tuner_inevt_core_tune_set( unsigned int );
void dtvd_tuner_tuner_inevt_core_tmcc( void );
void dtvd_tuner_tuner_inevt_core_tsout( void );
void dtvd_tuner_tuner_inevt_core_tune_ng( unsigned char );
void dtvd_tuner_tuner_inevt_core_nostd( unsigned char );
void dtvd_tuner_tuner_inevt_core_sync( void );
void dtvd_tuner_tuner_inevt_core_async( void );
void dtvd_tuner_tuner_inevt_core_stop( void );
void dtvd_tuner_tuner_inevt_core_deverr( void );
void dtvd_tuner_tuner_inevt_tmcc_tuner_init( void );
void dtvd_tuner_tuner_inevt_tmcc_tuner_end( void );
void dtvd_tuner_tuner_inevt_tmcc_infoget( void );
void dtvd_tuner_tuner_inevt_tmcc_stop( void );
void dtvd_tuner_tuner_inevt_core_init_omt( void );
void dtvd_tuner_tuner_inevt_core_stop_omt( void );
void dtvd_tuner_tuner_inevt_core_deverr_omt( void );
void dtvd_tuner_tuner_inevt_state_tuner_init( void );
void dtvd_tuner_tuner_inevt_state_tuner_end( void );
void dtvd_tuner_tuner_inevt_state_tuner_start( void );
void dtvd_tuner_tuner_inevt_state_tuner_stop( void );
void dtvd_tuner_tuner_inevt_state_tuner_confirm( unsigned char );

extern DTVD_TUNER_TUNER_INFO_t tdtvd_tuner_tuner;

extern const DTVD_TUNER_COM_I2C_DATA_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_ofdm_init_tbl[];

extern const DTVD_TUNER_COM_I2C_DATA_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_iffreq_init_tbl[];

extern const DTVD_TUNER_COM_I2C_DATA_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_vaint_factor_clear_tbl[];

extern const DTVD_TUNER_COM_I2C_DATA_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_rf_init_tbl[];

extern const unsigned char dtvd_tuner_rf_tune_tbl[][ D_DTVD_TUNER_RF_CH_TBL_SIZE ];  

#endif 

