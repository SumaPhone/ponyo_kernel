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

void dtvd_tuner_tuner_mtx_002_msg_core_pwroff( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_com_pwr_off();

    dtvd_tuner_tuner_inevt_core_pwroff();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_000 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_002_msg_core_init( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );
    dtvd_tuner_tuner_inevt_tmcc_tuner_init();

    dtvd_tuner_tuner_inevt_state_tuner_init();

    ret = dtvd_tuner_tuner_com_init();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();

        dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_003 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    ret = dtvd_tuner_tuner_com_vaint_factor_clear();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();

        dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_003 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_tuner_inevt_core_init();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_003 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_002_msg_core_init_omt( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_tmcc_tuner_init();

    dtvd_tuner_tuner_inevt_state_tuner_init();

    ret = dtvd_tuner_tuner_com_init();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr_omt();

        dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_003 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    ret = dtvd_tuner_tuner_com_vaint_factor_clear();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr_omt();

        dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_003 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_tuner_inevt_core_init_omt();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_003 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

