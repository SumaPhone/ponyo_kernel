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

#ifndef _DTVD_TUNER_API_H
#define _DTVD_TUNER_API_H

#include<stdio.h>

#define D_DTVD_TUNER_API_NULL            ((void*)0) 

#define D_DTVD_TUNER_API_OK              0          
#define D_DTVD_TUNER_API_NG_DEVERR       (-1)       
#define D_DTVD_TUNER_API_NG_RESET        (-2)       

#define D_DTVD_TUNER_API_INIT_RETRY_COUNT    3      
#define D_DTVD_TUNER_API_IOCTL_RETRY_COUNT   100    

#define D_DTVD_TUNER_SYSERR_BLKID                   ( ( DTV_DRV << 16 ) | 0x00001000 )

#define D_DTVD_TUNER_SYSERR_API_SYSTEM          0   
#define D_DTVD_TUNER_SYSERR_API_PARAM           1   
#define D_DTVD_TUNER_SYSERR_API_VAPI            2   
#define D_DTVD_TUNER_SYSERR_API_ETC             3   

#define DTVD_TUNER_API              0

typedef struct _DTVD_TUNER_API_SYSERR_t {
    unsigned char   file_no;        
    unsigned char   reserved[3];    
    unsigned long   line;           
    unsigned long   log_data[6];    
} DTVD_TUNER_API_SYSERR_t;

#ifdef _DTVD_TUNER_SYSERR
#if 0
#define DTVD_TUNER_API_SYSERR_RANK_A( kind, file_no, data1, data2, data3, data4, data5, data6 )                 ( printf( "SYSERR[RANK_A], kind=%d, file_no=%d, line=%d, data1=%d, data2=%d, data3=%d, data4=%d, data5=%d, data6=%d\n",                            kind, file_no, 67, data1, data2, data3, data4, data5, data6 ) )
#define DTVD_TUNER_API_SYSERR_RANK_B( kind, file_no, data1, data2, data3, data4, data5, data6 )                 ( printf( "SYSERR[RANK_B], kind=%d, file_no=%d, line=%d, data1=%d, data2=%d, data3=%d, data4=%d, data5=%d, data6=%d\n",                            kind, file_no, 70, data1, data2, data3, data4, data5, data6 ) )
#else
#define DTVD_TUNER_API_SYSERR_RANK_A( kind, file_no, data1, data2, data3, data4, data5, data6 )                 ( dtvd_tuner_api_log( CSYSERR_ALM_RANK_A, (kind), (file_no), 75,                                       (unsigned long)(data1), (unsigned long)(data2), (unsigned long)(data3),                                          (unsigned long)(data4), (unsigned long)(data5), (unsigned long)(data6) ) )
#define DTVD_TUNER_API_SYSERR_RANK_B( kind, file_no, data1, data2, data3, data4, data5, data6 )                 ( dtvd_tuner_api_log( CSYSERR_ALM_RANK_B, (kind), (file_no), 79,                                       (unsigned long)(data1), (unsigned long)(data2), (unsigned long)(data3),                                          (unsigned long)(data4), (unsigned long)(data5), (unsigned long)(data6) ) )
#endif 
#else
#define DTVD_TUNER_API_SYSERR_RANK_A( kind, file_no, data1, data2, data3, data4, data5, data6 )
#define DTVD_TUNER_API_SYSERR_RANK_B( kind, file_no, data1, data2, data3, data4, data5, data6 )
#endif 

#ifdef _DTVD_TUNER_DEBUG_OUT
#define DTVD_DEBUG_MSG_ENTER( funcname )                          printf("%s %s %s %d ENTER :%s \n"                    , Feb 10 2011, 14:54:28, dtvd_tuner_api.h, 89, (funcname) )
#define DTVD_DEBUG_MSG_EXIT( funcname )                          printf("%s %s %s %d EXIT  :%s \n"                    , Feb 10 2011, 14:54:28, dtvd_tuner_api.h, 92, (funcname) )
#define DTVD_DEBUG_MSG_CALL( funcname )                          printf("%s %s %s %d CALL  :%s \n"                    , Feb 10 2011, 14:54:28, dtvd_tuner_api.h, 95, (funcname) )
#define DTVD_DEBUG_MSG_INFO( fmt, args... )                      printf( "%s %s %s %d %s : " fmt,Feb 10 2011,                                  14:54:28,dtvd_tuner_api.h,98,__FUNCTION__,## args )
#else
#define DTVD_DEBUG_MSG_ENTER( funcname )  // printk ("%s-%d  -%s\n", __FILE__, __LINE__, __FUNCTION__);
#define DTVD_DEBUG_MSG_EXIT( funcname )   // printk ("%s-%d  -%s\n", __FILE__, __LINE__, __FUNCTION__);
#define DTVD_DEBUG_MSG_CALL( funcname )   // printk ("%s-%d  -%s\n", __FILE__, __LINE__, __FUNCTION__);
#ifndef WIN32
#define DTVD_DEBUG_MSG_INFO( fmt, args... )
#else
#define DTVD_DEBUG_MSG_INFO( fmt, args)
#endif 
#endif 

typedef struct _DTVD_TUNER_API_INFO_t {
    signed int   fd;                                
} DTVD_TUNER_API_INFO_t;

#endif 

