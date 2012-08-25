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
#include "dtvd_tuner_core.h"
#include "dtvd_tuner_cit.h"

static void dtvd_tuner_tuner_mtx_003_com_set_tuneinfo_omt( unsigned char * );

static signed int dtvd_tuner_tuner_mtx_003_com_tune_common(
    unsigned char eco_mode,
    unsigned int kind
);

static signed int dtvd_tuner_tuner_mtx_003_com_tune_normal_proc( unsigned char );
static signed int dtvd_tuner_tuner_mtx_003_com_tune_omt( unsigned char );
static signed int dtvd_tuner_tuner_mtx_003_com_tune_search( unsigned int );
static signed int dtvd_tuner_tuner_mtx_003_com_tune_normal( void );

static void dtvd_tuner_tuner_mtx_003_com_set_tuneinfo( unsigned char * );

void dtvd_tuner_tuner_mtx_003_msg_core_init( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_tuner_com_tuner_reset();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_003_msg_core_end( void )
{

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_tmcc_tuner_end();

    dtvd_tuner_tuner_inevt_state_tuner_end();

    dtvd_tuner_tuner_com_term();

    dtvd_tuner_tuner_inevt_core_end();

    dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_002 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_003_msg_core_tune_dtv( void )
{
    signed   int    ret;
    unsigned char   kind;
    signed int      next_sts;
    signed   int    result;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_mtx_003_com_set_tuneinfo( &kind );

    switch ( kind )
    {

        case D_DTVD_TUNER_TUNE_KIND_NORMAL:
            DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

            ret = dtvd_tuner_tuner_mtx_003_com_tune_normal();

            tdtvd_tuner_tuner.sync_wait_flg = D_DTVD_TUNER_ON;  

            next_sts = D_DTVD_TUNER_TUNER_005;          
            break;

        case D_DTVD_TUNER_TUNE_KIND_SEARCH:
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );
//[2011/09/21 simt-leijun]{
            set_cit_static_variable(SCANNING_CHANNEL_NUM, &tdtvd_tuner_tuner.ch.no);
//[2011/09/21 simt-leijun]}

        case D_DTVD_TUNER_TUNE_KIND_SCAN:

            DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

            ret = dtvd_tuner_tuner_mtx_003_com_tune_search( kind );

            tdtvd_tuner_tuner.sync_wait_flg = D_DTVD_TUNER_OFF; 

            next_sts = D_DTVD_TUNER_TUNER_004;          
            break;

        default:
            DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

            DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_ETC,
                                      DTVD_TUNER_TUNER_MTX003,
                                      kind, 0, 0, 0, 0, 0 );
            DTVD_DEBUG_MSG_EXIT();
            return;
    }

    result = ret;

    dtvd_tuner_tuner_inevt_core_tune_set( (unsigned int)result );

    dtvd_tuner_tuner_com_set_status( next_sts );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_003_msg_core_stop( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_core_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_003_msg_core_init_omt( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_tuner_com_tuner_reset();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr_omt();
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_003_msg_core_tune_dtv_omt( void )
{
    signed int      ret;
    unsigned char   omt_sync;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_mtx_003_com_set_tuneinfo_omt( &omt_sync );

    ret = dtvd_tuner_tuner_mtx_003_com_tune_omt( omt_sync );

    dtvd_tuner_tuner_inevt_core_tune_set( (unsigned int)ret );

    tdtvd_tuner_tuner.sync_wait_flg = D_DTVD_TUNER_OFF; 

    if ( omt_sync == D_DTVD_TUNER_DTVTUNE_SYNC_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_004 );

    }
    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        dtvd_tuner_tuner_com_set_status( D_DTVD_TUNER_TUNER_010 );
    }

    DTVD_DEBUG_MSG_EXIT();
    return;

}

void dtvd_tuner_tuner_mtx_003_msg_core_stop_omt( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_inevt_core_stop_omt();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_003_msg_core_eco_set( void )
{
    DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = &tdtvd_tuner_msg.buff.in_tuner_core_eco_set;
    tdtvd_tuner_tuner.eco_mode = data->mode;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_tuner_mtx_003_msg_tmcc_stop( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_com_shift_stop();

    ret = dtvd_tuner_tuner_com_tuner_stop();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        dtvd_tuner_tuner_inevt_core_deverr();

        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    dtvd_tuner_tuner_inevt_core_stop();

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_tuner_mtx_003_com_set_tuneinfo( unsigned char *kind )
{
    DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if ( kind == D_DTVD_TUNER_NULL )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TUNER_COM,
                                  0, 0, 0, 0, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    data = &tdtvd_tuner_msg.buff.in_tuner_core_tune_dtv;

    tdtvd_tuner_tuner.ch = data->ch;
    tdtvd_tuner_tuner.adj = data->adj;

    tdtvd_tuner_monitor.tune.ch = tdtvd_tuner_tuner.ch;
    tdtvd_tuner_monitor.tune.sync_state = D_DTVD_TUNER_SYNC_STATE_ASYNC;
    tdtvd_tuner_monitor.tune.sync_time = 0;

    *kind = data->kind;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static void dtvd_tuner_tuner_mtx_003_com_set_tuneinfo_omt
(
    unsigned char *tune_sync    
)
{
    DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_OMT_t *data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( tune_sync == D_DTVD_TUNER_NULL )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TUNER_MTX003,
                                  0, 0, 0, 0, 0, 0 );
        DTVD_DEBUG_MSG_EXIT();
        return;
    }

    data = &tdtvd_tuner_msg.buff.in_tuner_core_tune_dtv_omt;

    tdtvd_tuner_tuner.ch.no = data->ch_no;
    tdtvd_tuner_tuner.ch.seg = 7;
    tdtvd_tuner_tuner.adj.mode = D_DTVD_TUNER_TSMODE_NONE;
    tdtvd_tuner_tuner.adj.gi = D_DTVD_TUNER_GI_NONE;

    tdtvd_tuner_monitor.tune.ch = tdtvd_tuner_tuner.ch;
    tdtvd_tuner_monitor.tune.sync_state = D_DTVD_TUNER_SYNC_STATE_ASYNC;
    tdtvd_tuner_monitor.tune.sync_time = 0;

    *tune_sync = data->sync;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

static signed int dtvd_tuner_tuner_mtx_003_com_tune_normal( void )
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_tuner_mtx_003_com_tune_normal_proc(
            tdtvd_tuner_tuner.eco_mode    );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_tuner_inevt_state_tuner_start();

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_SYNC,
                            tdtvd_tuner_tuner.wait_sync_time );

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;

}

static signed int dtvd_tuner_tuner_mtx_003_com_tune_omt
(
    unsigned char sync
)
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if ( sync == D_DTVD_TUNER_DTVTUNE_SYNC_ON )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        ret = dtvd_tuner_tuner_mtx_003_com_tune_search (
                D_DTVD_TUNER_TUNE_KIND_SEARCH );
    }
    else
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        ret = dtvd_tuner_tuner_mtx_003_com_tune_normal_proc (
                   D_DTVD_TUNER_ECO_MODE_OFF );
    }

    DTVD_DEBUG_MSG_EXIT();
    return ret;

}

static signed int dtvd_tuner_tuner_mtx_003_com_tune_normal_proc
(
    unsigned char eco_mode
)
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = dtvd_tuner_tuner_com_sequence_init();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    dtvd_tuner_com_dev_int_disable();

    ret = dtvd_tuner_tuner_com_tuner_int_disable();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = dtvd_tuner_tuner_com_set_mode_tmcc_info( D_DTVD_TUNER_TUNE_KIND_NORMAL );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = dtvd_tuner_tuner_mtx_003_com_tune_common(eco_mode, D_DTVD_TUNER_TUNE_KIND_NORMAL);

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;

}


extern signed int init_for_channel_scan(void);
extern unsigned char get_scan_channel_num(void);
static signed int dtvd_tuner_tuner_mtx_003_com_tune_search( unsigned int kind )
{
    signed int ret;
    unsigned char current_channel;
	
//[2011/09/21 simt-leijun]{
    ret = init_for_channel_scan();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
//[2011/09/21 simt-leijun]}

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_tuner_com_search_set();

    dtvd_tuner_com_dev_int_disable();

    ret = dtvd_tuner_tuner_com_sequence_init();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = dtvd_tuner_tuner_com_tuner_int_clear();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = dtvd_tuner_tuner_com_tuner_int_disable();
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    current_channel = get_scan_channel_num();
    if ( !current_channel_isSPec(current_channel))
    {
	
        ret = dtvd_tuner_tuner_com_set_mode_tmcc_info( kind );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        ret = dtvd_tuner_tuner_com_search_count_set( kind );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 5, 0, 0 );

            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        ret = dtvd_tuner_tuner_com_search_resreg_set();
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 6, 0, 0 );

            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        ret = dtvd_tuner_tuner_com_sequence_init();
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 7, 0, 0 );

            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        ret = dtvd_tuner_tuner_mtx_003_com_tune_common(tdtvd_tuner_tuner.eco_mode, kind );

        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 8, 0, 0 );

            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        dtvd_tuner_com_dev_int_enable();

        ret = dtvd_tuner_tuner_com_tuner_int_enable();
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 9, 0, 0 );

            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        ret = dtvd_tuner_tuner_com_search_start();
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 10, 0, 0 );

            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }



        dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_SEARCH,
                                              tdtvd_tuner_tuner.wait_search_time );
    }else {
        ret = dtv_request_channel_scan( kind);
        if (ret != D_DTVD_TUNER_OK)
            return    D_DTVD_TUNER_NG;		 	
    }

    DTVD_DEBUG_MSG_EXIT();
  
    return D_DTVD_TUNER_OK;

	
}

static signed int dtvd_tuner_tuner_mtx_003_com_tune_common
(

    unsigned char eco_mode,
    unsigned int kind

)
{
    signed int ret;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if ( eco_mode == D_DTVD_TUNER_ECO_MODE_OFF )
    {
        DTVD_DEBUG_MSG_ENTER( 1, 0, 0 );

        ret = dtvd_tuner_tuner_com_rficpow();
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 2, 0, 0 );

            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
    }
    else
    {
        DTVD_DEBUG_MSG_ENTER( 3, 0, 0 );

        ret = dtvd_tuner_tuner_com_rficpow_eco();
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_ENTER( 4, 0, 0 );

            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
    }

    ret = dtvd_tuner_tuner_com_rfic_send_tuneset( tdtvd_tuner_tuner.ch.no, kind);

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_ENTER( 5, 0, 0 );

        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;

}

