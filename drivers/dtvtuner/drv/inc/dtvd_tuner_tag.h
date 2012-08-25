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

#ifndef _DTVD_TUNER_TAG_H_
#define _DTVD_TUNER_TAG_H_

typedef struct _DTVD_TUNER_COM_SYSERR_t {
    unsigned char   file_no;        
    unsigned char   reserved[3];    
    unsigned long   line;           
    unsigned long   log_data[6];    
} DTVD_TUNER_COM_SYSERR_t;

typedef struct _DTVD_TUNER_CONTROL_t {
    unsigned int                count;              
    atomic_t                    event_flg;          
    wait_queue_head_t           wait_queue;         
    volatile unsigned long      block_flg;          
    wait_queue_head_t           wait_queue_block;   
    signed int                  result;             
    signed int                  omt_tune_result;    

    struct i2c_adapter          *adap;              

} DTVD_TUNER_CONTROL_t;

typedef struct _DTVD_TUNER_COM_I2C_DATA_t
{
    unsigned char adr;                                  
    unsigned char data;                                 
    unsigned char reserved[2];                          
} DTVD_TUNER_COM_I2C_DATA_t;

typedef struct _DTVD_TUNER_COM_NONVOLATILE_t {
    unsigned short  wait_tmcc;                  
    unsigned short  cn_wait;                    
    unsigned short  wait_ts;                    
    unsigned short  cn_ave_count;               
    unsigned char   cn_cycle;                   
    unsigned char   cn_alpha;                   
    unsigned char   cn_beta;                    
    unsigned char   cn_gamma;                   
    unsigned char   agc_wait;                   
    unsigned char   agc_cycle;                  
    unsigned char   ber_wait;                   
    unsigned char   ber_cycle;                  
    unsigned char   state_monitor_cycle;        
    unsigned char   wait_search_int;            
    unsigned char   wait_mnt_sync;              
    unsigned char   omt_agc_thr;                
    unsigned char   auto_n2e_ifagc_qp12;        
    unsigned char   auto_n2e_ifagc_qp23;        
    unsigned char   auto_n2e_ifagc_qam12;       
    unsigned char   auto_e2n_ifagc_qp12;        
    unsigned char   auto_e2n_ifagc_qp23;        
    unsigned char   auto_e2n_ifagc_qam12;       

    unsigned char   wait_refclk;                

    unsigned char   search_thres;               

    unsigned char   wait_lock1;                 
    unsigned char   wait_lock2;                 
    unsigned char   reserved4;                  
    unsigned char   reserved5;                  
    unsigned int    cn_qpsk1_2_m;               
    unsigned int    cn_qpsk1_2_h;               
    unsigned int    cn_qpsk2_3_m;               
    unsigned int    cn_qpsk2_3_h;               
    unsigned int    cn_16qam1_2_m;              
    unsigned int    cn_16qam1_2_h;              
    unsigned int    cn_qpsk1_2_eco_m;           
    unsigned int    cn_qpsk1_2_eco_h;           
    unsigned int    cn_qpsk2_3_eco_m;           
    unsigned int    cn_qpsk2_3_eco_h;           
    unsigned int    cn_16qam1_2_eco_m;          
    unsigned int    cn_16qam1_2_eco_h;          
    unsigned char   mode_on;                    
    unsigned char   tmcc_on;                    
    unsigned char   mode_dec_count;             
    unsigned char   search_success_count;       
    unsigned char   search_success_thr;         
    unsigned char   search_time;                
    unsigned char   scan_mode_dec_count;        
    unsigned char   scan_search_success_count;  
    unsigned char   reserved10;                 
    unsigned char   reserved11;                 
    unsigned char   reserved12;                 
    unsigned char   reserved13;                 
    unsigned char   reserved14;                 
    unsigned char   reserved15;                 
    unsigned char   reserved16;                 
    unsigned char   reserved17;                 
    unsigned int    auto_n2e_cn_qp12;           
    unsigned int    auto_n2e_cn_qp23;           
    unsigned int    auto_n2e_cn_qam12;          
    unsigned int    auto_e2n_cn_qp12;           
    unsigned int    auto_e2n_cn_qp23;           
    unsigned int    auto_e2n_cn_qam12;          
#if 0 
    unsigned char   reserved20;                 
    unsigned char   reserved21;                 
    unsigned char   reserved22;                 
    unsigned char   reserved23;                 
    unsigned char   reserved24;                 
    unsigned char   reserved25;                 
    unsigned char   reserved26;                 
    unsigned char   reserved27;                 
#endif
} DTVD_TUNER_COM_NONVOLATILE_t;

#endif 

