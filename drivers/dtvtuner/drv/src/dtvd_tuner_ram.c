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

#include "dtvd_tuner_com.h"
#include "dtvd_tuner_main.h"
#include "dtvd_tuner_core.h"
#include "dtvd_tuner_ant.h"
#include "dtvd_tuner_tuner.h"
#include "dtvd_tuner_agc.h"
#include "dtvd_tuner_tmcc.h"
#include "dtvd_tuner_cn.h"
#include "dtvd_tuner_ber.h"
#include "dtvd_tuner_state.h"

DTVD_TUNER_CONTROL_t            tdtvd_tuner_info;       
DTVD_TUNER_EX_MSG_QUEUE_t       tdtvd_tuner_que;        
DTVD_TUNER_MAIN_t               tdtvd_tuner_main;       
DTVD_TUNER_RCVMSG_t             tdtvd_tuner_msg;        

DTVD_TUNER_MONITOR_INFO_t       tdtvd_tuner_monitor;    
DTVD_TUNER_COM_NONVOLATILE_t    tdtvd_tuner_nonvola;    

DTVD_TUNER_CORE_INFO_t          tdtvd_tuner_core;       
DTVD_TUNER_ANT_INFO_t           tdtvd_tuner_ant;        
DTVD_TUNER_TUNER_INFO_t         tdtvd_tuner_tuner;      
DTVD_TUNER_AGC_INFO_t           tdtvd_tuner_agc;        
DTVD_TUNER_TMCC_WATCH_INFO_t    tdtvd_tuner_tmcc;       
DTVD_TUNER_CN_INFO_t            tdtvd_tuner_cn;         
DTVD_TUNER_BER_INFO_t           tdtvd_tuner_ber;        
DTVD_TUNER_STATE_WATCH_INFO_t   tdtvd_tuner_state;      

DTVD_TUNER_AUTOECO_INFO_t       tdtvd_tuner_autoeco;    

struct task_struct *tdtvd_tuner_tsprcv_thread; 

DTVD_TUNER_TSP_RINGBUFFER_t     tdtvd_tuner_tsp_ringbuffer; 
DTVD_TUNER_TSPRCV_CTRL_t        tdtvd_tuner_tsprcv_ctrl;    

