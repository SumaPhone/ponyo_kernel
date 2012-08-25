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

#ifndef _DTVD_TUNER_PRO_H_
#define _DTVD_TUNER_PRO_H_

#define DTVD_TUNER_ATTRIBUTE

void _dtvd_tuner_memset( void* dest, unsigned int val, unsigned int count );
void _dtvd_tuner_memcpy( void* dest, const void* src, unsigned int count );
void _dtvd_tuner_memmove( void* dest, const void* src, unsigned int count );
#ifdef D_DTVD_TUNER_NO_BUFFOVER_CHECK
#define dtvd_tuner_memset( dest, val, count, destsize )    (_dtvd_tuner_memset( (dest), (val), (count) ))
#define dtvd_tuner_memmcpy( dest, src, count, destsize )    (_dtvd_tuner_memxpy( (dest), (src), (count) ))
#define dtvd_tuner_memmove( dest, src, count, destsize )    (_dtvd_tuner_memmove( (dest), (src), (count) ))
#else
void dtvd_tuner_memset( void* dest, unsigned int val, unsigned int count, unsigned int destsize );
void dtvd_tuner_memcpy( void* dest, const void* src, unsigned int count, unsigned int destsize );
void dtvd_tuner_memmove( void* dest, const void* src, unsigned int count, unsigned int destsize );
#endif 

signed int dtvd_tuner_com_dev_i2c_read( unsigned long, unsigned char*, unsigned char* );
signed int dtvd_tuner_com_dev_i2c_write( unsigned long, DTVD_TUNER_COM_I2C_DATA_t* );

signed int dtvd_tuner_com_dev_i2c_init( void );
void dtvd_tuner_com_dev_i2c_term( void );

void dtvd_tuner_com_dev_powic_read( unsigned long, unsigned char* );
void dtvd_tuner_com_dev_powic_write( unsigned long, unsigned char );

void dtvd_tuner_com_dev_gpio_read( unsigned long, unsigned char* );
void dtvd_tuner_com_dev_gpio_write( unsigned long, unsigned char );

void dtvd_tuner_com_dev_romdata_read( void );
void dtvd_tuner_com_dev_int_regist( unsigned int );
void dtvd_tuner_com_dev_int_unregist( void );
void dtvd_tuner_com_dev_int_enable( void );
void dtvd_tuner_com_dev_int_disable( void );

irqreturn_t dtvd_tuner_com_dev_int_handler(int irq, void *dev_id);

void dtvd_tuner_com_dev_ddsync_write( struct file *, void*, size_t );

signed int dtvd_tuner_com_dev_i2c_rfic_write( unsigned long, DTVD_TUNER_COM_I2C_DATA_t* ); 

signed int dtvd_tuner_com_dev_tsif_start( void );
void dtvd_tuner_com_dev_tsif_stop( void );
//signed int dtvd_tuner_com_dev_tsif_read_start( dma_addr_t buffer[2], unsigned int length[2]);
//signed int dtvd_tuner_com_dev_tsif_read( dma_addr_t buffer, unsigned int length);

signed int dtvd_tuner_com_dev_tsif_read_start( void **buffer, unsigned int length[2]);
signed int dtvd_tuner_com_dev_tsif_read( void *buffer, unsigned int length);

void dtvd_tuner_com_dev_tsif_read_stop(void);
void dtvd_tuner_com_dev_tsif_read_cancel(void);

#define DTVD_TUNER_COM_DEV_I2C_WRITE( _cmd_dat_ )                                 ( dtvd_tuner_com_dev_i2c_write(                                                   (unsigned long) ( sizeof( _cmd_dat_ ) / sizeof(DTVD_TUNER_COM_I2C_DATA_t) ) ,         ( _cmd_dat_ ) ) )

#define DTVD_TUNER_COM_DEV_I2C_RFIC_WRITE( _cmd_dat_ )                             ( dtvd_tuner_com_dev_i2c_rfic_write(                                                (unsigned long) ( sizeof( _cmd_dat_ ) / sizeof(DTVD_TUNER_COM_I2C_DATA_t) ) ,         ( _cmd_dat_ ) ) )

#define DTV_DRV    0x1234    

#define D_DTVD_TUNER_SYSERR_BLKID               ( ( DTV_DRV << 16 ) | 0x00002000 )

#define D_DTVD_TUNER_SYSERR_DRV_SYSTEM          0   
#define D_DTVD_TUNER_SYSERR_DRV_PARAM           1   
#define D_DTVD_TUNER_SYSERR_DRV_STATE           2   
#define D_DTVD_TUNER_SYSERR_DRV_I2C_READ        3   
#define D_DTVD_TUNER_SYSERR_DRV_I2C_WRITE       4   
#define D_DTVD_TUNER_SYSERR_DRV_I2C_INIT        5   
#define D_DTVD_TUNER_SYSERR_DRV_GPIO_READ       6   
#define D_DTVD_TUNER_SYSERR_DRV_GPIO_WRITE      7   
#define D_DTVD_TUNER_SYSERR_DRV_MCBSP_READ      8   
#define D_DTVD_TUNER_SYSERR_DRV_NONVOLA_READ    9   
#define D_DTVD_TUNER_SYSERR_DRV_SPI_READ        10  
#define D_DTVD_TUNER_SYSERR_DRV_SPI_WRITE       11  
#define D_DTVD_TUNER_SYSERR_DRV_PIPE_WRITE      12  
#define D_DTVD_TUNER_SYSERR_DRV_PINT_REG        13  
#define D_DTVD_TUNER_SYSERR_DRV_PINT_UNREG      14  
#define D_DTVD_TUNER_SYSERR_DRV_PINT_ENABLE     15  
#define D_DTVD_TUNER_SYSERR_DRV_PINT_DISABLE    16  
#define D_DTVD_TUNER_SYSERR_DRV_3WIRE_TRANS     17  
#define D_DTVD_TUNER_SYSERR_DRV_PSEQ_CHKSUM     18  
#define D_DTVD_TUNER_SYSERR_DRV_INT_CLEAR       19  
#define D_DTVD_TUNER_SYSERR_DRV_INT_UNKNOWN     20  
#define D_DTVD_TUNER_SYSERR_DRV_ETC             21  
#define D_DTVD_TUNER_SYSERR_DRV_DDSYNC_WRITE    22

#define DTVD_TUNER_API              0
#define DTVD_TUNER_AGC_API          1
#define DTVD_TUNER_AGC_COM          2
#define DTVD_TUNER_AGC_MTX000       3
#define DTVD_TUNER_AGC_MTX001       4
#define DTVD_TUNER_AGC_MTX002       5
#define DTVD_TUNER_AGC_MTX003       6
#define DTVD_TUNER_AGC_MTX004       7
#define DTVD_TUNER_AGC_MTX005       8
#define DTVD_TUNER_AGC_MTX006       9
#define DTVD_TUNER_AGC_MTX007       10
#define DTVD_TUNER_AGC_MTX008       11
#define DTVD_TUNER_AGC_TRANS        12
#define DTVD_TUNER_ANT_API          13
#define DTVD_TUNER_ANT_COM          14
#define DTVD_TUNER_ANT_MTX000       15
#define DTVD_TUNER_ANT_MTX001       16
#define DTVD_TUNER_ANT_TRANS        17
#define DTVD_TUNER_BER_API          18
#define DTVD_TUNER_BER_COM          19
#define DTVD_TUNER_BER_MTX000       20
#define DTVD_TUNER_BER_MTX001       21
#define DTVD_TUNER_BER_MTX002       22
#define DTVD_TUNER_BER_MTX003       23
#define DTVD_TUNER_BER_MTX004       24
#define DTVD_TUNER_BER_MTX005       25
#define DTVD_TUNER_BER_MTX006       26
#define DTVD_TUNER_BER_TRANS        27
#define DTVD_TUNER_CN_API           28
#define DTVD_TUNER_CN_COM           29
#define DTVD_TUNER_CN_MTX000        30
#define DTVD_TUNER_CN_MTX001        31
#define DTVD_TUNER_CN_MTX002        32
#define DTVD_TUNER_CN_MTX003        33
#define DTVD_TUNER_CN_MTX004        34
#define DTVD_TUNER_CN_MTX005        35
#define DTVD_TUNER_CN_MTX006        36
#define DTVD_TUNER_CN_TRANS         37
#define DTVD_TUNER_COM_DEV          38
#define DTVD_TUNER_COM_STD          39
#define DTVD_TUNER_CORE_API         40
#define DTVD_TUNER_CORE_COM         41
#define DTVD_TUNER_CORE_MTX000      42
#define DTVD_TUNER_CORE_MTX001      43
#define DTVD_TUNER_CORE_MTX002      44
#define DTVD_TUNER_CORE_MTX003      45
#define DTVD_TUNER_CORE_MTX004      46
#define DTVD_TUNER_CORE_MTX005      47
#define DTVD_TUNER_CORE_MTX006      48
#define DTVD_TUNER_CORE_MTX007      49
#define DTVD_TUNER_CORE_MTX008      50
#define DTVD_TUNER_CORE_MTX009      51
#define DTVD_TUNER_CORE_MTX010      52
#define DTVD_TUNER_CORE_MTX011      53
#define DTVD_TUNER_CORE_MTX012      54
#define DTVD_TUNER_CORE_MTX013      55
#define DTVD_TUNER_CORE_MTX014      56
#define DTVD_TUNER_CORE_TRANS       57
#define DTVD_TUNER_DRIVER           58
#define DTVD_TUNER_MAIN             59
#define DTVD_TUNER_MAIN_ANLZ        60
#define DTVD_TUNER_MAIN_MTX         61
#define DTVD_TUNER_MSG              62
#define DTVD_TUNER_RAM              63
#define DTVD_TUNER_ROM              64
#define DTVD_TUNER_TIMER            65
#define DTVD_TUNER_TMCC_API         66
#define DTVD_TUNER_TMCC_COM         67
#define DTVD_TUNER_TMCC_MTX000      68
#define DTVD_TUNER_TMCC_MTX001      69
#define DTVD_TUNER_TMCC_MTX002      70
#define DTVD_TUNER_TMCC_MTX003      71
#define DTVD_TUNER_TMCC_TRANS       72
#define DTVD_TUNER_TUNER_API        73
#define DTVD_TUNER_TUNER_COM        74
#define DTVD_TUNER_TUNER_MTX000     75
#define DTVD_TUNER_TUNER_MTX001     76
#define DTVD_TUNER_TUNER_MTX002     77
#define DTVD_TUNER_TUNER_MTX003     78
#define DTVD_TUNER_TUNER_MTX004     79
#define DTVD_TUNER_TUNER_MTX005     80
#define DTVD_TUNER_TUNER_MTX006     81
#define DTVD_TUNER_TUNER_MTX007     82
#define DTVD_TUNER_TUNER_MTX008     83
#define DTVD_TUNER_TUNER_MTX009     84
#define DTVD_TUNER_TUNER_MTX010     85
#define DTVD_TUNER_TUNER_TRANS      86
#define DTVD_TUNER_STATE_API        87
#define DTVD_TUNER_STATE_COM        88
#define DTVD_TUNER_STATE_MTX000     89
#define DTVD_TUNER_STATE_MTX001     90
#define DTVD_TUNER_STATE_MTX002     91
#define DTVD_TUNER_STATE_TRANS      92

#define DTVD_TUNER_AUTOECO_API      93
#define DTVD_TUNER_AUTOECO_COM      94
#define DTVD_TUNER_AUTOECO_MTX000   95
#define DTVD_TUNER_AUTOECO_MTX001   96
#define DTVD_TUNER_AUTOECO_MTX002   97
#define DTVD_TUNER_AUTOECO_TRANS    98

#define DTVD_TUNER_AUTOECO_MTX003   99
#define DTVD_TUNER_TSPRCV          100
#define DTVD_TUNER_TSPRCV_API      101
#define DTVD_TUNER_TSPRCV_TRANS    102

void dtvd_tuner_com_dev_log
(
    unsigned char   rank,       
    unsigned long   kind,       
    unsigned char   file_no,    
    unsigned long   line,       
    unsigned long   data1,      
    unsigned long   data2,      
    unsigned long   data3,      
    unsigned long   data4,      
    unsigned long   data5,      
    unsigned long   data6       
);

#ifdef _DTVD_TUNER_SYSERR
#if 1
#define DTVD_TUNER_SYSERR_RANK_A( kind, file_no, data1, data2, data3, data4, data5, data6 )         ( printk( "SYSERR[RANK_A], kind=%d, file_no=%d, line=%d, data1=%d, data2=%d, data3=%d, data4=%d, data5=%d, data6=%d\n",                    kind, file_no, 323, (int)(data1), (int)(data2), (int)(data3), (int)(data4), (int)(data5), (int)(data6) ) )
#define DTVD_TUNER_SYSERR_RANK_B( kind, file_no, data1, data2, data3, data4, data5, data6 )         ( printk( "SYSERR[RANK_B], kind=%d, file_no=%d, line=%d, data1=%d, data2=%d, data3=%d, data4=%d, data5=%d, data6=%d\n",                    kind, file_no, 326, (int)(data1), (int)(data2), (int)(data3), (int)(data4), (int)(data5), (int)(data6) ) )
#else

#define DTVD_TUNER_SYSERR_RANK_A( kind, file_no, data1, data2, data3, data4, data5, data6 )         ( dtvd_tuner_com_dev_log( D_DTVD_TUNER_DEVERR_SYSERRRANKA, (kind), (file_no), 339,                                    (data1), (data2), (data3), (data4), (data5), (data6) ) )

#define DTVD_TUNER_SYSERR_RANK_B( kind, file_no, data1, data2, data3, data4, data5, data6 )     ( dtvd_tuner_com_dev_log( D_DTVD_TUNER_DEVERR_SYSERRRANKB, (kind), (file_no), 343,                                    (data1), (data2), (data3), (data4), (data5), (data6) ) )

#endif 

#define DTVD_DEBUG_MSG_ERR( fmt, args... )                       printk( "%s %s %s %d %s : " fmt,Feb 10 2011,                                  14:54:28,dtvd_tuner_pro.h,352,__FUNCTION__,## args )

#else
#define DTVD_TUNER_SYSERR_RANK_A( kind, file_no, data1, data2, data3, data4, data5, data6 )
#define DTVD_TUNER_SYSERR_RANK_B( kind, file_no, data1, data2, data3, data4, data5, data6 )
#define DTVD_DEBUG_MSG_ERR( fmt, args... )

#endif 

#ifdef _DTVD_TUNER_DEBUG_OUT
#define DTVD_DEBUG_MSG_ENTER( p1, p2, p3 )          printk( "%s %s %s %d ENTER :%s p1=%d p2=%d p3=%d\n"                    , Feb 10 2011, 14:54:28, dtvd_tuner_pro.h, 365, __FUNCTION__, p1, p2, p3 )
#define DTVD_DEBUG_MSG_EXIT( )                          printk(  "%s %s %s %d EXIT  :%s \n"                    , Feb 10 2011, 14:54:28, dtvd_tuner_pro.h, 368, __FUNCTION__ )
#define DTVD_DEBUG_MSG_CALL( funcname )                          printk( "%s %s %s %d CALL  :%s \n"                    , Feb 10 2011, 14:54:28, dtvd_tuner_pro.h, 371, (funcname) )
#define DTVD_DEBUG_MSG_PCOM_READ( kind, addr, data )                          printk( "%s %s %s %d  %s READ:addr:%x data:%x\n"                    , Feb 10 2011, 14:54:28, dtvd_tuner_pro.h, 374, (kind), (unsigned int)(addr), (unsigned int)(data) )
#define DTVD_DEBUG_MSG_PCOM_WRITE( kind, addr, data )                          printk( "%s %s %s %d  %s WRITE:addr:%x data:%x\n"                    , Feb 10 2011, 14:54:28, dtvd_tuner_pro.h, 377, (kind), (unsigned int)(addr), (unsigned int)(data) )
#define DTVD_DEBUG_MSG_INFO( fmt, args... )                       printk( "%s %s %s %d %s : " fmt,Feb 10 2011,                                  14:54:28,dtvd_tuner_pro.h,380,__FUNCTION__,## args )

#else
#define DTVD_DEBUG_MSG_ENTER( p1, p2, p3 ) //  printk ("%s-%d  -%s\n", __FILE__, __LINE__,__FUNCTION__ );
#define DTVD_DEBUG_MSG_EXIT(  )  //printk ("%s-%d  -%s\n", __FILE__, __LINE__, __FUNCTION__); 
#define DTVD_DEBUG_MSG_CALL( funcname )    //printk ("%s-%d  -%s\n", __FILE__, __LINE__, __FUNCTION__);
#ifdef _DTVD_TUNER_DEBUG_IO_OUT 
#define DTVD_DEBUG_MSG_PCOM_READ( kind, addr, data )                          printk( "%s %s %s %d  %s READ:addr:%x data:%x\n"                    , Feb 10 2011, 14:54:28, dtvd_tuner_pro.h, 389, (kind), (addr), (int)(data) )
#define DTVD_DEBUG_MSG_PCOM_WRITE( kind, addr, data )                          printk( "%s %s %s %d  %s WRITE:addr:%x data:%x\n"                    , Feb 10 2011, 14:54:28, dtvd_tuner_pro.h, 392, (kind), (addr), (int)(data) )
#else
#define DTVD_DEBUG_MSG_PCOM_READ( kind, addr, data )
#define DTVD_DEBUG_MSG_PCOM_WRITE( kind, addr, data )
#endif 
#ifndef WIN32
#define DTVD_DEBUG_MSG_INFO( fmt, args... )
#else
#define DTVD_DEBUG_MSG_INFO( fmt, args )
#endif 
#endif 

void dtvd_tuner_timer_init( void );
void dtvd_tuner_timer_start( unsigned long, unsigned int );
void dtvd_tuner_timer_stop( unsigned long );
void dtvd_tuner_timer_handler( unsigned long );
void dtvd_tuner_timer_mdelay( unsigned long );
void dtvd_tuner_timer_sleep_on_timeout( signed long );

void dtvd_tuner_msg_init( unsigned long, unsigned long );
void dtvd_tuner_msg_send( unsigned short, unsigned short, void*, unsigned int );
signed int dtvd_tuner_msg_rcv( DTVD_TUNER_EX_MSG_DATA_t* );
void dtvd_tuner_main_inevt_send_msg( signed int, unsigned short, unsigned short, void*, unsigned int );

signed int dtvd_tuner_core_init( void );
signed int dtvd_tuner_core_get_state( void );

signed int dtvd_tuner_ant_init( void );
signed int dtvd_tuner_ant_get_state( void );

signed int dtvd_tuner_tuner_init( void );
signed int dtvd_tuner_tuner_get_state( void );

signed int dtvd_tuner_agc_init( void );
signed int dtvd_tuner_agc_get_state( void );

signed int dtvd_tuner_tmcc_init( void );
signed int dtvd_tuner_tmcc_get_state( void );

signed int dtvd_tuner_cn_init( void );
signed int dtvd_tuner_cn_get_state( void );
void dtvd_tuner_cn_measure_non( void );

signed int dtvd_tuner_ber_init( void );
signed int dtvd_tuner_ber_get_state( void );
void dtvd_tuner_ber_measure_non( void );

signed int dtvd_tuner_state_init( void );
signed int dtvd_tuner_state_get_state( void );

signed int dtvd_tuner_autoeco_init( void );
signed int dtvd_tuner_autoeco_get_state( void );

extern DTVD_TUNER_CONTROL_t         tdtvd_tuner_info;       
extern DTVD_TUNER_EX_MSG_QUEUE_t    tdtvd_tuner_que;        
extern DTVD_TUNER_RCVMSG_t          tdtvd_tuner_msg;        
extern DTVD_TUNER_MONITOR_INFO_t    tdtvd_tuner_monitor;
extern DTVD_TUNER_COM_NONVOLATILE_t tdtvd_tuner_nonvola;

#endif 

