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

#ifndef _DTVD_TUNER_AUTOECO_H_
#define _DTVD_TUNER_AUTOECO_H_

#define D_DTVD_TUNER_AUTOECO_000     0       
#define D_DTVD_TUNER_AUTOECO_001     1       
#define D_DTVD_TUNER_AUTOECO_002     2       

#define D_DTVD_TUNER_AUTOECO_003     3       

#define D_DTVD_TUNER_AUTOECO_ECO_NG  0       
#define D_DTVD_TUNER_AUTOECO_ECO_OK  1       

#define D_DTVD_TUNER_AUTOECO_DEF_TO_ECO  0   
#define D_DTVD_TUNER_AUTOECO_ECO_TO_DEF  1   
#define D_DTVD_TUNER_AUTOECO_NON         2   

typedef struct _DTVD_TUNER_AUTOECO_THR_t {
    unsigned int        cn_n2e;               
    unsigned int        cn_e2n;               
    unsigned char       agc_n2e;              
    unsigned char       agc_e2n;              
    unsigned char       reserved[2];          
} DTVD_TUNER_AUTOECO_THR_t;

typedef struct _DTVD_TUNER_AUTOECO_INFO_t {
    signed int                status;                    
    unsigned char             autoeco_flg;               
    unsigned char             reserved[3];               
    DTVD_TUNER_AUTOECO_THR_t  qpsk12;                    
    DTVD_TUNER_AUTOECO_THR_t  qpsk23;                    
    DTVD_TUNER_AUTOECO_THR_t  qam12;                     
} DTVD_TUNER_AUTOECO_INFO_t;

void dtvd_tuner_autoeco_com_set_status( signed int );
void dtvd_tuner_autoeco_com_init( void );
signed int dtvd_tuner_autoeco_com_agc_read( unsigned char * );
void dtvd_tuner_autoeco_com_eco_judge( unsigned char, unsigned int, unsigned int * );
void dtvd_tuner_autoeco_com_thr_judge( DTVD_TUNER_AUTOECO_THR_t ** );

void dtvd_tuner_autoeco_inevt_core_notice( unsigned char );
void dtvd_tuner_autoeco_inevt_core_deverr( void );

extern DTVD_TUNER_AUTOECO_INFO_t tdtvd_tuner_autoeco;

#endif 

