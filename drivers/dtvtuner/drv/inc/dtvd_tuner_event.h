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

#ifndef _DTVD_TUNER_EVENT_H_
#define _DTVD_TUNER_EVENT_H_

#define D_DTVD_TUNER_EXEVT_CUE_NUM      16  
#define D_DTVD_TUNER_INEVT_CUE_NUM      8   

#define D_DTVD_TUNER_BLK_UNDEF          0   
#define D_DTVD_TUNER_BLK_MIDDLE         1   
#define D_DTVD_TUNER_BLK_TUNER          2   
#define D_DTVD_TUNER_BLK_TIMER          3   
#define D_DTVD_TUNER_BLK_TUNERDD        4   

#define D_DTVD_TUNER_CTL_UNDEF          0   
#define D_DTVD_TUNER_CTL_CORE           1   
#define D_DTVD_TUNER_CTL_ANT            2   
#define D_DTVD_TUNER_CTL_TUNER          3   
#define D_DTVD_TUNER_CTL_WATCH_AGC      4   
#define D_DTVD_TUNER_CTL_WATCH_TMCC     5   
#define D_DTVD_TUNER_CTL_WATCH_CN       6   
#define D_DTVD_TUNER_CTL_WATCH_BER      7   
#define D_DTVD_TUNER_CTL_WATCH_STATE    8   
#define D_DTVD_TUNER_CTL_AUTOECO        9   

#define D_DTVD_TUNER_CMD_MDL_PWRON          0   
#define D_DTVD_TUNER_CMD_MDL_INIT           1   
#define D_DTVD_TUNER_CMD_MDL_TUNE_DTV       2   
#define D_DTVD_TUNER_CMD_MDL_SCAN_DTV       3   
#define D_DTVD_TUNER_CMD_MDL_STOP           4   
#define D_DTVD_TUNER_CMD_MDL_END            5   
#define D_DTVD_TUNER_CMD_MDL_PWROFF         6   
#define D_DTVD_TUNER_CMD_MDL_STYLE          7   
#define D_DTVD_TUNER_CMD_MDL_INIT_OMT       8   
#define D_DTVD_TUNER_CMD_MDL_STOP_OMT       9   
#define D_DTVD_TUNER_CMD_MDL_TUNE_DTV_OMT   10  
#define D_DTVD_TUNER_CMD_MDL_BER_START_OMT  11  
#define D_DTVD_TUNER_CMD_MDL_BER_STOP_OMT   12  
#define D_DTVD_TUNER_CMD_MDL_BER_GET_OMT    13  
#define D_DTVD_TUNER_CMD_MDL_CNR_START_OMT  14  
#define D_DTVD_TUNER_CMD_MDL_CNR_STOP_OMT   15  
#define D_DTVD_TUNER_CMD_MDL_CNR_GET_OMT    16  
#define D_DTVD_TUNER_CMD_MDL_AGC_START_OMT  17  
#define D_DTVD_TUNER_CMD_MDL_AGC_STOP_OMT   18  
#define D_DTVD_TUNER_CMD_MDL_AGC_GET_OMT    19  
#define D_DTVD_TUNER_CMD_MDL_ECO_SET        20  
#define D_DTVD_TUNER_CMD_MDL_ANT_SET        21  

#define D_DTVD_TUNER_CMD_TUNER_INT          0   

#define D_DTVD_TUNER_CMD_TIMER_TIMEOUT      0   

#define D_DTVD_TUNER_CMD_TIMER_PWR_HALFTIMEOUT 1   

#define D_DTVD_TUNER_CMD_TUNERDD_TUNEOK         0   
#define D_DTVD_TUNER_CMD_TUNERDD_STATE_MODE_OK  1   
#define D_DTVD_TUNER_CMD_TUNERDD_STATE_SYNC     2   
#define D_DTVD_TUNER_CMD_TUNERDD_STATE_TMCC_OK  3   
#define D_DTVD_TUNER_CMD_TUNERDD_STATE_TS_START 4   
#define D_DTVD_TUNER_CMD_TUNERDD_STATE_ASYNC    5   

#define D_DTVD_TUNER_NOEVENT                            0 
#define D_DTVD_TUNER_EVENT_NOP                          1 

#define D_DTVD_TUNER_EXEVT_MDL_PWRON                2   
#define D_DTVD_TUNER_EXEVT_MDL_INIT                 3   
#define D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV             4   
#define D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV             5   
#define D_DTVD_TUNER_EXEVT_MDL_STOP                 6   
#define D_DTVD_TUNER_EXEVT_MDL_END                  7   
#define D_DTVD_TUNER_EXEVT_MDL_PWROFF               8   
#define D_DTVD_TUNER_EXEVT_MDL_ECO_SET              9   
#define D_DTVD_TUNER_EXEVT_MDL_STYLE                10  
#define D_DTVD_TUNER_EXEVT_TUNER_INT                11  
#define D_DTVD_TUNER_EXEVT_TUNERDD_PRE              12  
#define D_DTVD_TUNER_EXEVT_TUNERDD_STATE_MODE_OK    13  
#define D_DTVD_TUNER_EXEVT_TUNERDD_STATE_SYNC       14  
#define D_DTVD_TUNER_EXEVT_TUNERDD_STATE_TMCC_OK    15  
#define D_DTVD_TUNER_EXEVT_TUNERDD_STATE_TS_START   16  
#define D_DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC      17  
#define D_DTVD_TUNER_EXEVT_TIMER_PWRON              18  
#define D_DTVD_TUNER_EXEVT_TIMER_SEARCH             19  
#define D_DTVD_TUNER_EXEVT_TIMER_SYNC               20  
#define D_DTVD_TUNER_EXEVT_TIMER_TS                 21  
#define D_DTVD_TUNER_EXEVT_TIMER_ECO                22  
#define D_DTVD_TUNER_EXEVT_TIMER_AGC                23  
#define D_DTVD_TUNER_EXEVT_TIMER_AGC_WATCH          24  
#define D_DTVD_TUNER_EXEVT_TIMER_CN_FST             25  
#define D_DTVD_TUNER_EXEVT_TIMER_CN_MEASURE         26  
#define D_DTVD_TUNER_EXEVT_TIMER_CN_WATCH           27  
#define D_DTVD_TUNER_EXEVT_TIMER_BER_FST            28  
#define D_DTVD_TUNER_EXEVT_TIMER_BER_MEASURE        29  
#define D_DTVD_TUNER_EXEVT_TIMER_BER_WATCH          30  
#define D_DTVD_TUNER_EXEVT_TIMER_STATE_WATCH        31  
#define D_DTVD_TUNER_EXEVT_TIMER_TMCC               32  
#define D_DTVD_TUNER_EXEVT_TIMER_TMCC_WATCH         33  
#define D_DTVD_TUNER_EXEVT_MDL_INIT_OMT             34  
#define D_DTVD_TUNER_EXEVT_MDL_STOP_OMT             35  
#define D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT         36  
#define D_DTVD_TUNER_EXEVT_MDL_BER_START_OMT        37  
#define D_DTVD_TUNER_EXEVT_MDL_BER_STOP_OMT         38  
#define D_DTVD_TUNER_EXEVT_MDL_BER_GET_OMT          39  
#define D_DTVD_TUNER_EXEVT_MDL_CNR_START_OMT        40  
#define D_DTVD_TUNER_EXEVT_MDL_CNR_STOP_OMT         41  
#define D_DTVD_TUNER_EXEVT_MDL_CNR_GET_OMT          42  
#define D_DTVD_TUNER_EXEVT_MDL_AGC_START_OMT        43  
#define D_DTVD_TUNER_EXEVT_MDL_AGC_STOP_OMT         44  
#define D_DTVD_TUNER_EXEVT_MDL_AGC_GET_OMT          45  
#define D_DTVD_TUNER_EXEVT_MDL_ANT_SET              46  

#define D_DTVD_TUNER_INEVT_CORE_ANT_INIT            50  
#define D_DTVD_TUNER_INEVT_CORE_ANT_END             51  
#define D_DTVD_TUNER_INEVT_CORE_ANT_INIT_OMT        52  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_PWRON         53  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_PWROFF        54  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_INIT          55  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_END           56  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET      57  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_TMCC          58  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_TSOUT         59  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_TUNE_NG       60  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_NONSTD        61  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_SYNC          62  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_ASYNC         63  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_STOP          64  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR        65  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_INIT_OMT      66  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_STOP_OMT      67  
#define D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT    68  
#define D_DTVD_TUNER_INEVT_CORE_AGC_INIT            69  
#define D_DTVD_TUNER_INEVT_CORE_AGC_END             70  
#define D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST     71  
#define D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE         72  
#define D_DTVD_TUNER_INEVT_CORE_AGC_STOP            73  
#define D_DTVD_TUNER_INEVT_CORE_AGC_DEVERR          74  
#define D_DTVD_TUNER_INEVT_CORE_AGC_INIT_OMT        75  
#define D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT 76  
#define D_DTVD_TUNER_INEVT_CORE_AGC_START_OMT       77  
#define D_DTVD_TUNER_INEVT_CORE_AGC_STOP_OMT        78  
#define D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_OMT     79  
#define D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_STOP_OMT 80 
#define D_DTVD_TUNER_INEVT_CORE_TMCC_DEVERR         81  
#define D_DTVD_TUNER_INEVT_CORE_CN_INIT             82  
#define D_DTVD_TUNER_INEVT_CORE_CN_END              83  
#define D_DTVD_TUNER_INEVT_CORE_CN_MEASURE          84  
#define D_DTVD_TUNER_INEVT_CORE_CN_STOP             85  
#define D_DTVD_TUNER_INEVT_CORE_CN_DEVERR           86  
#define D_DTVD_TUNER_INEVT_CORE_CN_INIT_OMT         87  
#define D_DTVD_TUNER_INEVT_CORE_CN_START_OMT        88  
#define D_DTVD_TUNER_INEVT_CORE_CN_STOP_OMT         89  
#define D_DTVD_TUNER_INEVT_CORE_CN_MEASURE_OMT      90  
#define D_DTVD_TUNER_INEVT_CORE_BER_INIT            91  
#define D_DTVD_TUNER_INEVT_CORE_BER_END             92  
#define D_DTVD_TUNER_INEVT_CORE_BER_STOP            93  
#define D_DTVD_TUNER_INEVT_CORE_BER_DEVERR          94  
#define D_DTVD_TUNER_INEVT_CORE_BER_INIT_OMT        95  
#define D_DTVD_TUNER_INEVT_CORE_BER_START_OMT       96  
#define D_DTVD_TUNER_INEVT_CORE_BER_STOP_OMT        97  
#define D_DTVD_TUNER_INEVT_CORE_BER_MEASURE_OMT     98  
#define D_DTVD_TUNER_INEVT_CORE_STATE_DEVERR        99  
#define D_DTVD_TUNER_INEVT_ANT_CORE_INIT            100 
#define D_DTVD_TUNER_INEVT_ANT_CORE_END             101 
#define D_DTVD_TUNER_INEVT_ANT_CORE_SET             102 
#define D_DTVD_TUNER_INEVT_ANT_CORE_INIT_OMT        103 
#define D_DTVD_TUNER_INEVT_TUNER_CORE_PWRON         104 
#define D_DTVD_TUNER_INEVT_TUNER_CORE_PWROFF        105 
#define D_DTVD_TUNER_INEVT_TUNER_CORE_INIT          106 
#define D_DTVD_TUNER_INEVT_TUNER_CORE_END           107 
#define D_DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV      108 
#define D_DTVD_TUNER_INEVT_TUNER_CORE_STOP          109 
#define D_DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET       110 
#define D_DTVD_TUNER_INEVT_TUNER_CORE_INIT_OMT      111 
#define D_DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_OMT  112 
#define D_DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT      113 
#define D_DTVD_TUNER_INEVT_AGC_CORE_INIT            114 
#define D_DTVD_TUNER_INEVT_AGC_CORE_END             115 
#define D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST     116 
#define D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE         117 
#define D_DTVD_TUNER_INEVT_AGC_CORE_STOP            118 
#define D_DTVD_TUNER_INEVT_AGC_CORE_INIT_OMT        119 
#define D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_OMT 120 
#define D_DTVD_TUNER_INEVT_AGC_CORE_START_OMT       121 
#define D_DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT        122 
#define D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_OMT     123 
#define D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_STOP_OMT 124 
#define D_DTVD_TUNER_INEVT_CN_CORE_INIT             125 
#define D_DTVD_TUNER_INEVT_CN_CORE_END              126 
#define D_DTVD_TUNER_INEVT_CN_CORE_MEASURE          127 
#define D_DTVD_TUNER_INEVT_CN_CORE_STOP             128 
#define D_DTVD_TUNER_INEVT_CN_CORE_SET              129 
#define D_DTVD_TUNER_INEVT_CN_CORE_INIT_OMT         130 
#define D_DTVD_TUNER_INEVT_CN_CORE_START_OMT        131 
#define D_DTVD_TUNER_INEVT_CN_CORE_STOP_OMT         132 
#define D_DTVD_TUNER_INEVT_CN_CORE_MEASURE_OMT      133 
#define D_DTVD_TUNER_INEVT_BER_CORE_INIT            134 
#define D_DTVD_TUNER_INEVT_BER_CORE_END             135 
#define D_DTVD_TUNER_INEVT_BER_CORE_START           136 
#define D_DTVD_TUNER_INEVT_BER_CORE_MEASURE         137 
#define D_DTVD_TUNER_INEVT_BER_CORE_STOP            138 
#define D_DTVD_TUNER_INEVT_BER_CORE_SET             139 
#define D_DTVD_TUNER_INEVT_BER_CORE_INIT_OMT        140 
#define D_DTVD_TUNER_INEVT_BER_CORE_START_OMT       141 
#define D_DTVD_TUNER_INEVT_BER_CORE_STOP_OMT        142 
#define D_DTVD_TUNER_INEVT_BER_CORE_MEASURE_OMT     143 
#define D_DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT  144 
#define D_DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET       145 
#define D_DTVD_TUNER_INEVT_TUNER_TMCC_STOP          146 
#define D_DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM      147 
#define D_DTVD_TUNER_INEVT_STATE_TUNER_INIT         148 
#define D_DTVD_TUNER_INEVT_STATE_TUNER_END          149 
#define D_DTVD_TUNER_INEVT_STATE_TUNER_START        150 
#define D_DTVD_TUNER_INEVT_STATE_TUNER_STOP         151 
#define D_DTVD_TUNER_INEVT_STATE_TUNER_CONFIRM      152 
#define D_DTVD_TUNER_INEVT_TMCC_TUNER_INIT          153 
#define D_DTVD_TUNER_INEVT_TMCC_TUNER_END           154 
#define D_DTVD_TUNER_INEVT_TMCC_TUNER_INFOGET       155 
#define D_DTVD_TUNER_INEVT_TMCC_TUNER_STOP          156 

#define D_DTVD_TUNER_INEVT_AUTOECO_CORE_INIT        157 
#define D_DTVD_TUNER_INEVT_AUTOECO_CORE_END         158 
#define D_DTVD_TUNER_INEVT_AUTOECO_CORE_START       159 
#define D_DTVD_TUNER_INEVT_AUTOECO_CORE_STOP        160 
#define D_DTVD_TUNER_INEVT_AUTOECO_CN_NOTICE        161 
#define D_DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE      162 
#define D_DTVD_TUNER_INEVT_CORE_AUTOECO_DEVERR      163 

#define D_DTVD_TUNER_INEVT_AUTOECO_CORE_START_OMT   164 
#define D_DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_OMT    165 
#define D_DTVD_TUNER_EXEVT_TIMER_AUTOECO_WATCH_OMT  166 

typedef struct _DTVD_TUNER_EXEVT_TIMEOUT_t {
    unsigned long timer_id;            
} DTVD_TUNER_EXEVT_TIMEOUT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_PWRON_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_MDL_PWRON_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_INIT_t {

    struct file     *pipe_handle;   

} DTVD_TUNER_EXEVT_MDL_INIT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_TUNE_DTV_t {
    unsigned long           seq_id;         
    DTVD_TUNER_CHANNEL_t    ch;             
    DTVD_TUNER_ADJUST_t     adj;            
    unsigned char           kind;           
    unsigned char           reserved[3];    
} DTVD_TUNER_EXEVT_MDL_TUNE_DTV_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_SCAN_DTV_t {
    unsigned long           seq_id;         
    DTVD_TUNER_CHANNEL_t    ch;             
} DTVD_TUNER_EXEVT_MDL_SCAN_DTV_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_MDL_STOP_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_END_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_EXEVT_MDL_END_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_PWROFF_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_MDL_PWROFF_t;

typedef struct _DTVD_TUNER_EXEVT_TUNERDD_PRE_t {
    unsigned long           seq_id;         
    DTVD_TUNER_CHANNEL_t    ch;             
    DTVD_TUNER_ADJUST_t     adj;            
    unsigned char           kind;           
    unsigned char           reserved[3];    
} DTVD_TUNER_EXEVT_TUNERDD_PRE_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_INIT_OMT_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_EXEVT_MDL_INIT_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_STOP_OMT_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_EXEVT_MDL_STOP_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT_t {
    unsigned char           ch_no;          
    unsigned char           tune_sync_kind; 
    unsigned char           reserved[2];    
} DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_BER_START_OMT_t {
    unsigned char           out;            
    unsigned short           period;        
    unsigned char           reserved;       
} DTVD_TUNER_EXEVT_MDL_BER_START_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_BER_STOP_OMT_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_EXEVT_MDL_BER_STOP_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_BER_GET_OMT_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_EXEVT_MDL_BER_GET_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_CNR_START_OMT_t {
    unsigned char           carrier;        
    unsigned char           reserved[3];    
} DTVD_TUNER_EXEVT_MDL_CNR_START_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_CNR_STOP_OMT_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_EXEVT_MDL_CNR_STOP_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_CNR_GET_OMT_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_EXEVT_MDL_CNR_GET_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_AGC_START_OMT_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_EXEVT_MDL_AGC_START_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_AGC_STOP_OMT_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_EXEVT_MDL_AGC_STOP_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_AGC_GET_OMT_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_EXEVT_MDL_AGC_GET_OMT_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_ECO_SET_t {
    unsigned char       mode;           
    unsigned char       reserved[3];    
} DTVD_TUNER_EXEVT_MDL_ECO_SET_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_STYLE_t {
    unsigned int    style;          
} DTVD_TUNER_EXEVT_MDL_STYLE_t;

typedef struct _DTVD_TUNER_EXEVT_MDL_ANT_SET_t {
    unsigned char       ant_kind;       
    unsigned char       reserved[3];    
} DTVD_TUNER_EXEVT_MDL_ANT_SET_t;

typedef struct _DTVD_TUNER_EXEVT_TUNER_INT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TUNER_INT_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_PWRON_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_PWRON_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_SEARCH_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_SEARCH_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_SYNC_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_SYNC_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_TS_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_TS_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_TMCCERR_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_TMCCERR_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_ECO_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_ECO_t;

typedef struct _DTVD_TUNER_EXEVT_TUNERDD_STATE_MODE_OK_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TUNERDD_STATE_MODE_OK_t;

typedef struct _DTVD_TUNER_EXEVT_TUNERDD_STATE_SYNC_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TUNERDD_STATE_SYNC_t;

typedef struct _DTVD_TUNER_EXEVT_TUNERDD_STATE_TMCC_OK_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TUNERDD_STATE_TMCC_OK_t;

typedef struct _DTVD_TUNER_EXEVT_TUNERDD_STATE_TS_START_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TUNERDD_STATE_TS_START_t;

typedef struct _DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_AGC_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_AGC_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_AGC_WATCH_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_AGC_WATCH_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_CN_FST_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_CN_FST_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_CN_MEASURE_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_CN_MEASURE_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_CN_WATCH_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_CN_WATCH_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_BER_FST_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_BER_FST_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_BER_MEASURE_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_BER_MEASURE_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_BER_WATCH_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_BER_WATCH_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_TMCC_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_TMCC_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_TMCC_WATCH_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_TMCC_WATCH_t;

typedef struct _DTVD_TUNER_EXEVT_TIMER_STATE_WATCH_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_EXEVT_TIMER_STATE_WATCH_t;

typedef struct _DTVD_TUNER_INEVT_CORE_ANT_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_ANT_INIT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_ANT_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_ANT_END_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_PWRON_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_PWRON_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_PWROFF_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_PWROFF_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_INIT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_END_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET_t {
    unsigned int    result;    
} DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_SYNC_S_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_SYNC_S_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_TMCC_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_TMCC_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_TSOUT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_TSOUT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_TUNE_NG_t {
    unsigned char   cause;          
    unsigned char   reserved[3];    
} DTVD_TUNER_INEVT_CORE_TUNER_TUNE_NG_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_NONSTD_t {
    unsigned char   kind;           
    unsigned char   reserved[3];    
} DTVD_TUNER_INEVT_CORE_TUNER_NONSTD_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_SYNC_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_SYNC_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_ASYNC_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_ASYNC_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_STOP_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_AGC_INIT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_AGC_END_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_t {
    unsigned char   x_val;          
    unsigned char   y_val;          
    unsigned char   reserved[2];    
} DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_MEASURE_t {
    unsigned char   x_val;          
    unsigned char   y_val;          
    unsigned char   reserved[2];    
} DTVD_TUNER_INEVT_CORE_AGC_MEASURE_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_AGC_STOP_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_DEVERR_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_AGC_DEVERR_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TMCC_DEVERR_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TMCC_DEVERR_t;

typedef struct _DTVD_TUNER_INEVT_CORE_CN_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_CN_INIT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_CN_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_CN_END_t;

typedef struct _DTVD_TUNER_INEVT_CORE_CN_MEASURE_t {
    unsigned long seq_id;           
} DTVD_TUNER_INEVT_CORE_CN_MEASURE_t;

typedef struct _DTVD_TUNER_INEVT_CORE_CN_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_CN_STOP_t;

typedef struct _DTVD_TUNER_INEVT_CORE_CN_DEVERR_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_CN_DEVERR_t;

typedef struct _DTVD_TUNER_INEVT_CORE_BER_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_BER_INIT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_BER_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_BER_END_t;

typedef struct _DTVD_TUNER_INEVT_CORE_BER_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_BER_STOP_t;

typedef struct _DTVD_TUNER_INEVT_CORE_BER_DEVERR_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_BER_DEVERR_t;

typedef struct _DTVD_TUNER_INEVT_CORE_STATE_DEVERR_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_STATE_DEVERR_t;

typedef struct _DTVD_TUNER_INEVT_CORE_ANT_INIT_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_ANT_INIT_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_INIT_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_INIT_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_STOP_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_STOP_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_INIT_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_AGC_INIT_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT_t {
    unsigned char   x_val;          
    unsigned char   y_val;          
    unsigned char   reserved[2];    
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_START_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_AGC_START_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_STOP_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_AGC_STOP_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_MEASURE_OMT_t {
    unsigned char   x_val;          
    unsigned char   y_val;          
    unsigned char   reserved[2];    
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_AGC_MEASURE_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AGC_MEASURE_STOP_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_AGC_MEASURE_STOP_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_CN_INIT_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_CN_INIT_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_CN_START_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_CN_START_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_CN_STOP_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_CN_STOP_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_CN_MEASURE_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_CN_MEASURE_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_BER_INIT_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_BER_INIT_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_BER_START_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_BER_START_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_BER_STOP_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_BER_STOP_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_BER_MEASURE_OMT_t {
    signed int      result;         
} DTVD_TUNER_INEVT_CORE_BER_MEASURE_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE_t {
    unsigned char   notice;         
    unsigned char   reserved[3];    
} DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE_t;

typedef struct _DTVD_TUNER_INEVT_CORE_AUTOECO_DEVERR_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CORE_AUTOECO_DEVERR_t;

typedef struct _DTVD_TUNER_INEVT_ANT_CORE_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_ANT_CORE_INIT_t;

typedef struct _DTVD_TUNER_INEVT_ANT_CORE_END_t {
    unsigned char   mode;           
    unsigned char   reserved[3];    
} DTVD_TUNER_INEVT_ANT_CORE_END_t;

typedef struct _DTVD_TUNER_INEVT_ANT_CORE_SET_t {
    unsigned char   ch;             
    unsigned char   reserved[3];    
} DTVD_TUNER_INEVT_ANT_CORE_SET_t;

typedef struct _DTVD_TUNER_INEVT_ANT_CORE_INIT_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_ANT_CORE_INIT_OMT_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_PWRON_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_TUNER_CORE_PWRON_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_PWROFF_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_TUNER_CORE_PWROFF_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_TUNER_CORE_INIT_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_TUNER_CORE_END_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_t {
    DTVD_TUNER_CHANNEL_t    ch;             
    DTVD_TUNER_ADJUST_t     adj;            
    unsigned char           kind;           
    unsigned char           reserved[3];    
} DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_SYNC_S_t {
    unsigned char           hold;           
    unsigned char           reserved[3];    
} DTVD_TUNER_INEVT_TUNER_CORE_SYNC_S_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_STOP_t {
    unsigned char           reserved[4];    
} DTVD_TUNER_INEVT_TUNER_CORE_STOP_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET_t {
    unsigned char                   non_std;        
    unsigned char                   reserved[3];    
} DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_TMCC_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_TUNER_TMCC_STOP_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM_t {
    signed int      result;         
} DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_INIT_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_TUNER_CORE_INIT_OMT_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_OMT_t {
    unsigned char           ch_no;          
    unsigned char           sync;           
    unsigned char           reserved[2];    
} DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_OMT_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT_t;

typedef struct _DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET_t {
    unsigned char       mode;           
    unsigned char       reserved[3];    
} DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_INIT_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_END_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_MEASURE_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_MEASURE_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_STOP_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_INIT_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_INIT_OMT_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_OMT_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_START_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_START_OMT_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_MEASURE_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_MEASURE_OMT_t;

typedef struct _DTVD_TUNER_INEVT_AGC_CORE_MEASURE_STOP_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AGC_CORE_MEASURE_STOP_OMT_t;

typedef struct _DTVD_TUNER_INEVT_TMCC_TUNER_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_TMCC_TUNER_INIT_t;

typedef struct _DTVD_TUNER_INEVT_TMCC_TUNER_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_TMCC_TUNER_END_t;

typedef struct _DTVD_TUNER_INEVT_TMCC_TUNER_INFOGET_t {
    unsigned char       kind;       
} DTVD_TUNER_INEVT_TMCC_TUNER_INFOGET_t;

typedef struct _DTVD_TUNER_INEVT_TMCC_TUNER_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_TMCC_TUNER_STOP_t;

typedef struct _DTVD_TUNER_INEVT_CN_CORE_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CN_CORE_INIT_t;

typedef struct _DTVD_TUNER_INEVT_CN_CORE_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CN_CORE_END_t;

typedef struct _DTVD_TUNER_INEVT_CN_CORE_MEASURE_t {
    unsigned long seq_id;           
} DTVD_TUNER_INEVT_CN_CORE_MEASURE_t;

typedef struct _DTVD_TUNER_INEVT_CN_CORE_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CN_CORE_STOP_t;

typedef struct _DTVD_TUNER_INEVT_CN_CORE_SET_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CN_CORE_SET_t;

typedef struct _DTVD_TUNER_INEVT_CN_CORE_INIT_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CN_CORE_INIT_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CN_CORE_START_OMT_t {
    unsigned char   carrier;        
    unsigned char   reserved[3];    
} DTVD_TUNER_INEVT_CN_CORE_START_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CN_CORE_STOP_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CN_CORE_STOP_OMT_t;

typedef struct _DTVD_TUNER_INEVT_CN_CORE_MEASURE_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_CN_CORE_MEASURE_OMT_t;

typedef struct _DTVD_TUNER_INEVT_BER_CORE_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_BER_CORE_INIT_t;

typedef struct _DTVD_TUNER_INEVT_BER_CORE_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_BER_CORE_END_t;

typedef struct _DTVD_TUNER_INEVT_BER_CORE_START_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_BER_CORE_START_t;

typedef struct _DTVD_TUNER_INEVT_BER_CORE_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_BER_CORE_STOP_t;

typedef struct _DTVD_TUNER_INEVT_BER_CORE_SET_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_BER_CORE_SET_t;

typedef struct _DTVD_TUNER_INEVT_BER_CORE_INIT_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_BER_CORE_INIT_OMT_t;

typedef struct _DTVD_TUNER_INEVT_BER_CORE_START_OMT_t {
    unsigned char   out;            
    unsigned short  period;         
    unsigned char   reserved;       
} DTVD_TUNER_INEVT_BER_CORE_START_OMT_t;

typedef struct _DTVD_TUNER_INEVT_BER_CORE_STOP_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_BER_CORE_STOP_OMT_t;

typedef struct _DTVD_TUNER_INEVT_BER_CORE_MEASURE_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_BER_CORE_MEASURE_OMT_t;

typedef struct _DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT_t;

typedef struct _DTVD_TUNER_INEVT_STATE_TUNER_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_STATE_TUNER_INIT_t;

typedef struct _DTVD_TUNER_INEVT_STATE_TUNER_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_STATE_TUNER_END_t;

typedef struct _DTVD_TUNER_INEVT_STATE_TUNER_START_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_STATE_TUNER_START_t;

typedef struct _DTVD_TUNER_INEVT_STATE_TUNER_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_STATE_TUNER_STOP_t;

typedef struct _DTVD_TUNER_INEVT_STATE_TUNER_CONFIRM_t {
    unsigned char   kind;           
    unsigned char   reserved[3];    
} DTVD_TUNER_INEVT_STATE_TUNER_CONFIRM_t;

typedef struct _DTVD_TUNER_INEVT_AUTOECO_CORE_INIT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AUTOECO_CORE_INIT_t;

typedef struct _DTVD_TUNER_INEVT_AUTOECO_CORE_END_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AUTOECO_CORE_END_t;

typedef struct _DTVD_TUNER_INEVT_AUTOECO_CORE_START_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AUTOECO_CORE_START_t;

typedef struct _DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_t;

typedef struct _DTVD_TUNER_INEVT_AUTOECO_CN_NOTICE_t {
    DTVD_TUNER_MEASURE_VALUE_t   cn;     
} DTVD_TUNER_INEVT_AUTOECO_CN_NOTICE_t;

typedef struct DTVD_TUNER_INEVT_AUTOECO_CORE_START_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AUTOECO_CORE_START_OMT_t;

typedef struct DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_OMT_t;

typedef struct DTVD_TUNER_TIMER_AUTOECO_WATCH_OMT_t {
    unsigned char   reserved[4];    
} DTVD_TUNER_TIMER_AUTOECO_WATCH_OMT_t;

typedef union _DTVD_TUNER_MSG_u {
    DTVD_TUNER_EXEVT_MDL_PWRON_t            ex_mdl_powon;           
    DTVD_TUNER_EXEVT_MDL_INIT_t             ex_mdl_init;            
    DTVD_TUNER_EXEVT_MDL_TUNE_DTV_t         ex_mdl_tune_dtv;        
    DTVD_TUNER_EXEVT_MDL_SCAN_DTV_t         ex_mdl_scan_dtv;        
    DTVD_TUNER_EXEVT_MDL_STOP_t             ex_mdl_stop;            
    DTVD_TUNER_EXEVT_MDL_END_t              ex_mdl_end;             
    DTVD_TUNER_EXEVT_MDL_PWROFF_t           ex_mdl_pwroff;          
    DTVD_TUNER_EXEVT_MDL_STYLE_t            ex_mdl_style;           
    DTVD_TUNER_EXEVT_MDL_ECO_SET_t          ex_mdl_eco_set;         
    DTVD_TUNER_EXEVT_TUNERDD_PRE_t          ex_tunerdd_pre;         
    DTVD_TUNER_EXEVT_TUNERDD_STATE_MODE_OK_t    ex_tunerdd_state_mode_ok;   
    DTVD_TUNER_EXEVT_TUNERDD_STATE_SYNC_t       ex_tunerdd_state_sync;      
    DTVD_TUNER_EXEVT_TUNERDD_STATE_TMCC_OK_t    ex_tunerdd_state_tmcc_ok;   
    DTVD_TUNER_EXEVT_TUNERDD_STATE_TS_START_t   ex_tunerdd_state_ts_start;  
    DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC_t  ex_tunerdd_state_async; 
    DTVD_TUNER_EXEVT_TIMEOUT_t              ex_timeout;             
    DTVD_TUNER_EXEVT_TUNER_INT_t            ex_tuner_int;           
    DTVD_TUNER_EXEVT_TIMER_PWRON_t          ex_timer_pwron;         
    DTVD_TUNER_EXEVT_TIMER_SEARCH_t         ex_timer_search;        
    DTVD_TUNER_EXEVT_TIMER_SYNC_t           ex_timer_sync;          
    DTVD_TUNER_EXEVT_TIMER_TS_t             ex_timer_ts;            
    DTVD_TUNER_EXEVT_TIMER_ECO_t            ex_timer_eco;           
    DTVD_TUNER_EXEVT_TIMER_TMCCERR_t        ex_timer_tmccerr;       
    DTVD_TUNER_EXEVT_TIMER_AGC_t            ex_timer_agc;           
    DTVD_TUNER_EXEVT_TIMER_AGC_WATCH_t      ex_timer_agc_watch;     
    DTVD_TUNER_EXEVT_TIMER_CN_FST_t         ex_timer_cn_fst;        
    DTVD_TUNER_EXEVT_TIMER_CN_MEASURE_t     ex_timer_cn_mesure;     
    DTVD_TUNER_EXEVT_TIMER_CN_WATCH_t       ex_timer_cn_watch;      
    DTVD_TUNER_EXEVT_TIMER_BER_FST_t        ex_timer_ber_fst;       
    DTVD_TUNER_EXEVT_TIMER_BER_MEASURE_t    ex_timer_ber_measure;   
    DTVD_TUNER_EXEVT_TIMER_BER_WATCH_t      ex_timer_ber_watch;     
    DTVD_TUNER_EXEVT_TIMER_STATE_WATCH_t    ex_timer_state_watch;   
    DTVD_TUNER_EXEVT_TIMER_TMCC_t           ex_timer_tmcc;          
    DTVD_TUNER_EXEVT_TIMER_TMCC_WATCH_t     ex_timer_tmcc_watch;    
    DTVD_TUNER_EXEVT_MDL_INIT_OMT_t         ex_mdl_init_omt;        
    DTVD_TUNER_EXEVT_MDL_STOP_OMT_t         ex_mdl_stop_omt;        
    DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT_t     ex_mdl_tune_dtv_omt;    
    DTVD_TUNER_EXEVT_MDL_BER_START_OMT_t    ex_mdl_ber_start_omt;   
    DTVD_TUNER_EXEVT_MDL_BER_STOP_OMT_t     ex_mdl_ber_stop_omt;    
    DTVD_TUNER_EXEVT_MDL_BER_GET_OMT_t      ex_mdl_ber_get_omt;     
    DTVD_TUNER_EXEVT_MDL_CNR_START_OMT_t    ex_mdl_cnr_start_omt;   
    DTVD_TUNER_EXEVT_MDL_CNR_STOP_OMT_t     ex_mdl_cnr_stop_omt;    
    DTVD_TUNER_EXEVT_MDL_CNR_GET_OMT_t      ex_mdl_cnr_get_omt;     
    DTVD_TUNER_EXEVT_MDL_AGC_START_OMT_t    ex_mdl_agc_start_omt;   
    DTVD_TUNER_EXEVT_MDL_AGC_STOP_OMT_t     ex_mdl_agc_stop_omt;    
    DTVD_TUNER_EXEVT_MDL_AGC_GET_OMT_t      ex_mdl_agc_get_omt;     
    DTVD_TUNER_EXEVT_MDL_ANT_SET_t          ex_mdl_ant_set;         
    DTVD_TUNER_INEVT_CORE_ANT_INIT_t        in_core_ant_init;       
    DTVD_TUNER_INEVT_CORE_ANT_END_t         in_core_ant_end;        
    DTVD_TUNER_INEVT_CORE_ANT_INIT_OMT_t    in_core_ant_init_omt;   
    DTVD_TUNER_INEVT_CORE_TUNER_PWRON_t     in_core_tuner_pwron;    
    DTVD_TUNER_INEVT_CORE_TUNER_PWROFF_t    in_core_tuner_pwroff;   
    DTVD_TUNER_INEVT_CORE_TUNER_INIT_t      in_core_tuner_init;     
    DTVD_TUNER_INEVT_CORE_TUNER_END_t       in_core_tuner_end;      
    DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET_t  in_core_tuner_tune_set; 
    DTVD_TUNER_INEVT_CORE_TUNER_SYNC_S_t    in_core_tuner_sync_s;   
    DTVD_TUNER_INEVT_CORE_TUNER_TMCC_t      in_core_tuner_tmcc;     
    DTVD_TUNER_INEVT_CORE_TUNER_TSOUT_t     in_core_tuner_tsout;    
    DTVD_TUNER_INEVT_CORE_TUNER_TUNE_NG_t   in_core_tuner_tune_ng;  
    DTVD_TUNER_INEVT_CORE_TUNER_NONSTD_t    in_core_tuner_nonstd;   
    DTVD_TUNER_INEVT_CORE_TUNER_SYNC_t      in_core_tuner_sync;     
    DTVD_TUNER_INEVT_CORE_TUNER_ASYNC_t     in_core_tuner_async;    
    DTVD_TUNER_INEVT_CORE_TUNER_STOP_t      in_core_tuner_st;       
    DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_t    in_core_tuner_deve;     
    DTVD_TUNER_INEVT_CORE_TUNER_INIT_OMT_t  in_core_tuner_init_omt; 
    DTVD_TUNER_INEVT_CORE_TUNER_STOP_OMT_t  in_core_tuner_stop_omt; 
    DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT_t    in_core_tuner_deverr_omt;   
    DTVD_TUNER_INEVT_CORE_AGC_INIT_t        in_core_agc_init;       
    DTVD_TUNER_INEVT_CORE_AGC_END_t         in_core_agc_end;        
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_t in_core_agc_measure_fst;    
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_t     in_core_agc_measure;    
    DTVD_TUNER_INEVT_CORE_AGC_STOP_t        in_core_agc_stop;       
    DTVD_TUNER_INEVT_CORE_AGC_DEVERR_t      in_core_agc_deverr;     
    DTVD_TUNER_INEVT_CORE_AGC_INIT_OMT_t    in_core_agc_init_omt;   
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT_t  in_core_agc_measure_fst_omt;    
    DTVD_TUNER_INEVT_CORE_AGC_START_OMT_t   in_core_agc_start_omt;  
    DTVD_TUNER_INEVT_CORE_AGC_STOP_OMT_t    in_core_agc_stop_omt;   
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_OMT_t in_core_agc_measure_omt;    
    DTVD_TUNER_INEVT_CORE_AGC_MEASURE_STOP_OMT_t    in_core_agc_measure_stop_omt;   
    DTVD_TUNER_INEVT_CORE_TMCC_DEVERR_t     in_core_tmcc_deverr;    
    DTVD_TUNER_INEVT_CORE_CN_INIT_t         in_core_cn_init;        
    DTVD_TUNER_INEVT_CORE_CN_END_t          in_core_cn_end;         
    DTVD_TUNER_INEVT_CORE_CN_MEASURE_t      in_core_cn_measure;     
    DTVD_TUNER_INEVT_CORE_CN_STOP_t         in_core_cn_stop;        
    DTVD_TUNER_INEVT_CORE_CN_DEVERR_t       in_core_cn_deverr;      
    DTVD_TUNER_INEVT_CORE_CN_INIT_OMT_t     in_core_cn_init_omt;    
    DTVD_TUNER_INEVT_CORE_CN_START_OMT_t    in_core_cn_start_omt;   
    DTVD_TUNER_INEVT_CORE_CN_STOP_OMT_t     in_core_cn_stop_omt;    
    DTVD_TUNER_INEVT_CORE_CN_MEASURE_OMT_t  in_core_cn_measure_omt; 
    DTVD_TUNER_INEVT_CORE_BER_INIT_t        in_core_ber_init;       
    DTVD_TUNER_INEVT_CORE_BER_END_t         in_core_ber_end;        
    DTVD_TUNER_INEVT_CORE_BER_STOP_t        in_core_ber_stop;       
    DTVD_TUNER_INEVT_CORE_BER_DEVERR_t      in_core_ber_deverr;     
    DTVD_TUNER_INEVT_CORE_BER_INIT_OMT_t    in_core_ber_init_omt;   
    DTVD_TUNER_INEVT_CORE_BER_START_OMT_t   in_core_ber_start_omt;  
    DTVD_TUNER_INEVT_CORE_BER_STOP_OMT_t    in_core_ber_stop_omt;   
    DTVD_TUNER_INEVT_CORE_BER_MEASURE_OMT_t in_core_ber_measure_omt;    
    DTVD_TUNER_INEVT_CORE_STATE_DEVERR_t    in_core_state_deverr;   
    DTVD_TUNER_INEVT_ANT_CORE_INIT_t        in_ant_core_init;       
    DTVD_TUNER_INEVT_ANT_CORE_END_t         in_ant_core_end;        
    DTVD_TUNER_INEVT_ANT_CORE_SET_t         in_ant_core_set;        
    DTVD_TUNER_INEVT_ANT_CORE_INIT_OMT_t    in_ant_core_init_omt;   
    DTVD_TUNER_INEVT_TUNER_CORE_PWRON_t     in_tuner_core_pwron;    
    DTVD_TUNER_INEVT_TUNER_CORE_PWROFF_t    in_tuner_core_pwroff;   
    DTVD_TUNER_INEVT_TUNER_CORE_INIT_t      in_tuner_core_init;     
    DTVD_TUNER_INEVT_TUNER_CORE_END_t       in_tuner_core_end;      
    DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_t  in_tuner_core_tune_dtv; 
    DTVD_TUNER_INEVT_TUNER_CORE_SYNC_S_t    in_tuner_core_sync_s;   
    DTVD_TUNER_INEVT_TUNER_CORE_STOP_t      in_tuner_core_stop;     
    DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET_t   in_tuner_core_eco_set;  
    DTVD_TUNER_INEVT_TUNER_CORE_INIT_OMT_t  in_tuner_core_init_omt; 
    DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_OMT_t  in_tuner_core_tune_dtv_omt; 
    DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT_t  in_tuner_core_stop_omt; 
    DTVD_TUNER_INEVT_AGC_CORE_INIT_t        in_agc_core_init;       
    DTVD_TUNER_INEVT_AGC_CORE_END_t         in_agc_core_end;        
    DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_t in_agc_core_measure_fst;    
    DTVD_TUNER_INEVT_AGC_CORE_MEASURE_t     in_agc_core_measure;    
    DTVD_TUNER_INEVT_AGC_CORE_STOP_t        in_agc_core_stop;       
    DTVD_TUNER_INEVT_AGC_CORE_INIT_OMT_t    in_agc_core_init_omt;   
    DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_OMT_t in_agc_core_measure_fst_omt;    
    DTVD_TUNER_INEVT_AGC_CORE_START_OMT_t   in_agc_core_start_omt;  
    DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT_t    in_agc_core_stop_omt;   
    DTVD_TUNER_INEVT_AGC_CORE_MEASURE_OMT_t in_agc_core_measure_omt;    
    DTVD_TUNER_INEVT_AGC_CORE_MEASURE_STOP_OMT_t    in_agc_core_measure_stop_omt;   
    DTVD_TUNER_INEVT_CN_CORE_INIT_t         in_cn_core_init;        
    DTVD_TUNER_INEVT_CN_CORE_END_t          in_cn_core_end;         
    DTVD_TUNER_INEVT_CN_CORE_MEASURE_t      in_cn_core_measure;     
    DTVD_TUNER_INEVT_CN_CORE_STOP_t         in_cn_core_stop;        
    DTVD_TUNER_INEVT_CN_CORE_SET_t          in_cn_core_set;         
    DTVD_TUNER_INEVT_CN_CORE_INIT_OMT_t     in_cn_core_init_omt;    
    DTVD_TUNER_INEVT_CN_CORE_START_OMT_t    in_cn_core_start_omt;   
    DTVD_TUNER_INEVT_CN_CORE_STOP_OMT_t     in_cn_core_stop_omt;    
    DTVD_TUNER_INEVT_CN_CORE_MEASURE_OMT_t  in_cn_core_measure_omt; 
    DTVD_TUNER_INEVT_BER_CORE_INIT_t        in_ber_core_init;       
    DTVD_TUNER_INEVT_BER_CORE_END_t         in_ber_core_end;        
    DTVD_TUNER_INEVT_BER_CORE_START_t       in_ber_core_start;      
    DTVD_TUNER_INEVT_BER_CORE_STOP_t        in_ber_core_stop;       
    DTVD_TUNER_INEVT_BER_CORE_SET_t         in_ber_core_set;        
    DTVD_TUNER_INEVT_BER_CORE_INIT_OMT_t    in_ber_core_init_omt;   
    DTVD_TUNER_INEVT_BER_CORE_START_OMT_t   in_ber_core_start_omt;  
    DTVD_TUNER_INEVT_BER_CORE_STOP_OMT_t    in_ber_core_stop_omt;   
    DTVD_TUNER_INEVT_BER_CORE_MEASURE_OMT_t in_ber_core_measure_omt;    
    DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT_t  in_ber_core_measure_cancel_omt; 
    DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET_t   in_tuner_tmcc_infoget;  
    DTVD_TUNER_INEVT_TUNER_TMCC_STOP_t      in_tuner_tmcc_st;       
    DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM_t  in_tuner_state_confirm; 
    DTVD_TUNER_INEVT_STATE_TUNER_INIT_t     in_state_tuner_init;    
    DTVD_TUNER_INEVT_STATE_TUNER_END_t      in_state_tuner_end;     
    DTVD_TUNER_INEVT_STATE_TUNER_START_t    in_state_tuner_start;   
    DTVD_TUNER_INEVT_STATE_TUNER_STOP_t     in_state_tuner_stop;    
    DTVD_TUNER_INEVT_STATE_TUNER_CONFIRM_t  in_state_tuner_confirm; 
    DTVD_TUNER_INEVT_TMCC_TUNER_INIT_t      in_tmcc_tuner_init;     
    DTVD_TUNER_INEVT_TMCC_TUNER_END_t       in_tmcc_tuner_end;      
    DTVD_TUNER_INEVT_TMCC_TUNER_INFOGET_t   in_tmcc_tuner_infoget;  
    DTVD_TUNER_INEVT_TMCC_TUNER_STOP_t      in_tmcc_tuner_stop;     

    DTVD_TUNER_INEVT_AUTOECO_CORE_INIT_t    in_autoeco_core_init;   
    DTVD_TUNER_INEVT_AUTOECO_CORE_END_t     in_autoeco_core_end;    
    DTVD_TUNER_INEVT_AUTOECO_CORE_START_t   in_autoeco_core_start;  
    DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_t    in_autoeco_core_stop;   
    DTVD_TUNER_INEVT_AUTOECO_CN_NOTICE_t    in_autoeco_cn_notice;   
    DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE_t  in_core_autoeco_notice; 
    DTVD_TUNER_INEVT_CORE_AUTOECO_DEVERR_t  in_core_autoeco_deverr; 

    DTVD_TUNER_INEVT_AUTOECO_CORE_START_OMT_t in_autoeco_core_start_omt;  
    DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_OMT_t  in_autoeco_core_stop_omt;   
    DTVD_TUNER_TIMER_AUTOECO_WATCH_OMT_t      ex_timer_autoeco_watch_omt; 

} DTVD_TUNER_MSG_u;

typedef struct _DTVD_TUNER_RCVMSG_t {
    unsigned char           available;      
    unsigned char           reserved[3];    
    unsigned short          toid;           
    unsigned short          fromid;         
    signed int              command;        
    DTVD_TUNER_MSG_u        buff;           
} DTVD_TUNER_RCVMSG_t;

typedef struct _DTVD_TUNER_EX_MSG_DATA_t {
    unsigned short            fromid;         
    unsigned short            command;        
    DTVD_TUNER_MSG_u        buf;            
} DTVD_TUNER_EX_MSG_DATA_t;

typedef struct _DTVD_TUNER_EX_MSG_QUEUE_t {
    unsigned long               max;                
    unsigned long               size;               
    DTVD_TUNER_EX_MSG_DATA_t    *pwrite;            
    DTVD_TUNER_EX_MSG_DATA_t    *pread;             
    DTVD_TUNER_EX_MSG_DATA_t    buf[D_DTVD_TUNER_EXEVT_CUE_NUM +1]; 
} DTVD_TUNER_EX_MSG_QUEUE_t;

#endif 

