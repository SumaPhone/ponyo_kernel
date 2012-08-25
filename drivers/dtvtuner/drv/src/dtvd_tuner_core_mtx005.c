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

void dtvd_tuner_core_mtx_005_tune_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_007 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_scan_dtv( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_set_tuneinfo( D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_007 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_stop();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_008 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_tuner_tsout( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_inevt_ber_start();

    dtvd_tuner_core_trans_msg_sync();

    tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_ON;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_tuner_nonstd( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.sync_flg == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_OFF;

        dtvd_tuner_core_inevt_ber_stop();

        dtvd_tuner_core_inevt_cn_stop();

        if ( tdtvd_tuner_core.eco_flag == D_DTVD_TUNER_OFF )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            dtvd_tuner_core_inevt_tuner_eco_set_param( D_DTVD_TUNER_OFF );
        }

        dtvd_tuner_core_inevt_autoeco_stop();

        dtvd_tuner_core_trans_msg_async();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_tuner_sync( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_tuner_async( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.sync_flg == D_DTVD_TUNER_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_OFF;

        dtvd_tuner_core_inevt_ber_stop();

        dtvd_tuner_core_inevt_cn_stop();

        if ( tdtvd_tuner_core.eco_flag == D_DTVD_TUNER_OFF )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            dtvd_tuner_core_inevt_tuner_eco_set_param( D_DTVD_TUNER_OFF );
        }

        dtvd_tuner_core_inevt_autoeco_stop();

        dtvd_tuner_core_trans_msg_async();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_tuner_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_tuner_errcore( void )
{
    unsigned long seq_id;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    seq_id = dtvd_tuner_core_get_cn_seq_id();
    dtvd_tuner_core_inevt_cn_measure( seq_id );

    if ( tdtvd_tuner_core.eco_flag == D_DTVD_TUNER_OFF )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_core_inevt_autoeco_start();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_agc_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_tmcc_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_cn_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_ber_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_msg_state_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_mtx_005_eco_set( void )
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

void dtvd_tuner_core_mtx_005_msg_autoeco_notice( void )
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

void dtvd_tuner_core_mtx_005_msg_autoeco_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_deverr();

    dtvd_tuner_core_com_set_status( D_DTVD_TUNER_CORE_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

