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

static void dtvd_tuner_core_sync_time_start( void );
static void dtvd_tuner_core_get_agcthr( unsigned char, unsigned char, unsigned char, unsigned char* );

void dtvd_tuner_core_com_init( void )
{

    dtvd_tuner_core_com_init_agcthr();

    dtvd_tuner_core_com_init_cnthr();

    return;
}

void dtvd_tuner_core_com_init_agcthr( void )
{

    static const unsigned char tdtvd_tuner_core_agcthr_normal1_tbl[D_DTVD_TUNER_CORE_CH_GRP_NUM] =
    {
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00    
    };

    static const unsigned char tdtvd_tuner_core_agcthr_eco1_tbl[D_DTVD_TUNER_CORE_CH_GRP_NUM] =
    {
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00    
    };

    static const unsigned char tdtvd_tuner_core_agcthr_normal2_tbl[D_DTVD_TUNER_CORE_CH_GRP_NUM] =
    {
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00    
    };

    static const unsigned char tdtvd_tuner_core_agcthr_eco2_tbl[D_DTVD_TUNER_CORE_CH_GRP_NUM] =
    {
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00,   
        0x00    
    };

    signed int i;

    for( i = 0; i < D_DTVD_TUNER_CORE_CH_GRP_NUM; i++ )
    {

        tdtvd_tuner_core.agc_normal[i].agc_thr1 = tdtvd_tuner_core_agcthr_normal1_tbl[i];
        tdtvd_tuner_core.agc_normal[i].agc_thr2 = tdtvd_tuner_core_agcthr_normal2_tbl[i];
        tdtvd_tuner_core.agc_eco[i].agc_thr1    = tdtvd_tuner_core_agcthr_eco1_tbl[i];
        tdtvd_tuner_core.agc_eco[i].agc_thr2    = tdtvd_tuner_core_agcthr_eco2_tbl[i];

        if( i == 0 )
        {
            tdtvd_tuner_core.agc_normal[i].ch_min  = D_DTVD_TUNER_CHANNEL_MIN;
            tdtvd_tuner_core.agc_eco[i].ch_min     = D_DTVD_TUNER_CHANNEL_MIN;

            tdtvd_tuner_core.agc_normal[i].ch_max   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_A;
            tdtvd_tuner_core.agc_eco[i].ch_max      = D_DTVD_TUNER_CORE_ECO_CH_GRP_A;
        }
        else if( i == 1 )
        {
            tdtvd_tuner_core.agc_normal[i].ch_min   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_A + 1;
            tdtvd_tuner_core.agc_eco[i].ch_min      = D_DTVD_TUNER_CORE_ECO_CH_GRP_A    + 1;

            tdtvd_tuner_core.agc_normal[i].ch_max   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_B;
            tdtvd_tuner_core.agc_eco[i].ch_max      = D_DTVD_TUNER_CORE_ECO_CH_GRP_B;
        }
        else if( i == 2 )
        {
            tdtvd_tuner_core.agc_normal[i].ch_min   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_B + 1;
            tdtvd_tuner_core.agc_eco[i].ch_min      = D_DTVD_TUNER_CORE_ECO_CH_GRP_B    + 1;

            tdtvd_tuner_core.agc_normal[i].ch_max   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_C;
            tdtvd_tuner_core.agc_eco[i].ch_max      = D_DTVD_TUNER_CORE_ECO_CH_GRP_C;
        }
        else if( i == 3 )
        {
            tdtvd_tuner_core.agc_normal[i].ch_min   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_C + 1;
            tdtvd_tuner_core.agc_eco[i].ch_min      = D_DTVD_TUNER_CORE_ECO_CH_GRP_C    + 1;

            tdtvd_tuner_core.agc_normal[i].ch_max   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_D;
            tdtvd_tuner_core.agc_eco[i].ch_max      = D_DTVD_TUNER_CORE_ECO_CH_GRP_D;
        }
        else if( i == 4 )
        {
            tdtvd_tuner_core.agc_normal[i].ch_min   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_D + 1;
            tdtvd_tuner_core.agc_eco[i].ch_min      = D_DTVD_TUNER_CORE_ECO_CH_GRP_D    + 1;

            tdtvd_tuner_core.agc_normal[i].ch_max   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_E;
            tdtvd_tuner_core.agc_eco[i].ch_max      = D_DTVD_TUNER_CORE_ECO_CH_GRP_E;
        }
        else if( i == 5 )
        {
            tdtvd_tuner_core.agc_normal[i].ch_min   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_E + 1;
            tdtvd_tuner_core.agc_eco[i].ch_min      = D_DTVD_TUNER_CORE_ECO_CH_GRP_E    + 1;

            tdtvd_tuner_core.agc_normal[i].ch_max   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_F;
            tdtvd_tuner_core.agc_eco[i].ch_max      = D_DTVD_TUNER_CORE_ECO_CH_GRP_F;
        }
        else if( i == 6 )
        {
            tdtvd_tuner_core.agc_normal[i].ch_min   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_F + 1;
            tdtvd_tuner_core.agc_eco[i].ch_min      = D_DTVD_TUNER_CORE_ECO_CH_GRP_F    + 1;

            tdtvd_tuner_core.agc_normal[i].ch_max   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_G;
            tdtvd_tuner_core.agc_eco[i].ch_max      = D_DTVD_TUNER_CORE_ECO_CH_GRP_G;
        }
        else
        {
            tdtvd_tuner_core.agc_normal[i].ch_min   = D_DTVD_TUNER_CORE_NORMAL_CH_GRP_G + 1;
            tdtvd_tuner_core.agc_eco[i].ch_min      = D_DTVD_TUNER_CORE_ECO_CH_GRP_G    + 1;

            tdtvd_tuner_core.agc_normal[i].ch_max   = D_DTVD_TUNER_CHANNEL_MAX;
            tdtvd_tuner_core.agc_eco[i].ch_max      = D_DTVD_TUNER_CHANNEL_MAX;
        }
    }
    return;
}

void dtvd_tuner_core_com_init_cnthr( void )
{

    tdtvd_tuner_core.cn_qpsk1_2_m       = tdtvd_tuner_nonvola.cn_qpsk1_2_m;
    tdtvd_tuner_core.cn_qpsk1_2_h       = tdtvd_tuner_nonvola.cn_qpsk1_2_h;
    tdtvd_tuner_core.cn_qpsk2_3_m       = tdtvd_tuner_nonvola.cn_qpsk2_3_m;
    tdtvd_tuner_core.cn_qpsk2_3_h       = tdtvd_tuner_nonvola.cn_qpsk2_3_h;
    tdtvd_tuner_core.cn_16qam1_2_m      = tdtvd_tuner_nonvola.cn_16qam1_2_m;
    tdtvd_tuner_core.cn_16qam1_2_h      = tdtvd_tuner_nonvola.cn_16qam1_2_h;
    tdtvd_tuner_core.cn_qpsk1_2_eco_m   = tdtvd_tuner_nonvola.cn_qpsk1_2_eco_m;
    tdtvd_tuner_core.cn_qpsk1_2_eco_h   = tdtvd_tuner_nonvola.cn_qpsk1_2_eco_h;
    tdtvd_tuner_core.cn_qpsk2_3_eco_m   = tdtvd_tuner_nonvola.cn_qpsk2_3_eco_m;
    tdtvd_tuner_core.cn_qpsk2_3_eco_h   = tdtvd_tuner_nonvola.cn_qpsk2_3_eco_h;
    tdtvd_tuner_core.cn_16qam1_2_eco_m  = tdtvd_tuner_nonvola.cn_16qam1_2_eco_m;
    tdtvd_tuner_core.cn_16qam1_2_eco_h  = tdtvd_tuner_nonvola.cn_16qam1_2_eco_h;

    return;
}

void dtvd_tuner_core_com_set_status
(
    signed int next_status              
)
{
    DTVD_DEBUG_MSG_ENTER( next_status, 0, 0 );

    tdtvd_tuner_core.status = next_status;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_set_result
(
    signed int result   
)
{
    DTVD_DEBUG_MSG_ENTER( result, 0, 0 );

    tdtvd_tuner_info.result = result;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_wakeup
(
    unsigned long   con_flg     
)
{
    DTVD_DEBUG_MSG_ENTER( (int)con_flg, 0, 0 );

    tdtvd_tuner_info.block_flg |= con_flg;

    wake_up_interruptible( &tdtvd_tuner_info.wait_queue_block );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_init_tuneflg( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.tune_cause = D_DTVD_TUNER_TUNE_NG_NONE;

    tdtvd_tuner_core.sync_flg = D_DTVD_TUNER_OFF;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_set_tuneinfo
(
    signed int  command     
)
{

    DTVD_TUNER_EXEVT_MDL_TUNE_DTV_t *tune;
    DTVD_TUNER_EXEVT_MDL_SCAN_DTV_t *scan;

    DTVD_DEBUG_MSG_ENTER( command, 0, 0 );

    dtvd_tuner_core_sync_time_start();

    if( command == D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV )
    {
        tune = &tdtvd_tuner_msg.buff.ex_mdl_tune_dtv;
        tdtvd_tuner_core.tune_ch = tune->ch;
        tdtvd_tuner_core.tune_adj = tune->adj;
        tdtvd_tuner_core.tune_kind = tune->kind;
        tdtvd_tuner_core.tune_seq_id = tune->seq_id;
    }
    else if( command == D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV )
    {
        scan = &tdtvd_tuner_msg.buff.ex_mdl_scan_dtv;
        tdtvd_tuner_core.tune_ch = scan->ch;
        tdtvd_tuner_core.tune_adj.mode = D_DTVD_TUNER_TSMODE_NONE;
        tdtvd_tuner_core.tune_adj.gi = D_DTVD_TUNER_GI_NONE;
        tdtvd_tuner_core.tune_kind = D_DTVD_TUNER_TUNE_KIND_SCAN;
        tdtvd_tuner_core.tune_seq_id = scan->seq_id;
    }
    else
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                  DTVD_TUNER_CORE_COM,
                                  0, 0, 0, 0, 0, 0 );
       return;
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

unsigned long dtvd_tuner_core_get_cn_seq_id( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tdtvd_tuner_core.cn_seq_id == (unsigned long)0xffffffff )
    {

        tdtvd_tuner_core.cn_seq_id = 0;
    }

    else
    {

        tdtvd_tuner_core.cn_seq_id++;
    }

    DTVD_DEBUG_MSG_EXIT();
    return tdtvd_tuner_core.cn_seq_id;
}

void dtvd_tuner_core_set_ng_cause
(
    unsigned char cause     
)
{
    DTVD_DEBUG_MSG_ENTER( cause, 0, 0 );

    tdtvd_tuner_core.tune_cause = cause;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

signed int dtvd_tuner_core_jdg_agc
(
    unsigned char tune_kind,        
    unsigned char x_agc,            
    unsigned char y_agc             
)
{
    unsigned char agc;

    unsigned char agc_thr = 0;

    unsigned char ch_no;
    signed int    result;

    DTVD_DEBUG_MSG_ENTER( tune_kind, x_agc, y_agc );

    if( x_agc >= y_agc )
    {
        agc = x_agc;
    }
    else
    {
        agc = y_agc;
    }

    if( tdtvd_tuner_core.eco_flag == D_DTVD_TUNER_ON )
    {
        agc = x_agc;
    }

    ch_no = tdtvd_tuner_core.tune_ch.no;
    if( ( ch_no < D_DTVD_TUNER_CHANNEL_MIN ) ||
        ( ch_no > D_DTVD_TUNER_CHANNEL_MAX ) )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_CORE_COM,
                                  ch_no, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_core_get_agcthr( tune_kind, ch_no, tdtvd_tuner_core.eco_flag, &agc_thr );

    if( agc < agc_thr )
    {

        result = D_DTVD_TUNER_NG;
    }
    else
    {

        result = D_DTVD_TUNER_OK;
    }

    DTVD_DEBUG_MSG_EXIT();
    return result;
}

void dtvd_tuner_core_deverr( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_trans_msg_deverr();

    dtvd_tuner_core_inevt_ber_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_core_sync_time_start( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.tune_start_jiff = (unsigned long)jiffies;
    tdtvd_tuner_core.tune_end_jiff = 0;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_sync_time_end( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_core.tune_end_jiff = (unsigned long)jiffies;
    tdtvd_tuner_monitor.tune.sync_time = jiffies - tdtvd_tuner_core.tune_start_jiff;

    tdtvd_tuner_monitor.tune.sync_time *= 10;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_set_tuneinfo_omt( void )
{
    DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT_t *tune;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_core_sync_time_start();

    tune = &tdtvd_tuner_msg.buff.ex_mdl_tune_dtv_omt;
    tdtvd_tuner_core.tune_ch.no = tune->ch_no;
    tdtvd_tuner_core.tune_ch.seg = 7;
    tdtvd_tuner_core.tune_adj.mode = D_DTVD_TUNER_TSMODE_NONE;
    tdtvd_tuner_core.tune_adj.gi = D_DTVD_TUNER_GI_NONE;
    tdtvd_tuner_core.tune_kind = 0;
    tdtvd_tuner_core.tune_seq_id = 0;

    tdtvd_tuner_core.omt_tune_kind = tune->tune_sync_kind;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_set_tune_result
(
    signed int result   
)
{
    DTVD_DEBUG_MSG_ENTER( result, 0, 0 );

    tdtvd_tuner_info.omt_tune_result = result;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_core_set_eco_flag( void )
{
    DTVD_TUNER_EXEVT_MDL_ECO_SET_t *eco;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    eco = &tdtvd_tuner_msg.buff.ex_mdl_eco_set;
    if( eco->mode == D_DTVD_TUNER_ECO_MODE_OFF )
    {
        tdtvd_tuner_core.eco_flag = D_DTVD_TUNER_OFF;
    }
    else
    {
        tdtvd_tuner_core.eco_flag = D_DTVD_TUNER_ON;
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_core_get_agcthr
(
    unsigned char tune_kind,        
    unsigned char ch,               
    unsigned char eco,              
    unsigned char *agc_thr          
)
{
    signed int i;
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( agc_thr == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_CORE_COM,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    if( eco == D_DTVD_TUNER_ON )
    {

        for( i = 0; i < D_DTVD_TUNER_CORE_CH_GRP_NUM; i++ )
        {
            if( ( ch >= tdtvd_tuner_core.agc_eco[i].ch_min ) &&
                ( ch <= tdtvd_tuner_core.agc_eco[i].ch_max ) )
            {
                break;
            }
        }

        if( i >= D_DTVD_TUNER_CORE_CH_GRP_NUM ){

            DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                      DTVD_TUNER_CORE_COM,
                                      0, 0, 0, 0, 0, 0 );
            return;
        }

        switch ( tune_kind )
        {

            case D_DTVD_TUNER_TUNE_KIND_NORMAL:
                *agc_thr = tdtvd_tuner_core.agc_eco[i].agc_thr1;
                break;

            case D_DTVD_TUNER_TUNE_KIND_SEARCH:
            case D_DTVD_TUNER_TUNE_KIND_SCAN:
                *agc_thr = tdtvd_tuner_core.agc_eco[i].agc_thr2;
                break;

            default:

                DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                          DTVD_TUNER_CORE_COM,
                                          0, 0, 0, 0, 0, 0 );
                break;
        }
    }

    else
    {
        for( i = 0; i < D_DTVD_TUNER_CORE_CH_GRP_NUM; i++ )
        {
            if( ( ch >= tdtvd_tuner_core.agc_normal[i].ch_min ) &&
                ( ch <= tdtvd_tuner_core.agc_normal[i].ch_max ) )
            {
                break;
            }
        }
        if( i >= D_DTVD_TUNER_CORE_CH_GRP_NUM ){

            DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                      DTVD_TUNER_CORE_COM,
                                      0, 0, 0, 0, 0, 0 );
            return;
        }

        switch ( tune_kind )
        {

            case D_DTVD_TUNER_TUNE_KIND_NORMAL:
                *agc_thr = tdtvd_tuner_core.agc_normal[i].agc_thr1;
                break;

            case D_DTVD_TUNER_TUNE_KIND_SEARCH:
            case D_DTVD_TUNER_TUNE_KIND_SCAN:
                *agc_thr = tdtvd_tuner_core.agc_normal[i].agc_thr2;
                break;

            default:

                DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                          DTVD_TUNER_CORE_COM,
                                          0, 0, 0, 0, 0, 0 );
                break;
        }
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}


