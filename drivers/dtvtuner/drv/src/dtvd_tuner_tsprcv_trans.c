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

static void dtvd_tuner_tsprcv_trans_mdl_send_msg( unsigned short, void*, unsigned int );

void dtvd_tuner_tsprcv_trans_msg_deverr( void )
{
    unsigned int                len;
    DTVD_TUNER_MSGDATA_DEVERR_t data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof( DTVD_TUNER_MSGDATA_DEVERR_t );

    dtvd_tuner_memset( &data, 0x00, len, len );
    dtvd_tuner_tsprcv_trans_mdl_send_msg( D_DTVD_TUNER_MSGID_DEVERR,
                                        &data, len );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tsprcv_trans_msg_overflow( void )
{
    unsigned int                len;
    DTVD_TUNER_MSGDATA_DEVERR_t data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof( DTVD_TUNER_MSGDATA_DEVERR_t );

    dtvd_tuner_memset( &data, 0x00, len, len );
    data.error_type = D_DTVD_TUNER_DEVERR_TSPOVERFLOW;
    
    printk("*******transfer overflow DEVERR***,but not transfer this error***%s\n",__func__);
    dtvd_tuner_tsprcv_trans_mdl_send_msg( D_DTVD_TUNER_MSGID_DEVERR,
                                        &data, len );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_tsprcv_trans_mdl_send_msg
(
    unsigned short msgid,   
    void* data,             
    unsigned int length     
)
{
    DTVD_TUNER_MSG_t  msg;
    unsigned int        data_len;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( data == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TSPRCV_TRANS,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    data_len = sizeof(DTVD_TUNER_MSG_DATA_u);
    if( length > data_len )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TSPRCV_TRANS,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    if(tdtvd_tuner_core.pipe_handle != D_DTVD_TUNER_NULL)
    {

        msg.header.msg_id = msgid;

        msg.header.receiver_id = D_DTVD_TUNER_BLOCK_ID_DMM;

        msg.header.sender_id = D_DTVD_TUNER_BLOCK_ID_TUNERDD;

        dtvd_tuner_memset( &msg.data, 0x00, data_len, data_len );
        dtvd_tuner_memcpy( &msg.data, data, length, sizeof(DTVD_TUNER_MSG_DATA_u) );

        dtvd_tuner_com_dev_ddsync_write( tdtvd_tuner_core.pipe_handle,
                                     ( void * )&msg,
                                     sizeof( DTVD_TUNER_MSG_t ) );
     }
    DTVD_DEBUG_MSG_EXIT();

    return;
}

