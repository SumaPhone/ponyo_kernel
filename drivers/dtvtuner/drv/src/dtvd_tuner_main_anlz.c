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
#include "dtvd_tuner_main.h"

static signed int dtvd_tuner_main_anlz_mdl_pwron( void );
static signed int dtvd_tuner_main_anlz_mdl_init( void );
static signed int dtvd_tuner_main_anlz_mdl_tune_dtv( void );
static signed int dtvd_tuner_main_anlz_mdl_scan_dtv( void );
static signed int dtvd_tuner_main_anlz_mdl_stop( void );
static signed int dtvd_tuner_main_anlz_mdl_end( void );
static signed int dtvd_tuner_main_anlz_mdl_pwroff( void );
static signed int dtvd_tuner_main_anlz_mdl_style( void );
static signed int dtvd_tuner_main_anlz_tunerdd_pre( void );
static signed int dtvd_tuner_main_anlz_tuner_int( void );
static signed int dtvd_tuner_main_anlz_timer_timeout( void );
static signed int dtvd_tuner_main_anlz_mdl_init_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_stop_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_tune_dtv_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_ber_start_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_ber_stop_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_ber_get_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_cnr_start_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_cnr_stop_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_cnr_get_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_agc_start_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_agc_stop_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_agc_get_omt( void );
static signed int dtvd_tuner_main_anlz_mdl_eco_set( void );
static signed int dtvd_tuner_main_anlz_tunerdd_state_mode_ok( void );
static signed int dtvd_tuner_main_anlz_tunerdd_state_sync( void );
static signed int dtvd_tuner_main_anlz_tunerdd_state_tmcc_ok( void );
static signed int dtvd_tuner_main_anlz_tunerdd_state_ts_start( void );
static signed int dtvd_tuner_main_anlz_tunerdd_state_async( void );

static signed int dtvd_tuner_main_anlz_timer_timeout( void )
{

    static const DTVD_TUNER_ANLZ_TIMER_TBL_t anlz_timeout_tbl[D_DTVD_TUNER_TIMER_MAX] =
    {

        { D_DTVD_TUNER_EVENT_NOP                 ,    D_DTVD_TUNER_CTL_UNDEF      },
        { D_DTVD_TUNER_EXEVT_TIMER_PWRON         ,    D_DTVD_TUNER_CTL_TUNER      },
        { D_DTVD_TUNER_EXEVT_TIMER_SEARCH        ,    D_DTVD_TUNER_CTL_TUNER      },
        { D_DTVD_TUNER_EXEVT_TIMER_SYNC          ,    D_DTVD_TUNER_CTL_TUNER      },
        { D_DTVD_TUNER_EXEVT_TIMER_TS            ,    D_DTVD_TUNER_CTL_TUNER      },
        { D_DTVD_TUNER_EXEVT_TIMER_ECO           ,    D_DTVD_TUNER_CTL_TUNER      },
        { D_DTVD_TUNER_EXEVT_TIMER_AGC           ,    D_DTVD_TUNER_CTL_WATCH_AGC  },
        { D_DTVD_TUNER_EXEVT_TIMER_AGC_WATCH     ,    D_DTVD_TUNER_CTL_WATCH_AGC  },
        { D_DTVD_TUNER_EXEVT_TIMER_CN_FST        ,    D_DTVD_TUNER_CTL_WATCH_CN   },
        { D_DTVD_TUNER_EXEVT_TIMER_CN_MEASURE    ,    D_DTVD_TUNER_CTL_WATCH_CN   },
        { D_DTVD_TUNER_EXEVT_TIMER_CN_WATCH      ,    D_DTVD_TUNER_CTL_WATCH_CN   },
        { D_DTVD_TUNER_EXEVT_TIMER_BER_FST       ,    D_DTVD_TUNER_CTL_WATCH_BER  },
        { D_DTVD_TUNER_EXEVT_TIMER_BER_MEASURE   ,    D_DTVD_TUNER_CTL_WATCH_BER  },
        { D_DTVD_TUNER_EXEVT_TIMER_BER_WATCH     ,    D_DTVD_TUNER_CTL_WATCH_BER  },
        { D_DTVD_TUNER_EXEVT_TIMER_STATE_WATCH   ,    D_DTVD_TUNER_CTL_WATCH_STATE},
        { D_DTVD_TUNER_EXEVT_TIMER_TMCC          ,    D_DTVD_TUNER_CTL_WATCH_TMCC },
        { D_DTVD_TUNER_EXEVT_TIMER_TMCC_WATCH    ,    D_DTVD_TUNER_CTL_WATCH_TMCC } 

       ,{ D_DTVD_TUNER_EXEVT_TIMER_AUTOECO_WATCH_OMT, D_DTVD_TUNER_CTL_AUTOECO    } 

    };

    DTVD_TUNER_RCVMSG_t* msgp = &tdtvd_tuner_msg;
    DTVD_TUNER_EXEVT_TIMEOUT_t* timeout;
    signed int evtcode;
    signed int funcid;

    timeout = &msgp->buff.ex_timeout;

    evtcode      = D_DTVD_TUNER_NOEVENT;
    msgp->toid   = D_DTVD_TUNER_CTL_UNDEF;

    for( funcid=0; funcid<D_DTVD_TUNER_TIMER_MAX; funcid++ )
    {
#ifdef DTVD_NO_HRTIMER
        if( tdtvd_tuner_main.timer.list[funcid].info.data == timeout->timer_id )
#else
        if( tdtvd_tuner_main.timer.list[funcid].data == timeout->timer_id )
#endif 
        {
            break;
        }
    }

    if( funcid >= D_DTVD_TUNER_TIMER_MAX )
    {

        evtcode = D_DTVD_TUNER_EVENT_NOP;
        return evtcode;
    }

    if( tdtvd_tuner_main.timer.list[funcid].state == D_DTVD_TUNER_TIMER_STOP )
    {

        evtcode = D_DTVD_TUNER_EVENT_NOP;
        return evtcode;
    }

    tdtvd_tuner_main.timer.list[funcid].state = D_DTVD_TUNER_TIMER_STOP;

    evtcode    = anlz_timeout_tbl[funcid].evtcode;
    msgp->toid = anlz_timeout_tbl[funcid].ctlid;

    return evtcode;
}

static signed int dtvd_tuner_main_anlz_mdl_pwron( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;
    return D_DTVD_TUNER_EXEVT_MDL_PWRON;
}

static signed int dtvd_tuner_main_anlz_mdl_init( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_INIT;
}

static signed int dtvd_tuner_main_anlz_mdl_tune_dtv( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV;
}

static signed int dtvd_tuner_main_anlz_mdl_scan_dtv( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV;
}

static signed int dtvd_tuner_main_anlz_mdl_stop( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_STOP;
}

static signed int dtvd_tuner_main_anlz_mdl_end( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_END;
}

static signed int dtvd_tuner_main_anlz_mdl_pwroff( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_PWROFF;
}

static signed int dtvd_tuner_main_anlz_mdl_init_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_INIT_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_stop_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_STOP_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_tune_dtv_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_ber_start_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_BER_START_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_ber_stop_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_BER_STOP_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_ber_get_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_BER_GET_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_cnr_start_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_CNR_START_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_cnr_stop_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_CNR_STOP_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_cnr_get_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_CNR_GET_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_agc_start_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_AGC_START_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_agc_stop_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_AGC_STOP_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_agc_get_omt( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_AGC_GET_OMT;
}

static signed int dtvd_tuner_main_anlz_mdl_eco_set( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_MDL_ECO_SET;
}

static signed int dtvd_tuner_main_anlz_tunerdd_pre( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_CORE;

    return D_DTVD_TUNER_EXEVT_TUNERDD_PRE;
}

static signed int dtvd_tuner_main_anlz_mdl_style( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_ANT;

    return D_DTVD_TUNER_EXEVT_MDL_STYLE;
}

static signed int dtvd_tuner_main_anlz_tuner_int( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_TUNER;

    return D_DTVD_TUNER_EXEVT_TUNER_INT;
}

static signed int dtvd_tuner_main_anlz_tunerdd_state_mode_ok( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_TUNER;

    return D_DTVD_TUNER_EXEVT_TUNERDD_STATE_MODE_OK;
}

static signed int dtvd_tuner_main_anlz_tunerdd_state_sync( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_TUNER;

    return D_DTVD_TUNER_EXEVT_TUNERDD_STATE_SYNC;
}

static signed int dtvd_tuner_main_anlz_tunerdd_state_tmcc_ok( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_TUNER;

    return D_DTVD_TUNER_EXEVT_TUNERDD_STATE_TMCC_OK;
}

static signed int dtvd_tuner_main_anlz_tunerdd_state_ts_start( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_TUNER;

    return D_DTVD_TUNER_EXEVT_TUNERDD_STATE_TS_START;
}

static signed int dtvd_tuner_main_anlz_tunerdd_state_async( void )
{

    tdtvd_tuner_msg.toid = D_DTVD_TUNER_CTL_TUNER;

    return D_DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC;
}

const DTVD_TUNER_EVENT_TBL_t* dtvd_tuner_main_anlz_get_table( void )
{

    static const DTVD_TUNER_EVENT_ANLZ_TBL_t DTVD_TUNER_ATTRIBUTE anlz_mdl_tbl[] =
    {
        { D_DTVD_TUNER_CMD_MDL_PWRON,        dtvd_tuner_main_anlz_mdl_pwron         },
        { D_DTVD_TUNER_CMD_MDL_INIT,         dtvd_tuner_main_anlz_mdl_init          },
        { D_DTVD_TUNER_CMD_MDL_TUNE_DTV,     dtvd_tuner_main_anlz_mdl_tune_dtv      },
        { D_DTVD_TUNER_CMD_MDL_SCAN_DTV,     dtvd_tuner_main_anlz_mdl_scan_dtv      },
        { D_DTVD_TUNER_CMD_MDL_STOP,         dtvd_tuner_main_anlz_mdl_stop          },
        { D_DTVD_TUNER_CMD_MDL_END,          dtvd_tuner_main_anlz_mdl_end           },
        { D_DTVD_TUNER_CMD_MDL_PWROFF,       dtvd_tuner_main_anlz_mdl_pwroff        },
        { D_DTVD_TUNER_CMD_MDL_STYLE,        dtvd_tuner_main_anlz_mdl_style         },
        { D_DTVD_TUNER_CMD_MDL_ECO_SET,      dtvd_tuner_main_anlz_mdl_eco_set       },
        { D_DTVD_TUNER_CMD_MDL_INIT_OMT,     dtvd_tuner_main_anlz_mdl_init_omt      },
        { D_DTVD_TUNER_CMD_MDL_STOP_OMT,     dtvd_tuner_main_anlz_mdl_stop_omt      },
        { D_DTVD_TUNER_CMD_MDL_TUNE_DTV_OMT, dtvd_tuner_main_anlz_mdl_tune_dtv_omt  },
        { D_DTVD_TUNER_CMD_MDL_BER_START_OMT,dtvd_tuner_main_anlz_mdl_ber_start_omt },
        { D_DTVD_TUNER_CMD_MDL_BER_STOP_OMT, dtvd_tuner_main_anlz_mdl_ber_stop_omt  },
        { D_DTVD_TUNER_CMD_MDL_BER_GET_OMT,  dtvd_tuner_main_anlz_mdl_ber_get_omt   },
        { D_DTVD_TUNER_CMD_MDL_CNR_START_OMT,dtvd_tuner_main_anlz_mdl_cnr_start_omt },
        { D_DTVD_TUNER_CMD_MDL_CNR_STOP_OMT, dtvd_tuner_main_anlz_mdl_cnr_stop_omt  },
        { D_DTVD_TUNER_CMD_MDL_CNR_GET_OMT,  dtvd_tuner_main_anlz_mdl_cnr_get_omt   },
        { D_DTVD_TUNER_CMD_MDL_AGC_START_OMT,dtvd_tuner_main_anlz_mdl_agc_start_omt },
        { D_DTVD_TUNER_CMD_MDL_AGC_STOP_OMT, dtvd_tuner_main_anlz_mdl_agc_stop_omt  },
        { D_DTVD_TUNER_CMD_MDL_AGC_GET_OMT,  dtvd_tuner_main_anlz_mdl_agc_get_omt   },
        { 0, D_DTVD_TUNER_NULL }  
    };

    static const DTVD_TUNER_EVENT_ANLZ_TBL_t DTVD_TUNER_ATTRIBUTE anlz_tunerdd_tbl[] =
    {
        { D_DTVD_TUNER_CMD_TUNERDD_TUNEOK,          dtvd_tuner_main_anlz_tunerdd_pre    }, 
        { D_DTVD_TUNER_CMD_TUNERDD_STATE_MODE_OK,   dtvd_tuner_main_anlz_tunerdd_state_mode_ok },
        { D_DTVD_TUNER_CMD_TUNERDD_STATE_SYNC,      dtvd_tuner_main_anlz_tunerdd_state_sync    },
        { D_DTVD_TUNER_CMD_TUNERDD_STATE_TMCC_OK,   dtvd_tuner_main_anlz_tunerdd_state_tmcc_ok },
        { D_DTVD_TUNER_CMD_TUNERDD_STATE_TS_START,  dtvd_tuner_main_anlz_tunerdd_state_ts_start},
        { D_DTVD_TUNER_CMD_TUNERDD_STATE_ASYNC,     dtvd_tuner_main_anlz_tunerdd_state_async   },
        { 0, D_DTVD_TUNER_NULL }  
    };

    static const DTVD_TUNER_EVENT_ANLZ_TBL_t DTVD_TUNER_ATTRIBUTE anlz_tuner_tbl[] =
    {
        { D_DTVD_TUNER_CMD_TUNER_INT,          dtvd_tuner_main_anlz_tuner_int   }, 
        { 0, D_DTVD_TUNER_NULL }  
    };

    static const DTVD_TUNER_EVENT_ANLZ_TBL_t DTVD_TUNER_ATTRIBUTE anlz_timer_tbl[] =
    {
        { D_DTVD_TUNER_CMD_TIMER_TIMEOUT,       dtvd_tuner_main_anlz_timer_timeout  }, 
        { 0, D_DTVD_TUNER_NULL }  
    };

    static const DTVD_TUNER_EVENT_TBL_t DTVD_TUNER_ATTRIBUTE anlz_tbl[] =
    {
        { D_DTVD_TUNER_BLK_MIDDLE,  (DTVD_TUNER_EVENT_ANLZ_TBL_t*)anlz_mdl_tbl    },
        { D_DTVD_TUNER_BLK_TUNER,   (DTVD_TUNER_EVENT_ANLZ_TBL_t*)anlz_tuner_tbl  },
        { D_DTVD_TUNER_BLK_TIMER,   (DTVD_TUNER_EVENT_ANLZ_TBL_t*)anlz_timer_tbl  },
        { D_DTVD_TUNER_BLK_TUNERDD, (DTVD_TUNER_EVENT_ANLZ_TBL_t*)anlz_tunerdd_tbl},
        { D_DTVD_TUNER_BLK_UNDEF, D_DTVD_TUNER_NULL }   
    };

    return anlz_tbl;
}

