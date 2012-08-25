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

#ifndef _DTVD_TUNER_DEF_H_
#define _DTVD_TUNER_DEF_H_

#define D_DTVD_TUNER_TRUE     1                     
#define D_DTVD_TUNER_FALSE    0                     
#define D_DTVD_TUNER_ON       D_DTVD_TUNER_TRUE     
#define D_DTVD_TUNER_OFF      D_DTVD_TUNER_FALSE    

#define D_DTVD_TUNER_INTR           (-2)            

#define D_DTVD_TUNER_NULL     ((void*)0)

#define D_DTVD_TUNER_REG_NO1  1

#define D_DTVD_TUNER_WAKEUP_PWRON           0x00000001  
#define D_DTVD_TUNER_WAKEUP_PWROFF          0x00000002  
#define D_DTVD_TUNER_WAKEUP_INIT            0x00000004  
#define D_DTVD_TUNER_WAKEUP_END             0x00000008  
#define D_DTVD_TUNER_WAKEUP_STOP            0x00000010  
#define D_DTVD_TUNER_WAKEUP_INIT_OMT        0x00000020  
#define D_DTVD_TUNER_WAKEUP_STOP_OMT        0x00000040  
#define D_DTVD_TUNER_WAKEUP_TUNE_OMT        0x00000080  
#define D_DTVD_TUNER_WAKEUP_BER_START_OMT   0x00000100  
#define D_DTVD_TUNER_WAKEUP_BER_STOP_OMT    0x00000200  
#define D_DTVD_TUNER_WAKEUP_BER_GET_OMT     0x00000400  
#define D_DTVD_TUNER_WAKEUP_CNR_START_OMT   0x00000800  
#define D_DTVD_TUNER_WAKEUP_CNR_STOP_OMT    0x00001000  
#define D_DTVD_TUNER_WAKEUP_CNR_GET_OMT     0x00002000  
#define D_DTVD_TUNER_WAKEUP_AGC_START_OMT   0x00004000  
#define D_DTVD_TUNER_WAKEUP_AGC_STOP_OMT    0x00008000  
#define D_DTVD_TUNER_WAKEUP_AGC_GET_OMT     0x00010000  
#define D_DTVD_TUNER_WAKEUP_TUNE            0x00020000  

#define D_DTVD_TUNER_TIMER_PWRON_HALFWAY    0   
#define D_DTVD_TUNER_TIMER_PWRON            1   
#define D_DTVD_TUNER_TIMER_SEARCH           2   
#define D_DTVD_TUNER_TIMER_SYNC             3   
#define D_DTVD_TUNER_TIMER_START_TS         4   
#define D_DTVD_TUNER_TIMER_ECO              5   
#define D_DTVD_TUNER_TIMER_AGC              6   
#define D_DTVD_TUNER_TIMER_WATCH_AGC        7   
#define D_DTVD_TUNER_TIMER_MEASURE_CN_FST   8   
#define D_DTVD_TUNER_TIMER_MEASURE_CN       9   
#define D_DTVD_TUNER_TIMER_WATCH_CN         10  
#define D_DTVD_TUNER_TIMER_MEASURE_BER_FST  11  
#define D_DTVD_TUNER_TIMER_MEASURE_BER      12  
#define D_DTVD_TUNER_TIMER_WATCH_BER        13  
#define D_DTVD_TUNER_TIMER_WATCH_STATE      14  
#define D_DTVD_TUNER_TIMER_TMCC             15  
#define D_DTVD_TUNER_TIMER_WATCH_TMCC       16  

#define D_DTVD_TUNER_TIMER_WATCH_AUTOECO_OMT 17 

#define D_DTVD_TUNER_TIMER_MAX              18  

#endif 

