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
#include "dtvd_tuner_tuner.h"

void dtvd_tuner_tuner_mtx_004_msg_core_stop( void )
{
    signed int ret;

     DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_SEARCH );

    dtvd_tuner_com_dev_int_disable();

    ret = dtvd_tuner_tuner_com_tuner_int_disable();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
    }

    ret = dtvd_tuner_tuner_com_search_endset();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
    }

    dtvd_tuner_tuner_inevt_tmcc_stop();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_003 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_004_msg_core_stop_omt( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_SEARCH );

    dtvd_tuner_com_dev_int_disable();

    ret = dtvd_tuner_tuner_com_tuner_int_disable();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr_omt();
    }

    ret = dtvd_tuner_tuner_com_search_endset();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr_omt();
    }

    dtvd_tuner_tuner_inevt_tmcc_stop();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_009 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_004_msg_core_eco_set( void )
{
    signed int ret;
    DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_tuner_core_eco_set;

    ret = dtvd_tuner_tuner_com_set_eco( data->mode );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_SEARCH );

        dtvd_tuner_tuner_inevt_core_deverr();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_004_tuner_int( void )
{
    signed   int  ret;
    unsigned int  result;
    signed int    next_sts;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_SEARCH );

    ret = dtvd_tuner_tuner_com_searchflg_judge( &result );

    if ( ( ret    != D_DTVD_TUNER_OK   ) ||
         ( result != D_DTVD_TUNER_TRUE )    )
    {

        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    ret = dtvd_tuner_tuner_com_tuner_int_clear();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    ret = dtvd_tuner_tuner_com_tuner_int_disable();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    ret = dtvd_tuner_tuner_com_search_result_read( &result );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    if ( result == D_DTVD_TUNER_TRUE )
    {
        DTVD_DEBUG_MSG_ENTER( 5, 0, 0 );

        next_sts = D_DTVD_TUNER_TUNER_005;
    }
    else
    {
        DTVD_DEBUG_MSG_ENTER( 6, 0, 0 );

        dtvd_tuner_tuner_inevt_core_tune_ng( D_DTVD_TUNER_TUNE_NG_CAUSE9 );

        next_sts = D_DTVD_TUNER_TUNER_008;
    }

    ret = dtvd_tuner_tuner_com_search_endset();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 7, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_tuner_inevt_state_tuner_start();

    dtvd_tuner_tuner_com_set_status( next_sts );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_004_timeout_serach( void )
{
    signed   int  ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_com_dev_int_disable();

    ret = dtvd_tuner_tuner_com_tuner_int_disable();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    ret = dtvd_tuner_tuner_com_tuner_int_clear();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    ret = dtvd_tuner_tuner_com_search_endset();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_tuner_inevt_core_tune_ng( D_DTVD_TUNER_TUNE_NG_CAUSE8 );

    dtvd_tuner_tuner_inevt_state_tuner_start();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_008 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_004_timeout_eco( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_com_timeout_eco();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

