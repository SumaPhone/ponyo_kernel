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
#include "dtvd_tuner_mtx.h"

#ifdef  __GNUC__
    const unsigned int dummy_tuner_main_mtx=0xFEFEFEFE;
#endif   

const DTVD_TUNER_MTX_TBL_t* dtvd_tuner_main_mtx_get_table( void )
{

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core000_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_PWRON        ,   dtvd_tuner_core_mtx_000_pwron   },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_PWRON ,   dtvd_tuner_core_mtx_000_msg_tuner_pwron },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core001_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_PWROFF       ,   dtvd_tuner_core_mtx_001_pwroff  },
        {   D_DTVD_TUNER_EXEVT_MDL_INIT         ,   dtvd_tuner_core_mtx_001_init    },
        {   D_DTVD_TUNER_EXEVT_MDL_INIT_OMT     ,   dtvd_tuner_core_mtx_001_init_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_ANT_INIT    ,   dtvd_tuner_core_mtx_001_msg_ant_init    },
        {   D_DTVD_TUNER_INEVT_CORE_ANT_INIT_OMT,   dtvd_tuner_core_mtx_001_msg_ant_init_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_PWROFF,   dtvd_tuner_core_mtx_001_msg_tuner_pwroff    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_INIT  ,   dtvd_tuner_core_mtx_001_msg_tuner_init  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   dtvd_tuner_core_mtx_001_msg_tuner_deverr    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_INIT_OMT, dtvd_tuner_core_mtx_001_msg_tuner_init_omt  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT,   dtvd_tuner_core_mtx_001_msg_tuner_deverr_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_INIT    ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_INIT_OMT,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_CN_INIT     ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_CN_INIT_OMT ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_BER_INIT    ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_BER_INIT_OMT,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core002_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_END          ,   dtvd_tuner_core_mtx_002_end },
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV     ,   dtvd_tuner_core_mtx_002_tune_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV     ,   dtvd_tuner_core_mtx_002_scan_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_STOP         ,   dtvd_tuner_core_mtx_002_stop    },
        {   D_DTVD_TUNER_EXEVT_MDL_STOP_OMT     ,   dtvd_tuner_core_mtx_002_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT ,   dtvd_tuner_core_mtx_002_tune_dtv_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_ECO_SET      ,   dtvd_tuner_core_mtx_002_eco_set },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_PRE      ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_ANT_END     ,   dtvd_tuner_core_mtx_002_msg_ant_end },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_END   ,   dtvd_tuner_core_mtx_002_msg_tuner_end   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_END     ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_CN_END      ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_BER_END     ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core003_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV     ,   dtvd_tuner_core_mtx_003_tune_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV     ,   dtvd_tuner_core_mtx_003_scan_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_STOP         ,   dtvd_tuner_core_mtx_003_stop    },
        {   D_DTVD_TUNER_EXEVT_MDL_ECO_SET      ,   dtvd_tuner_core_mtx_003_eco_set },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET  ,   dtvd_tuner_core_mtx_003_msg_tuner_tune_set  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TSOUT ,   dtvd_tuner_core_mtx_003_msg_tuner_tsout },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TUNE_NG   ,   dtvd_tuner_core_mtx_003_msg_tuner_tune_ng   },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_NONSTD,   dtvd_tuner_core_mtx_003_msg_tuner_nonstd    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_SYNC  ,   dtvd_tuner_core_mtx_003_msg_tuner_sync  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_ASYNC ,   dtvd_tuner_core_mtx_003_msg_tuner_async },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   dtvd_tuner_core_mtx_003_msg_tuner_deverr    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TMCC  ,   dtvd_tuner_core_mtx_003_msg_tuner_errcore   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST ,   dtvd_tuner_core_mtx_003_msg_agc_measure_fst },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE ,   dtvd_tuner_core_mtx_003_msg_agc_measure },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_DEVERR  ,   dtvd_tuner_core_mtx_003_msg_agc_deverr  },
        {   D_DTVD_TUNER_INEVT_CORE_TMCC_DEVERR ,   dtvd_tuner_core_mtx_003_msg_tmcc_deverr },
        {   D_DTVD_TUNER_INEVT_CORE_CN_MEASURE  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_CN_STOP     ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_CN_DEVERR   ,   dtvd_tuner_core_mtx_003_msg_cn_deverr   },
        {   D_DTVD_TUNER_INEVT_CORE_BER_STOP    ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_BER_DEVERR  ,   dtvd_tuner_core_mtx_003_msg_ber_deverr  },

        {   D_DTVD_TUNER_INEVT_CORE_STATE_DEVERR,   dtvd_tuner_core_mtx_003_msg_state_deverr  },

        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core004_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV     ,   dtvd_tuner_core_mtx_004_tune_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV     ,   dtvd_tuner_core_mtx_004_scan_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_STOP         ,   dtvd_tuner_core_mtx_004_stop    },
        {   D_DTVD_TUNER_EXEVT_MDL_ECO_SET      ,   dtvd_tuner_core_mtx_004_eco_set },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET  ,   dtvd_tuner_core_mtx_004_msg_tuner_tune_set  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TSOUT ,   dtvd_tuner_core_mtx_004_msg_tuner_tsout },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TUNE_NG   ,   dtvd_tuner_core_mtx_004_msg_tuner_tune_ng   },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_NONSTD,   dtvd_tuner_core_mtx_004_msg_tuner_nonstd    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_SYNC  ,   dtvd_tuner_core_mtx_004_msg_tuner_sync  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_ASYNC ,   dtvd_tuner_core_mtx_004_msg_tuner_async },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   dtvd_tuner_core_mtx_004_msg_tuner_deverr    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TMCC  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST ,   dtvd_tuner_core_mtx_004_msg_agc_measure_fst },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE ,   dtvd_tuner_core_mtx_004_msg_agc_measure },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_DEVERR  ,   dtvd_tuner_core_mtx_004_msg_agc_deverr  },
        {   D_DTVD_TUNER_INEVT_CORE_TMCC_DEVERR ,   dtvd_tuner_core_mtx_004_msg_tmcc_deverr },

        {   D_DTVD_TUNER_INEVT_CORE_STATE_DEVERR,   dtvd_tuner_core_mtx_004_msg_state_deverr },

        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core005_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV     ,   dtvd_tuner_core_mtx_005_tune_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV     ,   dtvd_tuner_core_mtx_005_scan_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_STOP         ,   dtvd_tuner_core_mtx_005_stop    },
        {   D_DTVD_TUNER_EXEVT_MDL_ECO_SET      ,   dtvd_tuner_core_mtx_005_eco_set },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TSOUT ,   dtvd_tuner_core_mtx_005_msg_tuner_tsout },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_NONSTD,   dtvd_tuner_core_mtx_005_msg_tuner_nonstd    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_SYNC  ,   dtvd_tuner_core_mtx_005_msg_tuner_sync  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_ASYNC ,   dtvd_tuner_core_mtx_005_msg_tuner_async },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   dtvd_tuner_core_mtx_005_msg_tuner_deverr    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TMCC  ,   dtvd_tuner_core_mtx_005_msg_tuner_errcore   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_DEVERR  ,   dtvd_tuner_core_mtx_005_msg_agc_deverr  },
        {   D_DTVD_TUNER_INEVT_CORE_TMCC_DEVERR ,   dtvd_tuner_core_mtx_005_msg_tmcc_deverr },
        {   D_DTVD_TUNER_INEVT_CORE_CN_MEASURE  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_CN_STOP     ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_CN_DEVERR   ,   dtvd_tuner_core_mtx_005_msg_cn_deverr   },
        {   D_DTVD_TUNER_INEVT_CORE_BER_STOP    ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_BER_DEVERR  ,   dtvd_tuner_core_mtx_005_msg_ber_deverr  },

        {   D_DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE  ,   dtvd_tuner_core_mtx_005_msg_autoeco_notice  },
        {   D_DTVD_TUNER_INEVT_CORE_AUTOECO_DEVERR  ,   dtvd_tuner_core_mtx_005_msg_autoeco_deverr  },

        {   D_DTVD_TUNER_INEVT_CORE_STATE_DEVERR,   dtvd_tuner_core_mtx_005_msg_state_deverr  },

        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core006_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV     ,   dtvd_tuner_core_mtx_006_tune_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV     ,   dtvd_tuner_core_mtx_006_scan_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_STOP         ,   dtvd_tuner_core_mtx_006_stop    },
        {   D_DTVD_TUNER_EXEVT_MDL_ECO_SET      ,   dtvd_tuner_core_mtx_006_eco_set },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TSOUT ,   dtvd_tuner_core_mtx_006_msg_tuner_tsout },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_NONSTD,   dtvd_tuner_core_mtx_006_msg_tuner_nonstd    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_SYNC  ,   dtvd_tuner_core_mtx_006_msg_tuner_sync  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_ASYNC ,   dtvd_tuner_core_mtx_006_msg_tuner_async },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   dtvd_tuner_core_mtx_006_msg_tuner_deverr    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TMCC  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_DEVERR  ,   dtvd_tuner_core_mtx_006_msg_agc_deverr  },
        {   D_DTVD_TUNER_INEVT_CORE_TMCC_DEVERR ,   dtvd_tuner_core_mtx_006_msg_tmcc_deverr },

        {   D_DTVD_TUNER_INEVT_CORE_STATE_DEVERR,   dtvd_tuner_core_mtx_006_msg_state_deverr },

        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core007_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV     ,   dtvd_tuner_core_mtx_007_tune_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV     ,   dtvd_tuner_core_mtx_007_scan_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_STOP         ,   dtvd_tuner_core_mtx_007_stop    },
        {   D_DTVD_TUNER_EXEVT_MDL_ECO_SET      ,   dtvd_tuner_core_mtx_007_eco_set },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_PRE      ,   dtvd_tuner_core_mtx_007_tunerdd_pre },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_STOP  ,   dtvd_tuner_core_mtx_007_msg_tuner_stop  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   dtvd_tuner_core_mtx_007_msg_tuner_deverr    },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_STOP    ,   dtvd_tuner_core_mtx_007_msg_agc_stop    },
        {   D_DTVD_TUNER_INEVT_CORE_CN_STOP     ,   dtvd_tuner_core_mtx_007_msg_cn_stop },
        {   D_DTVD_TUNER_INEVT_CORE_BER_STOP    ,   dtvd_tuner_core_mtx_007_msg_ber_stop    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core008_tbl[] = {
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_STOP  ,   dtvd_tuner_core_mtx_008_msg_tuner_stop  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   dtvd_tuner_core_mtx_008_msg_tuner_deverr    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_STOP_OMT  ,   dtvd_tuner_core_mtx_008_msg_tuner_stop_omt  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT,   dtvd_tuner_core_mtx_008_msg_tuner_deverr_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_STOP    ,   dtvd_tuner_core_mtx_008_msg_agc_stop    },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_STOP_OMT,   dtvd_tuner_core_mtx_008_msg_agc_stop_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_CN_STOP     ,   dtvd_tuner_core_mtx_008_msg_cn_stop },
        {   D_DTVD_TUNER_INEVT_CORE_CN_STOP_OMT ,   dtvd_tuner_core_mtx_008_msg_cn_stop_omt },
        {   D_DTVD_TUNER_INEVT_CORE_BER_STOP    ,   dtvd_tuner_core_mtx_008_msg_ber_stop    },
        {   D_DTVD_TUNER_INEVT_CORE_BER_STOP_OMT,   dtvd_tuner_core_mtx_008_msg_ber_stop_omt    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core009_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV     ,   dtvd_tuner_core_mtx_009_tune_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV     ,   dtvd_tuner_core_mtx_009_scan_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_STOP         ,   dtvd_tuner_core_mtx_009_stop    },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_PRE      ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_STOP  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_STOP    ,   dtvd_tuner_core_mtx_009_msg_agc_stop    },
        {   D_DTVD_TUNER_INEVT_CORE_CN_STOP     ,   dtvd_tuner_core_mtx_009_msg_cn_stop },
        {   D_DTVD_TUNER_INEVT_CORE_BER_STOP    ,   dtvd_tuner_core_mtx_009_msg_ber_stop    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core010_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_STOP_OMT     ,   dtvd_tuner_core_mtx_010_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT ,   dtvd_tuner_core_mtx_010_tune_dtv_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_AGC_START_OMT,   dtvd_tuner_core_mtx_010_agc_start_omt   },
        {   D_DTVD_TUNER_EXEVT_MDL_AGC_STOP_OMT ,   dtvd_tuner_core_mtx_010_agc_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_AGC_GET_OMT  ,   dtvd_tuner_core_mtx_010_agc_get_omt },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TUNE_SET, dtvd_tuner_core_mtx_010_msg_tuner_tune_set  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TSOUT ,   dtvd_tuner_core_mtx_010_msg_tuner_tsout },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TUNE_NG,  dtvd_tuner_core_mtx_010_msg_tuner_tune_ng   },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_NONSTD,   dtvd_tuner_core_mtx_010_msg_tuner_nonstd    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_SYNC  ,   dtvd_tuner_core_mtx_010_msg_tuner_sync  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_ASYNC ,   dtvd_tuner_core_mtx_010_msg_tuner_async },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   dtvd_tuner_core_mtx_010_msg_tuner_deverr    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT,   dtvd_tuner_core_mtx_010_msg_tuner_deverr_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TMCC  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT,    dtvd_tuner_core_mtx_010_msg_agc_measure_fst_omt },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_START_OMT,  dtvd_tuner_core_mtx_010_msg_agc_start_omt   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_STOP_OMT,   dtvd_tuner_core_mtx_010_msg_agc_measure_stop_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_OMT,    dtvd_tuner_core_mtx_010_msg_agc_measure_omt },
        {   D_DTVD_TUNER_INEVT_CORE_TMCC_DEVERR ,   dtvd_tuner_core_mtx_010_msg_tmcc_deverr },

        {   D_DTVD_TUNER_INEVT_CORE_STATE_DEVERR,   dtvd_tuner_core_mtx_010_msg_state_deverr },

        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core011_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_STOP_OMT     ,   dtvd_tuner_core_mtx_011_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT ,   dtvd_tuner_core_mtx_011_tune_dtv_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_BER_START_OMT,   dtvd_tuner_core_mtx_011_ber_start_omt   },
        {   D_DTVD_TUNER_EXEVT_MDL_BER_STOP_OMT ,   dtvd_tuner_core_mtx_011_ber_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_BER_GET_OMT  ,   dtvd_tuner_core_mtx_011_ber_get_omt },
        {   D_DTVD_TUNER_EXEVT_MDL_CNR_START_OMT,   dtvd_tuner_core_mtx_011_cnr_start_omt   },
        {   D_DTVD_TUNER_EXEVT_MDL_CNR_STOP_OMT ,   dtvd_tuner_core_mtx_011_cnr_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_CNR_GET_OMT  ,   dtvd_tuner_core_mtx_011_cnr_get_omt },
        {   D_DTVD_TUNER_EXEVT_MDL_AGC_START_OMT,   dtvd_tuner_core_mtx_011_agc_start_omt   },
        {   D_DTVD_TUNER_EXEVT_MDL_AGC_STOP_OMT ,   dtvd_tuner_core_mtx_011_agc_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_AGC_GET_OMT  ,   dtvd_tuner_core_mtx_011_agc_get_omt },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TSOUT ,   dtvd_tuner_core_mtx_011_msg_tuner_tsout },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_NONSTD,   dtvd_tuner_core_mtx_011_msg_tuner_nonstd    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_SYNC  ,   dtvd_tuner_core_mtx_011_msg_tuner_sync  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_ASYNC ,   dtvd_tuner_core_mtx_011_msg_tuner_async },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   dtvd_tuner_core_mtx_011_msg_tuner_deverr    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_TMCC  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_FST_OMT ,   dtvd_tuner_core_mtx_011_msg_agc_measure_fst_omt },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_START_OMT   ,   dtvd_tuner_core_mtx_011_msg_agc_start_omt   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_STOP_OMT    ,   dtvd_tuner_core_mtx_011_msg_agc_measure_stop_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_MEASURE_OMT ,   dtvd_tuner_core_mtx_011_msg_agc_measure_omt },
        {   D_DTVD_TUNER_INEVT_CORE_TMCC_DEVERR ,   dtvd_tuner_core_mtx_011_msg_tmcc_deverr },
        {   D_DTVD_TUNER_INEVT_CORE_CN_START_OMT,   dtvd_tuner_core_mtx_011_msg_cn_start_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_CN_STOP_OMT ,   dtvd_tuner_core_mtx_011_msg_cn_stop_omt },
        {   D_DTVD_TUNER_INEVT_CORE_CN_MEASURE_OMT, dtvd_tuner_core_mtx_011_msg_cn_measure_omt  },
        {   D_DTVD_TUNER_INEVT_CORE_BER_START_OMT,  dtvd_tuner_core_mtx_011_msg_ber_start_omt   },
        {   D_DTVD_TUNER_INEVT_CORE_BER_STOP_OMT,   dtvd_tuner_core_mtx_011_msg_ber_stop_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_BER_MEASURE_OMT ,   dtvd_tuner_core_mtx_011_msg_ber_measure_omt },

        {   D_DTVD_TUNER_INEVT_CORE_STATE_DEVERR,   dtvd_tuner_core_mtx_011_msg_state_deverr },

        {   D_DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE  ,   dtvd_tuner_core_mtx_011_msg_autoeco_notice  },
        {   D_DTVD_TUNER_INEVT_CORE_AUTOECO_DEVERR  ,   dtvd_tuner_core_mtx_011_msg_autoeco_deverr  },

        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core012_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_TUNERDD_PRE      ,   dtvd_tuner_core_mtx_012_tunerdd_pre },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_STOP_OMT  ,   dtvd_tuner_core_mtx_012_msg_tuner_stop_omt  },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT    ,   dtvd_tuner_core_mtx_012_msg_tuner_deverr_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_STOP_OMT,   dtvd_tuner_core_mtx_012_msg_agc_stop_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_CN_STOP_OMT ,   dtvd_tuner_core_mtx_012_msg_cn_stop_omt },
        {   D_DTVD_TUNER_INEVT_CORE_BER_STOP_OMT,   dtvd_tuner_core_mtx_012_msg_ber_stop_omt    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core013_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_STOP_OMT     ,   dtvd_tuner_core_mtx_013_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV_OMT ,   dtvd_tuner_core_mtx_013_tune_dtv_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_BER_START_OMT,   dtvd_tuner_core_mtx_013_ber_start_omt   },
        {   D_DTVD_TUNER_EXEVT_MDL_BER_STOP_OMT ,   dtvd_tuner_core_mtx_013_ber_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_BER_GET_OMT  ,   dtvd_tuner_core_mtx_013_ber_get_omt },
        {   D_DTVD_TUNER_EXEVT_MDL_CNR_START_OMT,   dtvd_tuner_core_mtx_013_cnr_start_omt   },
        {   D_DTVD_TUNER_EXEVT_MDL_CNR_STOP_OMT ,   dtvd_tuner_core_mtx_013_cnr_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_CNR_GET_OMT  ,   dtvd_tuner_core_mtx_013_cnr_get_omt },
        {   D_DTVD_TUNER_EXEVT_MDL_AGC_START_OMT,   dtvd_tuner_core_mtx_013_agc_start_omt   },
        {   D_DTVD_TUNER_EXEVT_MDL_AGC_STOP_OMT ,   dtvd_tuner_core_mtx_013_agc_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_MDL_AGC_GET_OMT  ,   dtvd_tuner_core_mtx_013_agc_get_omt },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_PRE      ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_STOP_OMT  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR_OMT,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_STOP_OMT,   dtvd_tuner_core_mtx_013_msg_agc_stop_omt    },
        {   D_DTVD_TUNER_INEVT_CORE_CN_STOP_OMT ,   dtvd_tuner_core_mtx_013_msg_cn_stop_omt },
        {   D_DTVD_TUNER_INEVT_CORE_BER_STOP_OMT,   dtvd_tuner_core_mtx_013_msg_ber_stop_omt    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_core014_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_MDL_TUNE_DTV     ,   dtvd_tuner_core_mtx_014_tune_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_SCAN_DTV     ,   dtvd_tuner_core_mtx_014_scan_dtv    },
        {   D_DTVD_TUNER_EXEVT_MDL_STOP         ,   dtvd_tuner_core_mtx_014_stop    },
        {   D_DTVD_TUNER_EXEVT_MDL_ECO_SET      ,   dtvd_tuner_core_mtx_014_eco_set },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_NONSTD,   dtvd_tuner_core_mtx_014_msg_tuner_nonstd    },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_ASYNC ,   dtvd_tuner_core_mtx_014_msg_tuner_async },
        {   D_DTVD_TUNER_INEVT_CORE_TUNER_DEVERR,   dtvd_tuner_core_mtx_014_msg_tuner_deverr    },
        {   D_DTVD_TUNER_INEVT_CORE_AGC_DEVERR  ,   dtvd_tuner_core_mtx_014_msg_agc_deverr  },
        {   D_DTVD_TUNER_INEVT_CORE_TMCC_DEVERR ,   dtvd_tuner_core_mtx_014_msg_tmcc_deverr },
        {   D_DTVD_TUNER_INEVT_CORE_CN_MEASURE  ,   dtvd_tuner_core_mtx_014_msg_cn_measure  },
        {   D_DTVD_TUNER_INEVT_CORE_CN_DEVERR   ,   dtvd_tuner_core_mtx_014_msg_cn_deverr   },
        {   D_DTVD_TUNER_INEVT_CORE_BER_DEVERR  ,   dtvd_tuner_core_mtx_014_msg_ber_deverr  },

        {   D_DTVD_TUNER_INEVT_CORE_AUTOECO_NOTICE  ,   dtvd_tuner_core_mtx_014_msg_autoeco_notice  },
        {   D_DTVD_TUNER_INEVT_CORE_AUTOECO_DEVERR  ,   dtvd_tuner_core_mtx_014_msg_autoeco_deverr  },

        {   D_DTVD_TUNER_INEVT_CORE_STATE_DEVERR,   dtvd_tuner_core_mtx_014_msg_state_deverr  },

        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t* DTVD_TUNER_ATTRIBUTE const tdtvd_tuner_mtx_core_tbl[] = {
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core000_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core001_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core002_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core003_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core004_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core005_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core006_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core007_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core008_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core009_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core010_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core011_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core012_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core013_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_core014_tbl
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_ant000_tbl[] = {
        { D_DTVD_TUNER_INEVT_ANT_CORE_INIT,         dtvd_tuner_ant_mtx_000_msg_core_init },
        { D_DTVD_TUNER_INEVT_ANT_CORE_INIT_OMT,     dtvd_tuner_ant_mtx_000_msg_core_init_omt },
        { D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_ant001_tbl[] = {
        { D_DTVD_TUNER_INEVT_ANT_CORE_INIT,         dtvd_tuner_ant_mtx_001_msg_core_init },
        { D_DTVD_TUNER_INEVT_ANT_CORE_END,          dtvd_tuner_ant_mtx_001_msg_core_end },
        { D_DTVD_TUNER_INEVT_ANT_CORE_SET,          dtvd_tuner_ant_mtx_001_msg_core_set },

        { D_DTVD_TUNER_INEVT_ANT_CORE_INIT_OMT,     dtvd_tuner_ant_mtx_001_msg_core_init_omt },

        { D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t* DTVD_TUNER_ATTRIBUTE const tdtvd_tuner_mtx_ant_tbl[] = {
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_ant000_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_ant001_tbl
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner000_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_PWRON ,   dtvd_tuner_tuner_mtx_000_msg_core_pwron },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner001_tbl[] = {
        {   D_DTVD_TUNER_EXEVT_TIMER_PWRON  ,   dtvd_tuner_tuner_mtx_001_timeout_pwron  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner002_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_PWROFF,   dtvd_tuner_tuner_mtx_002_msg_core_pwroff    },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_INIT  ,   dtvd_tuner_tuner_mtx_002_msg_core_init  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_INIT_OMT  ,   dtvd_tuner_tuner_mtx_002_msg_core_init_omt  },
        {   D_DTVD_TUNER_EXEVT_TUNER_INT    ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner003_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_INIT      ,   dtvd_tuner_tuner_mtx_003_msg_core_init  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_END       ,   dtvd_tuner_tuner_mtx_003_msg_core_end   },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV  ,   dtvd_tuner_tuner_mtx_003_msg_core_tune_dtv  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP      ,   dtvd_tuner_tuner_mtx_003_msg_core_stop  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_INIT_OMT  ,   dtvd_tuner_tuner_mtx_003_msg_core_init_omt  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_TUNE_DTV_OMT, dtvd_tuner_tuner_mtx_003_msg_core_tune_dtv_omt  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT  ,   dtvd_tuner_tuner_mtx_003_msg_core_stop_omt  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET   ,   dtvd_tuner_tuner_mtx_003_msg_core_eco_set   },
        {   D_DTVD_TUNER_INEVT_TUNER_TMCC_STOP      ,   dtvd_tuner_tuner_mtx_003_msg_tmcc_stop  },
        {   D_DTVD_TUNER_EXEVT_TUNER_INT            ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_MODE_OK,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_SYNC   ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_TMCC_OK,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_TS_START,  D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner004_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP      ,   dtvd_tuner_tuner_mtx_004_msg_core_stop  },

        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT  ,   dtvd_tuner_tuner_mtx_004_msg_core_stop_omt  },

        {   D_DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET   ,   dtvd_tuner_tuner_mtx_004_msg_core_eco_set   },
        {   D_DTVD_TUNER_EXEVT_TUNER_INT            ,   dtvd_tuner_tuner_mtx_004_tuner_int  },
        {   D_DTVD_TUNER_EXEVT_TIMER_SEARCH         ,   dtvd_tuner_tuner_mtx_004_timeout_serach },
        {   D_DTVD_TUNER_EXEVT_TIMER_ECO            ,   dtvd_tuner_tuner_mtx_004_timeout_eco    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner005_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP      ,   dtvd_tuner_tuner_mtx_005_msg_core_stop  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT  ,   dtvd_tuner_tuner_mtx_005_msg_core_stop_omt  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET   ,   dtvd_tuner_tuner_mtx_005_msg_core_eco_set   },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_MODE_OK,   dtvd_tuner_tuner_mtx_005_tunerdd_state_mode_ok  },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_SYNC   ,   dtvd_tuner_tuner_mtx_005_tunerdd_state_sync },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_TMCC_OK,  dtvd_tuner_tuner_mtx_005_tunerdd_state_tmcc_ok },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC  ,   dtvd_tuner_tuner_mtx_005_tunerdd_state_async    },
        {   D_DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM  ,   dtvd_tuner_tuner_mtx_005_msg_state_confirm  },
        {   D_DTVD_TUNER_EXEVT_TIMER_SYNC           ,   dtvd_tuner_tuner_mtx_005_timeout_sync   },
        {   D_DTVD_TUNER_EXEVT_TIMER_ECO            ,   dtvd_tuner_tuner_mtx_005_timeout_eco    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner006_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP      ,   dtvd_tuner_tuner_mtx_006_msg_core_stop  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT  ,   dtvd_tuner_tuner_mtx_006_msg_core_stop_omt  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET   ,   dtvd_tuner_tuner_mtx_006_msg_core_eco_set   },
        {   D_DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET   ,   dtvd_tuner_tuner_mtx_006_msg_tmcc_infoget   },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_TS_START,  D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC  ,   dtvd_tuner_tuner_mtx_006_tunerdd_state_async    },

        {   D_DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM  ,   dtvd_tuner_tuner_mtx_006_msg_state_confirm    },
        {   D_DTVD_TUNER_EXEVT_TIMER_SYNC           ,   dtvd_tuner_tuner_mtx_006_timeout_sync    },

        {   D_DTVD_TUNER_EXEVT_TIMER_ECO            ,   dtvd_tuner_tuner_mtx_006_timeout_eco    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner007_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP      ,   dtvd_tuner_tuner_mtx_007_msg_core_stop  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT  ,   dtvd_tuner_tuner_mtx_007_msg_core_stop_omt  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET   ,   dtvd_tuner_tuner_mtx_007_msg_core_eco_set   },
        {   D_DTVD_TUNER_INEVT_TUNER_TMCC_INFOGET   ,   dtvd_tuner_tuner_mtx_007_msg_tmcc_infoget   },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_TS_START,  dtvd_tuner_tuner_mtx_007_tunerdd_state_ts_start },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC  ,   dtvd_tuner_tuner_mtx_007_tunerdd_state_async    },
        {   D_DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM  ,   dtvd_tuner_tuner_mtx_007_msg_state_confirm  },
        {   D_DTVD_TUNER_EXEVT_TIMER_TS             ,   dtvd_tuner_tuner_mtx_007_timeout_ts },
        {   D_DTVD_TUNER_EXEVT_TIMER_ECO            ,   dtvd_tuner_tuner_mtx_007_timeout_eco    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner008_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP      ,   dtvd_tuner_tuner_mtx_008_msg_core_stop  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT  ,   dtvd_tuner_tuner_mtx_008_msg_core_stop_omt  },
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_ECO_SET   ,   dtvd_tuner_tuner_mtx_008_msg_core_eco_set   },
        {   D_DTVD_TUNER_INEVT_TUNER_TMCC_STOP      ,   dtvd_tuner_tuner_mtx_008_msg_tmcc_stop  },
        {   D_DTVD_TUNER_EXEVT_TUNER_INT            ,   D_DTVD_TUNER_NULL  },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_MODE_OK,   dtvd_tuner_tuner_mtx_008_tunerdd_state_mode_ok  },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_SYNC   ,   dtvd_tuner_tuner_mtx_008_tunerdd_state_sync },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_TMCC_OK,   dtvd_tuner_tuner_mtx_008_tunerdd_state_tmcc_ok  },
        {   D_DTVD_TUNER_EXEVT_TUNERDD_STATE_ASYNC  ,   D_DTVD_TUNER_NULL   },

        {   D_DTVD_TUNER_INEVT_TUNER_STATE_CONFIRM  ,   dtvd_tuner_tuner_mtx_008_msg_state_confirm    },
        {   D_DTVD_TUNER_EXEVT_TIMER_SYNC           ,   dtvd_tuner_tuner_mtx_008_timeout_sync    },

        {   D_DTVD_TUNER_EXEVT_TIMER_ECO            ,   dtvd_tuner_tuner_mtx_008_timeout_eco    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner009_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TUNER_TMCC_STOP      ,   dtvd_tuner_tuner_mtx_009_msg_tmcc_stop  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tuner010_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TUNER_CORE_STOP_OMT  ,   dtvd_tuner_tuner_mtx_010_msg_core_stop_omt  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t* DTVD_TUNER_ATTRIBUTE const tdtvd_tuner_mtx_tuner_tbl[] = {
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner000_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner001_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner002_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner003_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner004_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner005_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner006_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner007_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner008_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner009_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tuner010_tbl
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_agc000_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AGC_CORE_INIT        ,   dtvd_tuner_agc_mtx_000_msg_core_init    },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_INIT_OMT    ,   dtvd_tuner_agc_mtx_000_msg_core_init_omt    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_agc001_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AGC_CORE_END         ,   dtvd_tuner_agc_mtx_001_msg_core_end },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST ,   dtvd_tuner_agc_mtx_001_msg_core_measure_fst },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_STOP        ,   dtvd_tuner_agc_mtx_001_msg_core_stop    },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_FST_OMT ,   dtvd_tuner_agc_mtx_001_msg_core_measure_fst_omt },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT    ,   dtvd_tuner_agc_mtx_001_msg_core_stop_omt    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_agc002_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE     ,   dtvd_tuner_agc_mtx_002_msg_core_measure },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_STOP        ,   dtvd_tuner_agc_mtx_002_msg_core_stop    },
        {   D_DTVD_TUNER_EXEVT_TIMER_AGC            ,   dtvd_tuner_agc_mtx_002_timeout_agc  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_agc003_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AGC_CORE_STOP        ,   dtvd_tuner_agc_mtx_003_msg_core_stop    },
        {   D_DTVD_TUNER_EXEVT_TIMER_AGC            ,   dtvd_tuner_agc_mtx_003_timeout_agc  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_agc004_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE     ,   dtvd_tuner_agc_mtx_004_msg_core_measure },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_STOP        ,   dtvd_tuner_agc_mtx_004_msg_core_stop    },
        {   D_DTVD_TUNER_EXEVT_TIMER_AGC_WATCH      ,   dtvd_tuner_agc_mtx_004_timeout_agc_watch    },
        { D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_agc005_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AGC_CORE_START_OMT   ,   dtvd_tuner_agc_mtx_005_msg_core_start_omt   },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT    ,   dtvd_tuner_agc_mtx_005_msg_core_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_TIMER_AGC            ,   dtvd_tuner_agc_mtx_005_timeout_agc  },
        { D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_agc006_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT    ,   dtvd_tuner_agc_mtx_006_msg_core_stop_omt    },
        {   D_DTVD_TUNER_EXEVT_TIMER_AGC            ,   dtvd_tuner_agc_mtx_006_timeout_agc  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_agc007_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AGC_CORE_START_OMT   ,   dtvd_tuner_agc_mtx_007_msg_core_start_omt   },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT    ,   dtvd_tuner_agc_mtx_007_msg_core_stop_omt    },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_OMT ,   dtvd_tuner_agc_mtx_007_msg_core_measure_omt },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_MEASURE_STOP_OMT    ,   dtvd_tuner_agc_mtx_007_msg_core_measure_stop_omt    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_agc008_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AGC_CORE_START_OMT   ,   dtvd_tuner_agc_mtx_008_msg_core_start_omt   },
        {   D_DTVD_TUNER_INEVT_AGC_CORE_STOP_OMT    ,   dtvd_tuner_agc_mtx_008_msg_core_stop_omt    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t* DTVD_TUNER_ATTRIBUTE const tdtvd_tuner_mtx_agc_tbl[] = {
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_agc000_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_agc001_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_agc002_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_agc003_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_agc004_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_agc005_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_agc006_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_agc007_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_agc008_tbl
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tmcc000_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TMCC_TUNER_INIT  ,   dtvd_tuner_tmcc_mtx_000_msg_tuner_init  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tmcc001_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TMCC_TUNER_END   ,   dtvd_tuner_tmcc_mtx_001_msg_tuner_end   },
        {   D_DTVD_TUNER_INEVT_TMCC_TUNER_INFOGET,  dtvd_tuner_tmcc_mtx_001_msg_tuner_infoget   },
        {   D_DTVD_TUNER_INEVT_TMCC_TUNER_STOP  ,   dtvd_tuner_tmcc_mtx_001_msg_tuner_stop  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tmcc002_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TMCC_TUNER_STOP  ,   dtvd_tuner_tmcc_mtx_002_msg_tuner_stop  },
        {   D_DTVD_TUNER_EXEVT_TIMER_TMCC       ,   dtvd_tuner_tmcc_mtx_002_timeout_tmcc_get    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_tmcc003_tbl[] = {
        {   D_DTVD_TUNER_INEVT_TMCC_TUNER_STOP      ,   dtvd_tuner_tmcc_mtx_003_msg_tuner_stop  },
        {   D_DTVD_TUNER_EXEVT_TIMER_TMCC_WATCH     ,   dtvd_tuner_tmcc_mtx_003_timeout_tmcc_watch  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t* DTVD_TUNER_ATTRIBUTE const tdtvd_tuner_mtx_tmcc_tbl[] = {
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tmcc000_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tmcc001_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tmcc002_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_tmcc003_tbl
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_cn000_tbl[] = {
        {   D_DTVD_TUNER_INEVT_CN_CORE_INIT         ,   dtvd_tuner_cn_mtx_000_msg_core_init },
        {   D_DTVD_TUNER_INEVT_CN_CORE_INIT_OMT     ,   dtvd_tuner_cn_mtx_000_msg_core_init_omt },
        { D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_cn001_tbl[] = {
        {   D_DTVD_TUNER_INEVT_CN_CORE_END          ,   dtvd_tuner_cn_mtx_001_msg_core_end  },
        {   D_DTVD_TUNER_INEVT_CN_CORE_SET          ,   dtvd_tuner_cn_mtx_001_msg_core_set  },
        {   D_DTVD_TUNER_INEVT_CN_CORE_MEASURE      ,   dtvd_tuner_cn_mtx_001_msg_core_measure  },
        {   D_DTVD_TUNER_INEVT_CN_CORE_STOP         ,   dtvd_tuner_cn_mtx_001_msg_core_stop },
        {   D_DTVD_TUNER_INEVT_CN_CORE_STOP_OMT     ,   dtvd_tuner_cn_mtx_001_msg_core_stop_omt },
        {   D_DTVD_TUNER_INEVT_CN_CORE_START_OMT    ,   dtvd_tuner_cn_mtx_001_msg_core_start_omt    },
        { D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_cn002_tbl[] = {
        {   D_DTVD_TUNER_INEVT_CN_CORE_MEASURE      ,   dtvd_tuner_cn_mtx_002_msg_core_measure  },
        {   D_DTVD_TUNER_INEVT_CN_CORE_STOP         ,   dtvd_tuner_cn_mtx_002_msg_core_stop },
        {   D_DTVD_TUNER_EXEVT_TIMER_CN_FST         ,   dtvd_tuner_cn_mtx_002_timeout_cn    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_cn003_tbl[] = {
        {   D_DTVD_TUNER_INEVT_CN_CORE_MEASURE      ,   dtvd_tuner_cn_mtx_003_msg_core_measure  },
        {   D_DTVD_TUNER_INEVT_CN_CORE_STOP         ,   dtvd_tuner_cn_mtx_003_msg_core_stop },
        {   D_DTVD_TUNER_EXEVT_TIMER_CN_MEASURE     ,   dtvd_tuner_cn_mtx_003_timeout_cn_second },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_cn004_tbl[] = {
        {   D_DTVD_TUNER_INEVT_CN_CORE_MEASURE      ,   dtvd_tuner_cn_mtx_004_msg_core_measure  },
        {   D_DTVD_TUNER_INEVT_CN_CORE_STOP         ,   dtvd_tuner_cn_mtx_004_msg_core_stop },
        {   D_DTVD_TUNER_EXEVT_TIMER_CN_MEASURE     ,   dtvd_tuner_cn_mtx_004_timeout_cn_second },
        {   D_DTVD_TUNER_EXEVT_TIMER_CN_WATCH       ,   dtvd_tuner_cn_mtx_004_timeout_cn_monitor    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_cn005_tbl[] = {
        {   D_DTVD_TUNER_INEVT_CN_CORE_STOP_OMT     ,   dtvd_tuner_cn_mtx_005_msg_core_stop_omt },
        {   D_DTVD_TUNER_INEVT_CN_CORE_MEASURE_OMT  ,   dtvd_tuner_cn_mtx_005_msg_core_measure_omt  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_cn006_tbl[] = {
        {   D_DTVD_TUNER_INEVT_CN_CORE_STOP_OMT     ,   dtvd_tuner_cn_mtx_006_msg_core_stop_omt },
        {   D_DTVD_TUNER_EXEVT_TIMER_CN_MEASURE     ,   dtvd_tuner_cn_mtx_006_timeout_cn_second },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t* DTVD_TUNER_ATTRIBUTE const tdtvd_tuner_mtx_cn_tbl[] = {
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_cn000_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_cn001_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_cn002_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_cn003_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_cn004_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_cn005_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_cn006_tbl
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_ber000_tbl[] = {
        {   D_DTVD_TUNER_INEVT_BER_CORE_INIT        ,   dtvd_tuner_ber_mtx_000_msg_core_init    },
        {   D_DTVD_TUNER_INEVT_BER_CORE_INIT_OMT    ,   dtvd_tuner_ber_mtx_000_msg_core_init_omt    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_ber001_tbl[] = {
        {   D_DTVD_TUNER_INEVT_BER_CORE_END         ,   dtvd_tuner_ber_mtx_001_msg_core_end },
        {   D_DTVD_TUNER_INEVT_BER_CORE_SET         ,   dtvd_tuner_ber_mtx_001_msg_core_set },
        {   D_DTVD_TUNER_INEVT_BER_CORE_START       ,   dtvd_tuner_ber_mtx_001_msg_core_start   },
        {   D_DTVD_TUNER_INEVT_BER_CORE_STOP        ,   dtvd_tuner_ber_mtx_001_msg_core_stop    },
        {   D_DTVD_TUNER_INEVT_BER_CORE_START_OMT   ,   dtvd_tuner_ber_mtx_001_msg_core_start_omt   },
        {   D_DTVD_TUNER_INEVT_BER_CORE_STOP_OMT    ,   dtvd_tuner_ber_mtx_001_msg_core_stop_omt    },
        {   D_DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_ber002_tbl[] = {
        {   D_DTVD_TUNER_INEVT_BER_CORE_START       ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_BER_CORE_STOP        ,   dtvd_tuner_ber_mtx_002_msg_core_stop    },
        {   D_DTVD_TUNER_EXEVT_TIMER_BER_FST        ,   dtvd_tuner_ber_mtx_002_timeout_ber_fst  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_ber003_tbl[] = {
        {   D_DTVD_TUNER_INEVT_BER_CORE_START       ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_BER_CORE_STOP        ,   dtvd_tuner_ber_mtx_003_msg_core_stop    },
        {   D_DTVD_TUNER_EXEVT_TIMER_BER_MEASURE    ,   dtvd_tuner_ber_mtx_003_timeout_ber  },
        {   D_DTVD_TUNER_EXEVT_TIMER_BER_WATCH      ,   dtvd_tuner_ber_mtx_003_timeout_ber_watch    },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_ber004_tbl[] = {
        {   D_DTVD_TUNER_INEVT_BER_CORE_STOP_OMT    ,   dtvd_tuner_ber_mtx_004_msg_core_stop_omt    },
        {   D_DTVD_TUNER_INEVT_BER_CORE_MEASURE_OMT ,   dtvd_tuner_ber_mtx_004_msg_core_get_omt  },
        {   D_DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_ber005_tbl[] = {
        {   D_DTVD_TUNER_INEVT_BER_CORE_STOP_OMT ,   dtvd_tuner_ber_mtx_005_msg_core_stop_omt },
        {   D_DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT  ,   dtvd_tuner_ber_mtx_005_msg_core_getstop_omt },
        {   D_DTVD_TUNER_EXEVT_TIMER_BER_MEASURE    ,   dtvd_tuner_ber_mtx_005_timeout_ber  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_ber006_tbl[] = {
        {   D_DTVD_TUNER_INEVT_BER_CORE_STOP_OMT    ,   dtvd_tuner_ber_mtx_006_msg_core_stop_omt    },
        {   D_DTVD_TUNER_INEVT_BER_CORE_MEASURE_OMT ,   dtvd_tuner_ber_mtx_006_msg_core_measure_omt },
        {   D_DTVD_TUNER_INEVT_BER_CORE_MEASURE_CANCEL_OMT  ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t* DTVD_TUNER_ATTRIBUTE const tdtvd_tuner_mtx_ber_tbl[] = {
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_ber000_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_ber001_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_ber002_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_ber003_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_ber004_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_ber005_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_ber006_tbl
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_state000_tbl[] = {
        {   D_DTVD_TUNER_INEVT_STATE_TUNER_INIT     ,   dtvd_tuner_state_mtx_000_msg_tuner_init },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_state001_tbl[] = {
        {   D_DTVD_TUNER_INEVT_STATE_TUNER_END      ,   dtvd_tuner_state_mtx_001_msg_tuner_end  },
        {   D_DTVD_TUNER_INEVT_STATE_TUNER_START    ,   dtvd_tuner_state_mtx_001_msg_tuner_start    },
        {   D_DTVD_TUNER_INEVT_STATE_TUNER_STOP     ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_state002_tbl[] = {
        {   D_DTVD_TUNER_INEVT_STATE_TUNER_START    ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_STATE_TUNER_STOP     ,   dtvd_tuner_state_mtx_002_msg_tuner_stop },
        {   D_DTVD_TUNER_INEVT_STATE_TUNER_CONFIRM  ,   dtvd_tuner_state_mtx_002_msg_tuner_confirm  },
        {   D_DTVD_TUNER_EXEVT_TIMER_STATE_WATCH    ,   dtvd_tuner_state_mtx_002_timeout_state  },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t* DTVD_TUNER_ATTRIBUTE const tdtvd_tuner_mtx_state_tbl[] = {
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_state000_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_state001_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_state002_tbl
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_autoeco000_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_INIT    ,   dtvd_tuner_autoeco_mtx_000_msg_core_init },
        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_END     ,   D_DTVD_TUNER_NULL },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_autoeco001_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_END     ,   dtvd_tuner_autoeco_mtx_001_msg_core_end },
        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_START   ,   dtvd_tuner_autoeco_mtx_001_msg_core_start },
        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_STOP    ,   D_DTVD_TUNER_NULL   },

        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_START_OMT   ,   dtvd_tuner_autoeco_mtx_001_msg_core_start_omt },
        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_OMT    ,   D_DTVD_TUNER_NULL   },

        {   D_DTVD_TUNER_INEVT_AUTOECO_CN_NOTICE    ,   D_DTVD_TUNER_NULL   },

        {   D_DTVD_TUNER_EXEVT_TIMER_AUTOECO_WATCH_OMT, D_DTVD_TUNER_NULL },

        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_autoeco002_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_START   ,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_STOP    ,   dtvd_tuner_autoeco_mtx_002_msg_core_stop },
        {   D_DTVD_TUNER_INEVT_AUTOECO_CN_NOTICE    ,   dtvd_tuner_autoeco_mtx_002_msg_cn_notice },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t DTVD_TUNER_ATTRIBUTE dtvd_tuner_mtx_autoeco003_tbl[] = {
        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_START_OMT,   D_DTVD_TUNER_NULL   },
        {   D_DTVD_TUNER_INEVT_AUTOECO_CORE_STOP_OMT ,   dtvd_tuner_autoeco_mtx_003_msg_core_stop_omt },
        {   D_DTVD_TUNER_EXEVT_TIMER_AUTOECO_WATCH_OMT,   dtvd_tuner_autoeco_mtx_003_msg_timeout_autoeco_watch_omt },
        {   D_DTVD_TUNER_NOEVENT, D_DTVD_TUNER_CTL_UNDEF } 
    };

    static const DTVD_TUNER_MTX_t* DTVD_TUNER_ATTRIBUTE const tdtvd_tuner_mtx_autoeco_tbl[] = {
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_autoeco000_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_autoeco001_tbl,
        (DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_autoeco002_tbl

       ,(DTVD_TUNER_MTX_t*)dtvd_tuner_mtx_autoeco003_tbl

    };

    static const DTVD_TUNER_MTX_TBL_t DTVD_TUNER_ATTRIBUTE tdtvd_tuner_mtx_tbl[] =
    {
        { D_DTVD_TUNER_CTL_CORE,        (DTVD_TUNER_MTX_t**)tdtvd_tuner_mtx_core_tbl    },
        { D_DTVD_TUNER_CTL_ANT,         (DTVD_TUNER_MTX_t**)tdtvd_tuner_mtx_ant_tbl     },
        { D_DTVD_TUNER_CTL_TUNER,       (DTVD_TUNER_MTX_t**)tdtvd_tuner_mtx_tuner_tbl   },
        { D_DTVD_TUNER_CTL_WATCH_AGC,   (DTVD_TUNER_MTX_t**)tdtvd_tuner_mtx_agc_tbl     },
        { D_DTVD_TUNER_CTL_WATCH_TMCC,  (DTVD_TUNER_MTX_t**)tdtvd_tuner_mtx_tmcc_tbl    },
        { D_DTVD_TUNER_CTL_WATCH_CN,    (DTVD_TUNER_MTX_t**)tdtvd_tuner_mtx_cn_tbl      },
        { D_DTVD_TUNER_CTL_WATCH_BER,   (DTVD_TUNER_MTX_t**)tdtvd_tuner_mtx_ber_tbl     },
        { D_DTVD_TUNER_CTL_WATCH_STATE, (DTVD_TUNER_MTX_t**)tdtvd_tuner_mtx_state_tbl   },

        { D_DTVD_TUNER_CTL_AUTOECO,     (DTVD_TUNER_MTX_t**)tdtvd_tuner_mtx_autoeco_tbl },

        { D_DTVD_TUNER_CTL_UNDEF,       D_DTVD_TUNER_NULL }           
    };

    return tdtvd_tuner_mtx_tbl;
}

