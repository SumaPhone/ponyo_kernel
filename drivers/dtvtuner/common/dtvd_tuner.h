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

#ifndef _DTVD_TUNER_
#define _DTVD_TUNER_

#include "dtvtuner.h"                                               

void DTVD_TUNER_OPEN( void );

void DTVD_TUNER_CLOSE( void );

void DTVD_TUNER_PWRON( void );

signed int DTVD_TUNER_INIT( unsigned char *pipename );              

signed int DTVD_TUNER_READ_TSP(  void *buffer,                      
                                signed int length );                

signed int DTVD_TUNER_TUNE_DTV( unsigned long seq_id,               
                                DTVD_TUNER_CHANNEL_t channel,       
                                DTVD_TUNER_ADJUST_t adjust,         
                                unsigned char kind );               

signed int DTVD_TUNER_STOP( void );

signed int DTVD_TUNER_END( void );

void DTVD_TUNER_PWROFF( void );

void DTVD_TUNER_GET_CN( signed int *result,                         
                        unsigned char *level );                     

void DTVD_TUNER_GET_INFO( DTVD_TUNER_MONITOR_INFO_t *info );        

void DTVD_TUNER_SET_STYLE( unsigned int style );                    

void DTVD_TUNER_SET_ECO( unsigned char mode );                      

signed int DTVD_TUNER_INIT_OMT( void );

signed int DTVD_TUNER_STOP_OMT( void );

signed int DTVD_TUNER_TUNE_DTV_OMT( unsigned char ch_no,            
                                    unsigned char sync,             
                                    signed int* result,             
                                    DTVD_TUNER_ADJUST_t* adjust );  

signed int DTVD_TUNER_BER_START_OMT( unsigned char out,             
                                     unsigned char period );        

signed int DTVD_TUNER_BER_STOP_OMT( void );

signed int DTVD_TUNER_BER_GET_OMT( DTVD_TUNER_MEASURE_VALUE_t* value );         

signed int DTVD_TUNER_CNR_START_OMT( unsigned char carrier );       

signed int DTVD_TUNER_CNR_STOP_OMT( void );

signed int DTVD_TUNER_CNR_GET_OMT( DTVD_TUNER_MEASURE_VALUE_t* value_x,         
                                   DTVD_TUNER_MEASURE_VALUE_t* value_y,         
                                   DTVD_TUNER_MEASURE_VALUE_t* value_comp );    

signed int DTVD_TUNER_AGC_START_OMT( void );

signed int DTVD_TUNER_AGC_STOP_OMT( void );

signed int DTVD_TUNER_AGC_GET_OMT( unsigned char* value_x,          
                                   unsigned char* value_y );        

void DTVD_TUNER_ANT_SET_OMT( unsigned char ant );   

#define DTVD_TUNER_ANT_DISABLE_OMT( a )    ( DTVD_TUNER_ANT_SET_OMT( D_DTVD_TUNER_ANT_KIND_HSJ ) )

#endif 

