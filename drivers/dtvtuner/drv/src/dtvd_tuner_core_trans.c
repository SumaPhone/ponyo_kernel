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
#include "dtvd_tuner_core.h"
#include "dtvd_tuner_tuner.h"

void dtvd_tuner_core_trans_msg_tune_ok
(
    unsigned char agc       
)
{
    unsigned int                    len;
    DTVD_TUNER_MSGDATA_TUNE_OK_t    data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof( DTVD_TUNER_MSGDATA_TUNE_OK_t );

    dtvd_tuner_memset( &data, 0x00, len, len );

    data.seq_id = tdtvd_tuner_core.tune_seq_id;

    data.adj.mode = tdtvd_tuner_monitor.tune.adj.mode;

    data.adj.gi = tdtvd_tuner_monitor.tune.adj.gi;

    data.agc = (unsigned char)( 0xFF - agc );

    dtvd_tuner_core_trans_mdl_send_msg( D_DTVD_TUNER_MSGID_TUNE_OK,
                                        &data, len );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_trans_msg_tune_ng
(
    unsigned char cause     
)
{
    unsigned int                    len;
    DTVD_TUNER_MSGDATA_TUNE_NG_t    data;

    DTVD_DEBUG_MSG_ENTER( cause, 0, 0 );

    printk("*****ng***cause=%d***%s***\n", cause, __func__);
    if( tdtvd_tuner_core.tune_cause != D_DTVD_TUNER_TUNE_NG_NONE )
    {
        return;
    }

    len = sizeof( DTVD_TUNER_MSGDATA_TUNE_NG_t );

    dtvd_tuner_memset( &data, 0x00, len, len );

    data.seq_id = tdtvd_tuner_core.tune_seq_id;

    data.cause = cause;

    dtvd_tuner_core_trans_mdl_send_msg( D_DTVD_TUNER_MSGID_TUNE_NG,
                                        &data, len );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_trans_msg_sync( void )
{
    unsigned int                len;
    DTVD_TUNER_MSGDATA_SYNC_t   data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof( DTVD_TUNER_MSGDATA_SYNC_t );

    dtvd_tuner_memset( &data, 0x00, len, len );

    data.adj.mode = tdtvd_tuner_monitor.tune.adj.mode;

    data.adj.gi = tdtvd_tuner_monitor.tune.adj.gi;

    dtvd_tuner_core_trans_mdl_send_msg( D_DTVD_TUNER_MSGID_SYNC,
                                        &data, len );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_trans_msg_async( void )
{
    unsigned int                len;
    DTVD_TUNER_MSGDATA_ASYNC_t  data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof( DTVD_TUNER_MSGDATA_ASYNC_t );

    dtvd_tuner_memset( &data, 0x00, len, len );

    dtvd_tuner_core_trans_mdl_send_msg( D_DTVD_TUNER_MSGID_ASYNC,
                                        &data, len );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_trans_msg_cn
(
    unsigned char level     
)
{
    unsigned int                len;
    DTVD_TUNER_MSGDATA_CN_OK_t  data;

    DTVD_DEBUG_MSG_ENTER( level, 0, 0 );

    len = sizeof( DTVD_TUNER_MSGDATA_CN_OK_t );

    dtvd_tuner_memset( &data, 0x00, len, len );

    data.rx_level = level;

    dtvd_tuner_core_trans_mdl_send_msg( D_DTVD_TUNER_MSGID_CN_OK,
                                        &data, len );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_trans_msg_deverr( void )
{
    unsigned int                len;
    DTVD_TUNER_MSGDATA_DEVERR_t data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof( DTVD_TUNER_MSGDATA_DEVERR_t );

    dtvd_tuner_memset( &data, 0x00, len, len );

    dtvd_tuner_core_trans_mdl_send_msg( D_DTVD_TUNER_MSGID_DEVERR,
                                        &data, len );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_trans_msg_pre_tune( void )
{
    unsigned int            len;
    DTVD_TUNER_MSG_u        body;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof(DTVD_TUNER_EXEVT_TUNERDD_PRE_t);
    dtvd_tuner_memset( &body, 0x00,
                       len, sizeof(DTVD_TUNER_MSG_u) );

    body.ex_tunerdd_pre.seq_id = tdtvd_tuner_core.tune_seq_id;

    dtvd_tuner_msg_send( D_DTVD_TUNER_BLK_TUNERDD,
                         D_DTVD_TUNER_CMD_TUNERDD_TUNEOK,
                         (void*)&body,
                         len );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_trans_mdl_send_msg
(
    unsigned short msgid,   
    void* data,             
    unsigned int length     
)
{
    DTVD_TUNER_MSG_t    msg;
    unsigned int        data_len;

    DTVD_DEBUG_MSG_ENTER( msgid, 0, length );

    if( data == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_CORE_TRANS,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    data_len = sizeof(DTVD_TUNER_MSG_DATA_u);
    if( length > data_len )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_CORE_TRANS,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    msg.header.msg_id = msgid;

    msg.header.receiver_id = D_DTVD_TUNER_BLOCK_ID_DMM;

    msg.header.sender_id = D_DTVD_TUNER_BLOCK_ID_TUNERDD;

    dtvd_tuner_memset( &msg.data, 0x00, data_len, data_len );
    dtvd_tuner_memcpy( &msg.data, data, length, sizeof(DTVD_TUNER_MSG_DATA_u) );

    dtvd_tuner_com_dev_ddsync_write( tdtvd_tuner_core.pipe_handle,
                                     (void *)&msg,
                                     sizeof(DTVD_TUNER_MSG_t) );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ant_init( void )
{
    DTVD_TUNER_INEVT_ANT_CORE_INIT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_ANT_CORE_INIT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_ANT_CORE_INIT,
                                    D_DTVD_TUNER_CTL_ANT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ant_end
(
    unsigned char mode  
)
{
    DTVD_TUNER_INEVT_ANT_CORE_END_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_ANT_CORE_END_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    data.mode = mode;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_ANT_CORE_END,
                                    D_DTVD_TUNER_CTL_ANT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ant_set( void )
{
    DTVD_TUNER_INEVT_ANT_CORE_SET_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_ANT_CORE_SET_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.ch = tdtvd_tuner_core.tune_ch.no;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_ANT_CORE_SET,
                                    D_DTVD_TUNER_CTL_ANT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_pwron( void )
{
    DTVD_TUNER_INEVT_TUNER_CORE_PWRON_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TUNER_CORE_PWRON_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_CORE_PWRON,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_pwroff( void )
{
    DTVD_TUNER_INEVT_TUNER_CORE_PWROFF_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TUNER_CORE_PWROFF_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_CORE_PWROFF,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_init( void )
{
    DTVD_TUNER_INEVT_TUNER_CORE_INIT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TUNER_CORE_INIT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_CORE_INIT,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_end( void )
{
    DTVD_TUNER_INEVT_TUNER_CORE_END_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TUNER_CORE_END_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_CORE_END,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_tune_dtv( void )
{
    DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    data.ch = tdtvd_tuner_core.tune_ch;
    data.adj = tdtvd_tuner_core.tune_adj;

    data.kind = tdtvd_tuner_core.tune_kind;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_stop( void )
{
    DTVD_TUNER_INEVT_TUNER_CORE_STOP_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TUNER_CORE_STOP_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_CORE_STOP,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_init( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_INIT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_INIT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_INIT,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_end( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_END_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_END_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_END,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_fst_measure( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_measure( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_MEASURE_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_MEASURE_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_stop( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_STOP_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_STOP_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_STOP,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_cn_measure
(
    unsigned long seq_id    
)
{
    DTVD_TUNER_INEVT_CN_CORE_MEASURE_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CN_CORE_MEASURE_t);

    DTVD_DEBUG_MSG_ENTER( (int)seq_id, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    data.seq_id = seq_id;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CN_CORE_MEASURE,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_cn_stop( void )
{
    DTVD_TUNER_INEVT_CN_CORE_STOP_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CN_CORE_STOP_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CN_CORE_STOP,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ber_init( void )
{
    DTVD_TUNER_INEVT_BER_CORE_INIT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_BER_CORE_INIT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_BER_CORE_INIT,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ber_end( void )
{
    DTVD_TUNER_INEVT_BER_CORE_END_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CORE_BER_END_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_BER_CORE_END,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ber_start( void )
{
    DTVD_TUNER_INEVT_BER_CORE_START_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_BER_CORE_START_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_BER_CORE_START,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ber_stop( void )
{
    DTVD_TUNER_INEVT_BER_CORE_STOP_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_BER_CORE_STOP_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_BER_CORE_STOP,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_cn_init( void )
{
    DTVD_TUNER_INEVT_CN_CORE_INIT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CN_CORE_INIT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CN_CORE_INIT,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_cn_end( void )
{
    DTVD_TUNER_INEVT_CN_CORE_END_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CN_CORE_END_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CN_CORE_END,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_cn_set( void )
{
    DTVD_TUNER_INEVT_CN_CORE_SET_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CN_CORE_SET_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CN_CORE_SET,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ber_set( void )
{
    DTVD_TUNER_INEVT_BER_CORE_SET_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_BER_CORE_SET_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_BER_CORE_SET,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_init_omt( void )
{
    DTVD_TUNER_INEVT_TUNER_CORE_INIT_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TUNER_CORE_INIT_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_CORE_INIT_OMT,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_stop_omt( void )
{
    DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_tune_dtv_omt( void )
{
    DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_OMT_t data;

    unsigned int length = sizeof(DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    data.ch_no = tdtvd_tuner_core.tune_ch.no;

    data.sync = tdtvd_tuner_core.omt_tune_kind;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_OMT,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ant_init_omt( void )
{
    DTVD_TUNER_INEVT_ANT_CORE_INIT_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_ANT_CORE_INIT_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_ANT_CORE_INIT_OMT,
                                    D_DTVD_TUNER_CTL_ANT,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_init_omt( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_INIT_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_INIT_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_INIT_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_fst_measure_omt( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_start_omt( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_START_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_START_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_START_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_stop_omt( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_measure_omt( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_MEASURE_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_MEASURE_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_agc_measure_stop_omt( void )
{
    DTVD_TUNER_INEVT_AGC_CORE_MEASURE_STOP_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AGC_CORE_MEASURE_STOP_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_STOP_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_AGC,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_cn_init_omt( void )
{
    DTVD_TUNER_INEVT_CN_CORE_INIT_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CN_CORE_INIT_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CN_CORE_INIT_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_cn_start_omt
(
    unsigned char carrier   
)
{
    DTVD_TUNER_INEVT_CN_CORE_START_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CN_CORE_START_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.carrier = carrier;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CN_CORE_START_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_cn_stop_omt( void )
{
    DTVD_TUNER_INEVT_CN_CORE_STOP_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CN_CORE_STOP_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CN_CORE_STOP_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_cn_measure_omt( void )
{
    DTVD_TUNER_INEVT_CN_CORE_MEASURE_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_CN_CORE_MEASURE_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_CN_CORE_MEASURE_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_CN,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ber_init_omt( void )
{
    DTVD_TUNER_INEVT_BER_CORE_INIT_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_BER_CORE_INIT_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_BER_CORE_INIT_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ber_measure_cancel_omt( void )
{
    DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ber_start_omt
(
    unsigned char   out,        
    unsigned short  period      
)
{
    DTVD_TUNER_INEVT_BER_CORE_START_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_BER_CORE_START_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );
    data.out = out;
    data.period = period;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_BER_CORE_START_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ber_stop_omt( void )
{
    DTVD_TUNER_INEVT_BER_CORE_STOP_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_BER_CORE_STOP_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_BER_CORE_STOP_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_ber_measure_omt( void )
{
    DTVD_TUNER_INEVT_BER_CORE_MEASURE_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_BER_CORE_MEASURE_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_BER_CORE_MEASURE_OMT,
                                    D_DTVD_TUNER_CTL_WATCH_BER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_eco_set_param( unsigned char in_mode )
{
    DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET_t data;

    unsigned int length = sizeof(DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET_t);

    DTVD_DEBUG_MSG_ENTER( in_mode, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    data.mode = in_mode;

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET,
                                    D_DTVD_TUNER_CTL_TUNER,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_tuner_eco_set( void )
{

    dtvd_tuner_core_inevt_tuner_eco_set_param(tdtvd_tuner_core.eco_flag);

    return;
}

void dtvd_tuner_core_inevt_autoeco_init( void )
{
    DTVD_TUNER_INEVT_AUTOECO_CORE_INIT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AUTOECO_CORE_INIT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AUTOECO_CORE_INIT,
                                    D_DTVD_TUNER_CTL_AUTOECO,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_autoeco_end( void )
{
    DTVD_TUNER_INEVT_AUTOECO_CORE_END_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AUTOECO_CORE_END_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AUTOECO_CORE_END,
                                    D_DTVD_TUNER_CTL_AUTOECO,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_autoeco_start( void )
{
    DTVD_TUNER_INEVT_AUTOECO_CORE_START_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AUTOECO_CORE_START_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AUTOECO_CORE_START,
                                    D_DTVD_TUNER_CTL_AUTOECO,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_autoeco_stop( void )
{
    DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AUTOECO_CORE_STOP,
                                    D_DTVD_TUNER_CTL_AUTOECO,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_autoeco_start_omt( void )
{
    DTVD_TUNER_INEVT_AUTOECO_CORE_START_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AUTOECO_CORE_START_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AUTOECO_CORE_START_OMT,
                                    D_DTVD_TUNER_CTL_AUTOECO,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_inevt_autoeco_stop_omt( void )
{
    DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_OMT_t data;
    unsigned int length = sizeof(DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_OMT_t);

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &data, 0x00, length, length );

    dtvd_tuner_main_inevt_send_msg( D_DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_OMT,
                                    D_DTVD_TUNER_CTL_AUTOECO,
                                    D_DTVD_TUNER_CTL_CORE,
                                    &data, length );
    DTVD_DEBUG_MSG_EXIT();
    return;
}

