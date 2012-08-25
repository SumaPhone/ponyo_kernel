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

#ifndef _DTVTUNER_H_
#define _DTVTUNER_H_

#define D_DTVD_TUNER_OK             0                   
#define D_DTVD_TUNER_NG             (-1)                

#define D_DTVD_TUNER_CHANNEL_MIN    13              
#define D_DTVD_TUNER_CHANNEL_MAX    62              

#define D_DTVD_TUNER_IOC_MAGIC          0xE6
#define D_DTVD_TUNER_REQ_PWRON          _IO(D_DTVD_TUNER_IOC_MAGIC, 0)  
#define D_DTVD_TUNER_REQ_PWROFF         _IO(D_DTVD_TUNER_IOC_MAGIC, 1)  
#define D_DTVD_TUNER_REQ_INIT           _IO(D_DTVD_TUNER_IOC_MAGIC, 2)  
#define D_DTVD_TUNER_REQ_END            _IO(D_DTVD_TUNER_IOC_MAGIC, 3)  
#define D_DTVD_TUNER_REQ_TUNE_DTV       _IO(D_DTVD_TUNER_IOC_MAGIC, 4)  
#define D_DTVD_TUNER_REQ_STOP           _IO(D_DTVD_TUNER_IOC_MAGIC, 5)  
#define D_DTVD_TUNER_REQ_GET_CN         _IO(D_DTVD_TUNER_IOC_MAGIC, 6)  
#define D_DTVD_TUNER_REQ_GET_INFO       _IO(D_DTVD_TUNER_IOC_MAGIC, 7)  
#define D_DTVD_TUNER_REQ_SET_STYLE      _IO(D_DTVD_TUNER_IOC_MAGIC, 8)  
#define D_DTVD_TUNER_REQ_SET_ECO        _IO(D_DTVD_TUNER_IOC_MAGIC, 9)  
#define D_DTVD_TUNER_REQ_INIT_OMT       _IO(D_DTVD_TUNER_IOC_MAGIC, 20) 
#define D_DTVD_TUNER_REQ_STOP_OMT       _IO(D_DTVD_TUNER_IOC_MAGIC, 21) 
#define D_DTVD_TUNER_REQ_TUNE_DTV_OMT   _IO(D_DTVD_TUNER_IOC_MAGIC, 22) 
#define D_DTVD_TUNER_REQ_BER_START_OMT  _IO(D_DTVD_TUNER_IOC_MAGIC, 23) 
#define D_DTVD_TUNER_REQ_BER_STOP_OMT   _IO(D_DTVD_TUNER_IOC_MAGIC, 24) 
#define D_DTVD_TUNER_REQ_BER_GET_OMT    _IO(D_DTVD_TUNER_IOC_MAGIC, 25) 
#define D_DTVD_TUNER_REQ_CNR_START_OMT  _IO(D_DTVD_TUNER_IOC_MAGIC, 26) 
#define D_DTVD_TUNER_REQ_CNR_STOP_OMT   _IO(D_DTVD_TUNER_IOC_MAGIC, 27) 
#define D_DTVD_TUNER_REQ_CNR_GET_OMT    _IO(D_DTVD_TUNER_IOC_MAGIC, 28) 
#define D_DTVD_TUNER_REQ_AGC_START_OMT  _IO(D_DTVD_TUNER_IOC_MAGIC, 29) 
#define D_DTVD_TUNER_REQ_AGC_STOP_OMT   _IO(D_DTVD_TUNER_IOC_MAGIC, 30) 
#define D_DTVD_TUNER_REQ_AGC_GET_OMT    _IO(D_DTVD_TUNER_IOC_MAGIC, 31) 
#define D_DTVD_TUNER_REQ_ANT_SET_OMT    _IO(D_DTVD_TUNER_IOC_MAGIC, 32) 

#define D_DTVD_TUNER_CIT_TEST_SBER			100
#define D_DTVD_TUNER_CIT_TEST_CN				101
#define D_DTVD_TUNER_CIT_TEST_RSSI				102
#define D_DTVD_TUNER_CIT_TEST_REGLIST			103
#define D_DTVD_TUNER_CIT_TEST_CHANNELSCAN	104
#define D_DTVD_TUNER_CIT_TEST_VBER			105
#define D_DTVD_TUNER_CIT_TEST_MER				106

#define D_DTVD_TUNER_CALLMODE_NORMAL    0   
#define D_DTVD_TUNER_CALLMODE_RETRY     1   

#define D_DTVD_TUNER_TSMODE_NONE        0   
#define D_DTVD_TUNER_TSMODE_MODE2       2   
#define D_DTVD_TUNER_TSMODE_MODE3       3   

#define D_DTVD_TUNER_GI_NONE            0   
#define D_DTVD_TUNER_GI_1_16            1   
#define D_DTVD_TUNER_GI_1_8             2   
#define D_DTVD_TUNER_GI_1_4             3   

#define D_DTVD_TUNER_TUNE_KIND_NORMAL   0   
#define D_DTVD_TUNER_TUNE_KIND_SEARCH   1   
#define D_DTVD_TUNER_TUNE_KIND_SCAN     2   

#define D_DTVD_TUNER_STYLE_NONE         0   
#define D_DTVD_TUNER_STYLE_NS           1   
#define D_DTVD_TUNER_STYLE_CS           2   
#define D_DTVD_TUNER_STYLE_VS           3   
#define D_DTVD_TUNER_STYLE_LLS          D_DTVD_TUNER_STYLE_NS   
#define D_DTVD_TUNER_STYLE_LRS          D_DTVD_TUNER_STYLE_NS   

#define D_DTVD_TUNER_STYLE_SW           D_DTVD_TUNER_STYLE_NS   

#define D_DTVD_TUNER_DTVTUNE_SYNC_OFF   0   
#define D_DTVD_TUNER_DTVTUNE_SYNC_ON    1   

#define D_DTVD_TUNER_BER_MEASURE_OUT_OFF    0   
#define D_DTVD_TUNER_BER_MEASURE_OUT_ON     1   

#define D_DTVD_TUNER_BER_MEASURE_BERLEN_1       0               
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_2       1               
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_4       2               
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_8       3               
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_16      4               
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_32      5               
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_64      6               
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_128     7               
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_256     8               
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_512     9               
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_1024    10              
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_2048    11              
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_4096    12              
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_8192    13              

#define D_DTVD_TUNER_BER_MEASURE_BERLEN_16384   14              
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_32768   15              
#define D_DTVD_TUNER_BER_MEASURE_BERLEN_MAX     D_DTVD_TUNER_BER_MEASURE_BERLEN_32768   

#define D_DTVD_TUNER_BER_MEASURE_PACKET( a )    ( 0x0001 << (a) ) 

#define D_DTVD_TUNER_CN_MEASURE_CNWIN_0     0   
#define D_DTVD_TUNER_CN_MEASURE_CNWIN_1     1   
#define D_DTVD_TUNER_CN_MEASURE_CNWIN_2     2   
#define D_DTVD_TUNER_CN_MEASURE_CNWIN_3     3   
#define D_DTVD_TUNER_CN_MEASURE_CNWIN_4     4   
#define D_DTVD_TUNER_CN_MEASURE_CNWIN_5     5   
#define D_DTVD_TUNER_CN_MEASURE_CNWIN_6     6   
#define D_DTVD_TUNER_CN_MEASURE_CNWIN_7     7   
#define D_DTVD_TUNER_CN_MEASURE_CNWIN_8     D_DTVD_TUNER_CN_MEASURE_CNWIN_7   

#define D_DTVD_TUNER_MSGID_TUNE_OK      1   
#define D_DTVD_TUNER_MSGID_TUNE_NG      2   
#define D_DTVD_TUNER_MSGID_CN_OK        3   
#define D_DTVD_TUNER_MSGID_SYNC         4   
#define D_DTVD_TUNER_MSGID_ASYNC        5   
#define D_DTVD_TUNER_MSGID_DEVERR       6   

#define D_DTVD_TUNER_DEVERR_HARD        0   
#define D_DTVD_TUNER_DEVERR_TSPOVERFLOW 1   
#define D_DTVD_TUNER_DEVERR_SYSERRRANKA 2   
#define D_DTVD_TUNER_DEVERR_SYSERRRANKB 3   

#define D_DTVD_TUNER_BLOCK_ID_TUNERDD   6   

#define D_DTVD_TUNER_BLOCK_ID_DMM       1   

#define D_DTVD_TUNER_TUNE_NG_NONE       0   
#define D_DTVD_TUNER_TUNE_NG_CAUSE1     1   
#define D_DTVD_TUNER_TUNE_NG_CAUSE2     2    
#define D_DTVD_TUNER_TUNE_NG_CAUSE3     3   
#define D_DTVD_TUNER_TUNE_NG_CAUSE4     4   
#define D_DTVD_TUNER_TUNE_NG_CAUSE5     5   
#define D_DTVD_TUNER_TUNE_NG_CAUSE6     6   
#define D_DTVD_TUNER_TUNE_NG_CAUSE7     7   
#define D_DTVD_TUNER_TUNE_NG_CAUSE8     8   
#define D_DTVD_TUNER_TUNE_NG_CAUSE9     9   

#define D_DTVD_TUNER_GET_RESULT_OK          0       
#define D_DTVD_TUNER_GET_RESULT_NG_NONE     1       
#define D_DTVD_TUNER_GET_RESULT_NG_INSUFF   2        

#define D_DTVD_TUNER_SYNC_STATE_ASYNC   0   
#define D_DTVD_TUNER_SYNC_STATE_SYNC    1   

#define D_DTVD_TUNER_RX_LEVEL_OUT       0   
#define D_DTVD_TUNER_RX_LEVEL_LOW       1   
#define D_DTVD_TUNER_RX_LEVEL_MDL       2   
#define D_DTVD_TUNER_RX_LEVEL_HIGH      3   

#define D_DTVD_TUNER_TMCC_MODULATION_DQPSK      0   
#define D_DTVD_TUNER_TMCC_MODULATION_QPSK       1   
#define D_DTVD_TUNER_TMCC_MODULATION_16QAM      2   
#define D_DTVD_TUNER_TMCC_MODULATION_64QAM      3   
#define D_DTVD_TUNER_TMCC_MODULATION_RSV1       4   
#define D_DTVD_TUNER_TMCC_MODULATION_RSV2       5   
#define D_DTVD_TUNER_TMCC_MODULATION_RSV3       6   
#define D_DTVD_TUNER_TMCC_MODULATION_NONE       7   

#define D_DTVD_TUNER_TMCC_CODERATE_1_2          0   
#define D_DTVD_TUNER_TMCC_CODERATE_2_3          1   
#define D_DTVD_TUNER_TMCC_CODERATE_3_4          2   
#define D_DTVD_TUNER_TMCC_CODERATE_5_6          3   
#define D_DTVD_TUNER_TMCC_CODERATE_7_8          4   
#define D_DTVD_TUNER_TMCC_CODERATE_RSV1         5   
#define D_DTVD_TUNER_TMCC_CODERATE_RSV2         6   
#define D_DTVD_TUNER_TMCC_CODERATE_NONE         7   

#define D_DTVD_TUNER_TMCC_INTERLEAVE_0          0   
#define D_DTVD_TUNER_TMCC_INTERLEAVE_1          1   
#define D_DTVD_TUNER_TMCC_INTERLEAVE_2          2   
#define D_DTVD_TUNER_TMCC_INTERLEAVE_3          3   

#define D_DTVD_TUNER_MEASURE_STATE_OFF          0   
#define D_DTVD_TUNER_MEASURE_STATE_ON           1   
#define D_DTVD_TUNER_MEASURE_STATE_ON_X         2   
#define D_DTVD_TUNER_MEASURE_STATE_ON_Y         3   

#define D_DTVD_TUNER_TMCC_NOERR         0   
#define D_DTVD_TUNER_TMCC_ERROR         1   

#define D_DTVD_TUNER_END_KIND_NORNAL    0   
#define D_DTVD_TUNER_END_KIND_RETRY     1   

#define D_DTVD_TUNER_ECO_MODE_OFF       0   
#define D_DTVD_TUNER_ECO_MODE_ON        1   

#define D_DTVD_TUNER_ANT_KIND_WHP       0   
#define D_DTVD_TUNER_ANT_KIND_DPL       1   
#define D_DTVD_TUNER_ANT_KIND_HSJ       2   

typedef struct _DTVD_TUNER_CHANNEL_t {
    unsigned char   no;             
    unsigned char   seg;            
    unsigned char   reserved[2];    
} DTVD_TUNER_CHANNEL_t;

typedef struct _DTVD_TUNER_ADJUST_t {
    unsigned char   mode;           
    unsigned char   gi;             
    unsigned char   reserved[2];    
} DTVD_TUNER_ADJUST_t;

typedef struct _DTVD_TUNER_MONITOR_RFIC_t {
    unsigned char   ant;            
    unsigned char   power;          
    unsigned char   reserved[2];    
} DTVD_TUNER_MONITOR_RFIC_t;

typedef struct _DTVD_TUNER_MONITOR_TUNE_t {
    DTVD_TUNER_CHANNEL_t    ch;         
    DTVD_TUNER_ADJUST_t     adj;        
    unsigned char           sync_state; 
    unsigned char           reserved[3];

    volatile unsigned long   sync_time; 
} DTVD_TUNER_MONITOR_TUNE_t;

typedef struct _DTVD_TUNER_TMCCD_TRANS_PARAM_t {
    unsigned char   modulation;     
    unsigned char   coderate;       
    unsigned char   interleave;     
    unsigned char   seg;            
} DTVD_TUNER_TMCCD_TRANS_PARAM_t;

typedef struct _DTVD_TUNER_TMCC_DATA_t {
    unsigned char                   part;           
    unsigned char                   reserved[3];    
    DTVD_TUNER_TMCCD_TRANS_PARAM_t  layer_a;        
    DTVD_TUNER_TMCCD_TRANS_PARAM_t  layer_b;        
} DTVD_TUNER_TMCC_DATA_t;

typedef struct _DTVD_TUNER_TMCC_INFO_t {
    unsigned char           system;     
    unsigned char           cntdwn;     
    unsigned char           emgflg;     
    unsigned char           reserved;   
    DTVD_TUNER_TMCC_DATA_t  curr;       
    DTVD_TUNER_TMCC_DATA_t  next;       
} DTVD_TUNER_TMCC_INFO_t;

typedef struct _DTVD_TUNER_MONITOR_TMCC_t {
    unsigned char           error;          
    unsigned char           nonstd;         
    unsigned char           reserved[2];    
    DTVD_TUNER_TMCC_INFO_t  info;           
} DTVD_TUNER_MONITOR_TMCC_t;

typedef struct _DTVD_TUNER_MEASURE_AGC_t {
    unsigned char   state;      
    unsigned char   value_x;    
    unsigned char   value_y;    
    unsigned char   reserved;   
} DTVD_TUNER_MEASURE_AGC_t;

typedef struct _DTVD_TUNER_MEASURE_VALUE_t {
    unsigned char   result;         
    unsigned char   up;             
    unsigned char   low;            
    unsigned char   ext;             
} DTVD_TUNER_MEASURE_VALUE_t;

typedef struct _DTVD_TUNER_MEASURE_CNA_t {
    unsigned char               state;          
    unsigned char               reserved[3];    
    DTVD_TUNER_MEASURE_VALUE_t  value_x;        

    DTVD_TUNER_MEASURE_VALUE_t  value_y;        

    DTVD_TUNER_MEASURE_VALUE_t  value_comp;     

} DTVD_TUNER_MEASURE_CNA_t;

typedef struct _DTVD_TUNER_MEASURE_BER_t {
    unsigned char               state;          
    unsigned char               measure_type;   
    unsigned char               reserved[2];    
    DTVD_TUNER_MEASURE_VALUE_t  value;          
} DTVD_TUNER_MEASURE_BER_t;

typedef struct _DTVD_TUNER_MONITOR_RX_t {
    DTVD_TUNER_MEASURE_AGC_t agc;   
    DTVD_TUNER_MEASURE_CNA_t cna;   
    DTVD_TUNER_MEASURE_BER_t ber;   
} DTVD_TUNER_MONITOR_RX_t;

typedef struct _DTVD_TUNER_MONITOR_INFO_t {
    unsigned char               ant_ext_state;  
    unsigned char               diver;          
    unsigned char               reserved[2];    
    DTVD_TUNER_MONITOR_RFIC_t   rfic_main;      
    DTVD_TUNER_MONITOR_RFIC_t   rfic_sub;       
    DTVD_TUNER_MONITOR_TUNE_t   tune;           
    DTVD_TUNER_MONITOR_TMCC_t   tmcc;           
    DTVD_TUNER_MONITOR_RX_t     rx;             
} DTVD_TUNER_MONITOR_INFO_t;

typedef struct _DTVD_TUNER_REQ_PWRON_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_REQ_PWRON_t;

typedef struct _DTVD_TUNER_REQ_INIT_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    

    unsigned char   pipename[256];  

} DTVD_TUNER_REQ_INIT_t;

typedef struct _DTVD_TUNER_REQ_TUNE_t {
    unsigned long           seq_id;         
    DTVD_TUNER_CHANNEL_t    ch;             
    DTVD_TUNER_ADJUST_t     adj;            
    unsigned char           kind;           
    unsigned char           mode;           
    unsigned char           reserved[2];    
} DTVD_TUNER_REQ_TUNE_t;

typedef struct _DTVD_TUNER_REQ_STOP_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_REQ_STOP_t;

typedef struct _DTVD_TUNER_REQ_END_t {
    unsigned char   mode;           
    unsigned char   kind;           
    unsigned char   reserved[2];    
} DTVD_TUNER_REQ_END_t;

typedef struct _DTVD_TUNER_REQ_PWROFF_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_REQ_PWROFF_t;

typedef struct _DTVD_TUNER_REQ_GET_CN_t {
    signed int      result;         
    unsigned char   rx_level;       
    unsigned char   reserved[3];    
} DTVD_TUNER_REQ_GET_CN_t;

typedef struct _DTVD_TUNER_REQ_GET_INFO_t {
    DTVD_TUNER_MONITOR_INFO_t   monitor;    
} DTVD_TUNER_REQ_GET_INFO_t;

typedef struct _DTVD_TUNER_REQ_SET_STYLE_t {
    unsigned int    style;      
} DTVD_TUNER_REQ_SET_STYLE_t;

typedef struct _DTVD_TUNER_REQ_SET_ECO_t {
   unsigned char       eco_mode;       
   unsigned char       reserved[3];    
} DTVD_TUNER_REQ_SET_ECO_t;

typedef struct _DTVD_TUNER_REQ_INIT_OMT_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_REQ_INIT_OMT_t;

typedef struct _DTVD_TUNER_REQ_STOP_OMT_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_REQ_STOP_OMT_t;

typedef struct _DTVD_TUNER_REQ_TUNE_DTV_OMT_t {
    unsigned char       mode;           
    unsigned char       ch_no;          
    unsigned char       sync;           
    unsigned char       reserved[2];    
    signed int          result;         
    DTVD_TUNER_ADJUST_t adj;            
} DTVD_TUNER_REQ_TUNE_DTV_OMT_t;

typedef struct _DTVD_TUNER_REQ_BER_START_OMT_t {
    unsigned char   mode;       
    unsigned char   out;        
    unsigned short  period;     
} DTVD_TUNER_REQ_BER_START_OMT_t;

typedef struct _DTVD_TUNER_REQ_BER_STOP_OMT_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_REQ_BER_STOP_OMT_t;

typedef struct _DTVD_TUNER_REQ_BER_GET_OMT_t {
    unsigned char               mode;           
    unsigned char               reserved[3];    
    DTVD_TUNER_MEASURE_VALUE_t  value;          
} DTVD_TUNER_REQ_BER_GET_OMT_t;

typedef struct _DTVD_TUNER_REQ_CNR_START_OMT_t {
    unsigned char   mode;           
    unsigned char   carrier;        
    unsigned char   reserved[2];    
} DTVD_TUNER_REQ_CNR_START_OMT_t;

typedef struct _DTVD_TUNER_REQ_CNR_STOP_OMT_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_REQ_CNR_STOP_OMT_t;

typedef struct _DTVD_TUNER_REQ_CNR_GET_OMT_t {
    unsigned char               mode;           
    unsigned char               reserved[3];    
    DTVD_TUNER_MEASURE_VALUE_t  value_x;        
    DTVD_TUNER_MEASURE_VALUE_t  value_y;        
    DTVD_TUNER_MEASURE_VALUE_t  value_comp;     
} DTVD_TUNER_REQ_CNR_GET_OMT_t;

typedef struct _DTVD_TUNER_REQ_AGC_START_OMT_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_REQ_AGC_START_OMT_t;

typedef struct _DTVD_TUNER_REQ_AGC_STOP_OMT_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_REQ_AGC_STOP_OMT_t;

typedef struct _DTVD_TUNER_REQ_AGC_GET_OMT_t {
    unsigned char   mode;       
    unsigned char   value_x;    
    unsigned char   value_y;    
    unsigned char   reserved;   
} DTVD_TUNER_REQ_AGC_GET_OMT_t;

typedef struct DTVD_TUNER_REQ_ANT_SET_OMT_t {
    unsigned char   mode;           
    unsigned char   ant;            
    unsigned char   reserved[2];    
} DTVD_TUNER_REQ_ANT_SET_OMT_t;

typedef struct _DTVD_TUNER_MSGDATA_TUNE_OK_t {
    unsigned long           seq_id;         
    DTVD_TUNER_ADJUST_t     adj;            
    unsigned char           agc;            
    unsigned char           reserved[3];    
} DTVD_TUNER_MSGDATA_TUNE_OK_t;

typedef struct _DTVD_TUNER_MSGDATA_TUNE_NG_t {
    unsigned long           seq_id;         
    unsigned char           cause;          
    unsigned char           reserved[3];    
} DTVD_TUNER_MSGDATA_TUNE_NG_t;

typedef struct _DTVD_TUNER_MSGDATA_SYNC_t {
    DTVD_TUNER_ADJUST_t     adj;            
} DTVD_TUNER_MSGDATA_SYNC_t;

typedef struct _DTVD_TUNER_MSGDATA_ASYNC_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_MSGDATA_ASYNC_t;

typedef struct _DTVD_TUNER_MSGDATA_CN_OK_t {
    unsigned char           rx_level;       
    unsigned char           reserved[3];    
} DTVD_TUNER_MSGDATA_CN_OK_t;

typedef struct _DTVD_TUNER_MSGDATA_DEVERR_t {
    unsigned char           error_type;     
    unsigned char           reserved[3];    
} DTVD_TUNER_MSGDATA_DEVERR_t;

typedef struct _DTVD_TUNER_MSG_HEADER_t {
    unsigned short  msg_id;         
    unsigned char   receiver_id;    
    unsigned char   sender_id;      
} DTVD_TUNER_MSG_HEADER_t;

    typedef union DTVD_TUNER_MSG_DATA_u {
    DTVD_TUNER_MSGDATA_TUNE_OK_t    tune_ok;    
    DTVD_TUNER_MSGDATA_TUNE_NG_t    tune_ng;    
    DTVD_TUNER_MSGDATA_SYNC_t       sync;       
    DTVD_TUNER_MSGDATA_ASYNC_t      async;      
    DTVD_TUNER_MSGDATA_CN_OK_t      cn_ok;      
    DTVD_TUNER_MSGDATA_DEVERR_t     deverr;     
    unsigned char                   dummy[60];  
} DTVD_TUNER_MSG_DATA_u;

typedef struct _DTVD_TUNER_MSG_t {
    DTVD_TUNER_MSG_HEADER_t     header;     
    DTVD_TUNER_MSG_DATA_u       data;       
} DTVD_TUNER_MSG_t;

#endif 

