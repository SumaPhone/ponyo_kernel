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
#include "dtvd_tuner_autoeco.h"

void dtvd_tuner_autoeco_mtx_002_msg_core_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_autoeco.autoeco_flg = D_DTVD_TUNER_OFF;

    dtvd_tuner_autoeco_com_set_status( D_DTVD_TUNER_AUTOECO_001 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_autoeco_mtx_002_msg_cn_notice( void )
{
    signed int     ret;
    unsigned int   result;
    unsigned char  agc_data;
    unsigned int   cn_data;
    unsigned int   ext;
    unsigned int   up;
    unsigned int   low;
    DTVD_TUNER_INEVT_AUTOECO_CN_NOTICE_t *cn_msg_data =
                                    &tdtvd_tuner_msg.buff.in_autoeco_cn_notice;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ext = cn_msg_data->cn.ext;
    up = cn_msg_data->cn.up;
    low = cn_msg_data->cn.low;

    cn_data = ( ( ( ext << 16 ) |                        
                     ( up << 8 ) ) |                     
                     ( low ) );                          

    ret = dtvd_tuner_autoeco_com_agc_read( &agc_data );

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_autoeco_inevt_core_deverr();

        dtvd_tuner_autoeco_com_set_status( D_DTVD_TUNER_AUTOECO_001 );

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_autoeco_com_eco_judge( agc_data, cn_data, &result );

    if( result == D_DTVD_TUNER_AUTOECO_DEF_TO_ECO )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        tdtvd_tuner_autoeco.autoeco_flg = D_DTVD_TUNER_ON;

        dtvd_tuner_autoeco_inevt_core_notice( D_DTVD_TUNER_AUTOECO_ECO_OK );

    }

    else if( result == D_DTVD_TUNER_AUTOECO_ECO_TO_DEF )
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        tdtvd_tuner_autoeco.autoeco_flg = D_DTVD_TUNER_OFF;

        dtvd_tuner_autoeco_inevt_core_notice( D_DTVD_TUNER_AUTOECO_ECO_NG );

    }

    else if( result == D_DTVD_TUNER_AUTOECO_NON )
    {
        DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

    }

    else
    {
        DTVD_DEBUG_MSG_ENTER( 5, 0, 0 );

    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

