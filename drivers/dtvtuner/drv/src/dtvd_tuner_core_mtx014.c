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

void dtvd_tuner_core_mtx_014_tune_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_007 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_scan_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_007 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_008 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_eco_set( void )
{

    DTVD_TUNER_EXEVT_MDL_ECO_SET_t *eco;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    eco = &tdtvd_tuner_msg.buff.ex_mdl_eco_set;

    if ( ( tdtvd_tuner_core.eco_flag == D_DTVD_TUNER_ON ) &&
         ( eco->mode == D_DTVD_TUNER_ECO_MODE_OFF ) )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_set_eco_flag();

        dtvd_tuner_core_inevt_tuner_eco_set();

        dtvd_tuner_core_inevt_autoeco_start();
    }

    else if ( ( tdtvd_tuner_core.eco_flag == D_DTVD_TUNER_OFF ) &&
              ( eco->mode == D_DTVD_TUNER_ECO_MODE_ON ) )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_set_eco_flag();

        dtvd_tuner_core_inevt_autoeco_stop();

        dtvd_tuner_core_inevt_tuner_eco_set();
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_msg_tuner_nonstd( void )
{
    unsigned char level;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_OFF;

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_inevt_cn_stop();

    dtvd_tuner_core_inevt_autoeco_stop();

    dtvd_tuner_core_trans_msg_async();

    level = dtvd_tuner_core_get_rx_level();

    dtvd_tuner_core_trans_msg_cn( level );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_005 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_msg_tuner_async( void )
{
    unsigned char level;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_OFF;

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_inevt_cn_stop();

    dtvd_tuner_core_inevt_autoeco_stop();

    dtvd_tuner_core_trans_msg_async();

    level = dtvd_tuner_core_get_rx_level();

    dtvd_tuner_core_trans_msg_cn( level );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_005 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_msg_tuner_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_msg_agc_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_msg_tmcc_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_msg_cn_measure( void )
{
    unsigned char level;
    DTVD_TUNER_INEVT_CORE_CN_MEASURE_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_core_cn_measure;

    if( data->seq_id != tdtvd_tuner_core.cn_seq_id )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    level = dtvd_tuner_core_get_rx_level();

    dtvd_tuner_core_trans_msg_cn( level );

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_005 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_msg_cn_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_msg_ber_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_msg_state_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_014_msg_autoeco_notice( void )
{
    DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );
    data = &tdtvd_tuner_msg.buff.in_core_autoeco_notice;

    if ( data->notice == D_DTVD_TUNER_AUTOECO_ECO_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_inevt_tuner_eco_set_param( D_DTVD_TUNER_ON );
    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_core_inevt_tuner_eco_set_param( D_DTVD_TUNER_OFF );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_core_mtx_014_msg_autoeco_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

