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

#ifndef _DTVD_TUNER_ANT_H_
#define _DTVD_TUNER_ANT_H_

#define D_DTVD_TUNER_ANT_000    0   
#define D_DTVD_TUNER_ANT_001    1   

typedef struct _DTVD_TUNER_ANT_INFO_t {
    signed int      status;         
    unsigned int    style;          
    unsigned char   ch;             
    unsigned char   ant_kind;       
    unsigned char   reserved[2];    
} DTVD_TUNER_ANT_INFO_t;

void dtvd_tuner_ant_com_set_status( signed int );

void dtvd_tuner_ant_inevt_core_init( void );
void dtvd_tuner_ant_inevt_core_end( void );
void dtvd_tuner_ant_inevt_core_init_omt( void );

extern DTVD_TUNER_ANT_INFO_t tdtvd_tuner_ant;

#endif 

