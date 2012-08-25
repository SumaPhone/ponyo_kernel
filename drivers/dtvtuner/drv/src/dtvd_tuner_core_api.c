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

static void dtvd_tuner_core_get_level_thr( unsigned int*, unsigned int* );

signed int dtvd_tuner_core_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_memset( &tdtvd_tuner_core, 0x00,
                       sizeof(DTVD_TUNER_CORE_INFO_t),
                       sizeof(DTVD_TUNER_CORE_INFO_t) );

    tdtvd_tuner_core.status = D_DTVD_TUNER_CORE_000;
    tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_SYNC_STATE_ASYNC;
    tdtvd_tuner_core.tune_cause = D_DTVD_TUNER_TUNE_NG_NONE;
    tdtvd_tuner_core.cn_seq_id = 0;
    tdtvd_tuner_core.tune_seq_id = 0;

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_core_get_state( void )
{
    signed int state;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    state = tdtvd_tuner_core.status;

    DTVD_DEBUG_MSG_EXIT();
    return state;
}

unsigned char dtvd_tuner_core_get_rx_level( void )
{
    unsigned char level;
    unsigned int thr1;              
    unsigned int thr2;              
    unsigned int cn;
    unsigned int cn_up;
    unsigned int cn_low;
    unsigned int cn_ext;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_monitor.tmcc.nonstd == D_DTVD_TUNER_TRUE )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_RX_LEVEL_OUT;
    }

    if( tdtvd_tuner_monitor.tune.sync_state == D_DTVD_TUNER_SYNC_STATE_ASYNC )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_RX_LEVEL_OUT;
    }

    if( tdtvd_tuner_monitor.rx.cna.value_comp.result != D_DTVD_TUNER_GET_RESULT_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_RX_LEVEL_OUT;
    }

    dtvd_tuner_core_get_level_thr( &thr1, &thr2 );

    cn_up   = ( unsigned int )tdtvd_tuner_monitor.rx.cna.value_comp.up;
    cn_low  = ( unsigned int )tdtvd_tuner_monitor.rx.cna.value_comp.low;
    cn_ext  = ( unsigned int )tdtvd_tuner_monitor.rx.cna.value_comp.ext;
    cn      = ( ( cn_ext << 16 ) | ( cn_up << 8 ) ) | cn_low;

    if( cn > thr1 )
    {
        level = D_DTVD_TUNER_RX_LEVEL_LOW;
    }
    else if( ( cn <= thr1 ) && ( cn > thr2 ) )
    {
        level = D_DTVD_TUNER_RX_LEVEL_MDL;
    }
    else{
        level = D_DTVD_TUNER_RX_LEVEL_HIGH;
    }

    DTVD_DEBUG_MSG_EXIT();
    return level;
}

static void dtvd_tuner_core_get_level_thr
(
    unsigned int *thr1,           
    unsigned int *thr2            
)
{
    unsigned char modulation;
    unsigned char coderate;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    modulation = tdtvd_tuner_monitor.tmcc.info.curr.layer_a.modulation;
    coderate = tdtvd_tuner_monitor.tmcc.info.curr.layer_a.coderate;

    if( thr1 == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_CORE_API,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    if( thr2 == D_DTVD_TUNER_NULL )
    {
        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_CORE_API,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    *thr1 = tdtvd_tuner_core.cn_qpsk2_3_m;
    *thr2 = tdtvd_tuner_core.cn_qpsk2_3_h;

    if( ( tdtvd_tuner_tuner.eco_mode == D_DTVD_TUNER_ECO_MODE_ON ) ||
        ( tdtvd_tuner_tuner.eco_mode == D_DTVD_TUNER_ECO_MODE_ON_SHIFT ) )
    {
        if( modulation == D_DTVD_TUNER_TMCC_MODULATION_QPSK )
        {
            if( coderate == D_DTVD_TUNER_TMCC_CODERATE_1_2 )
            {
                *thr1 = tdtvd_tuner_core.cn_qpsk1_2_eco_m;
                *thr2 = tdtvd_tuner_core.cn_qpsk1_2_eco_h;
            }

            if( coderate == D_DTVD_TUNER_TMCC_CODERATE_2_3 )
            {
                *thr1 = tdtvd_tuner_core.cn_qpsk2_3_eco_m;
                *thr2 = tdtvd_tuner_core.cn_qpsk2_3_eco_h;
            }
            DTVD_DEBUG_MSG_EXIT();
            return;

        }

        if( modulation == D_DTVD_TUNER_TMCC_MODULATION_16QAM )
        {
            if( coderate == D_DTVD_TUNER_TMCC_CODERATE_1_2 )
            {
                *thr1 = tdtvd_tuner_core.cn_16qam1_2_eco_m;
                *thr2 = tdtvd_tuner_core.cn_16qam1_2_eco_h;
            }
            DTVD_DEBUG_MSG_EXIT();
            return;
        }
    }

    if( modulation == D_DTVD_TUNER_TMCC_MODULATION_QPSK )
    {
        if( coderate == D_DTVD_TUNER_TMCC_CODERATE_1_2 )
        {
            *thr1 = tdtvd_tuner_core.cn_qpsk1_2_m;
            *thr2 = tdtvd_tuner_core.cn_qpsk1_2_h;
        }

        if( coderate == D_DTVD_TUNER_TMCC_CODERATE_2_3 )
        {
            *thr1 = tdtvd_tuner_core.cn_qpsk2_3_m;
            *thr2 = tdtvd_tuner_core.cn_qpsk2_3_h;
        }
        DTVD_DEBUG_MSG_EXIT();
        return;

    }

    if( modulation == D_DTVD_TUNER_TMCC_MODULATION_16QAM )
    {
        if( coderate == D_DTVD_TUNER_TMCC_CODERATE_1_2 )
        {
            *thr1 = tdtvd_tuner_core.cn_16qam1_2_m;
            *thr2 = tdtvd_tuner_core.cn_16qam1_2_h;
        }
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}
