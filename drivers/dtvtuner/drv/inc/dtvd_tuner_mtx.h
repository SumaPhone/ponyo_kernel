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

#ifndef _DTVD_TUNER_MTX_H_
#define _DTVD_TUNER_MTX_H_

void dtvd_tuner_core_mtx_000_pwron( void );
void dtvd_tuner_core_mtx_000_msg_tuner_pwron( void );
void dtvd_tuner_core_mtx_001_pwroff( void );
void dtvd_tuner_core_mtx_001_init( void );
void dtvd_tuner_core_mtx_001_init_omt( void );
void dtvd_tuner_core_mtx_001_msg_ant_init( void );
void dtvd_tuner_core_mtx_001_msg_ant_init_omt( void );
void dtvd_tuner_core_mtx_001_msg_tuner_pwroff( void );
void dtvd_tuner_core_mtx_001_msg_tuner_init( void );
void dtvd_tuner_core_mtx_001_msg_tuner_deverr( void );
void dtvd_tuner_core_mtx_001_msg_tuner_init_omt( void );
void dtvd_tuner_core_mtx_001_msg_tuner_deverr_omt( void );
void dtvd_tuner_core_mtx_002_end( void );
void dtvd_tuner_core_mtx_002_tune_dtv( void );
void dtvd_tuner_core_mtx_002_scan_dtv( void );
void dtvd_tuner_core_mtx_002_stop( void );
void dtvd_tuner_core_mtx_002_stop_omt( void );
void dtvd_tuner_core_mtx_002_tune_dtv_omt( void );
void dtvd_tuner_core_mtx_002_eco_set( void );
void dtvd_tuner_core_mtx_002_msg_ant_end( void );
void dtvd_tuner_core_mtx_002_msg_tuner_end( void );
void dtvd_tuner_core_mtx_003_tune_dtv( void );
void dtvd_tuner_core_mtx_003_scan_dtv( void );
void dtvd_tuner_core_mtx_003_stop( void );
void dtvd_tuner_core_mtx_003_eco_set( void );
void dtvd_tuner_core_mtx_003_msg_tuner_tune_set( void );
void dtvd_tuner_core_mtx_003_msg_tuner_tsout( void );
void dtvd_tuner_core_mtx_003_msg_tuner_tune_ng( void );
void dtvd_tuner_core_mtx_003_msg_tuner_nonstd( void );
void dtvd_tuner_core_mtx_003_msg_tuner_sync( void );
void dtvd_tuner_core_mtx_003_msg_tuner_async( void );
void dtvd_tuner_core_mtx_003_msg_tuner_deverr( void );
void dtvd_tuner_core_mtx_003_msg_tuner_errcore( void );
void dtvd_tuner_core_mtx_003_msg_agc_measure_fst( void );
void dtvd_tuner_core_mtx_003_msg_agc_measure( void );
void dtvd_tuner_core_mtx_003_msg_agc_deverr( void );
void dtvd_tuner_core_mtx_003_msg_tmcc_deverr( void );
void dtvd_tuner_core_mtx_003_msg_cn_deverr( void );
void dtvd_tuner_core_mtx_003_msg_ber_deverr( void );

void dtvd_tuner_core_mtx_003_msg_state_deverr( void );

void dtvd_tuner_core_mtx_004_tune_dtv( void );
void dtvd_tuner_core_mtx_004_scan_dtv( void );
void dtvd_tuner_core_mtx_004_stop( void );
void dtvd_tuner_core_mtx_004_eco_set( void );
void dtvd_tuner_core_mtx_004_msg_tuner_tune_set( void );
void dtvd_tuner_core_mtx_004_msg_tuner_tsout( void );
void dtvd_tuner_core_mtx_004_msg_tuner_tune_ng( void );
void dtvd_tuner_core_mtx_004_msg_tuner_nonstd( void );
void dtvd_tuner_core_mtx_004_msg_tuner_sync( void );
void dtvd_tuner_core_mtx_004_msg_tuner_async( void );
void dtvd_tuner_core_mtx_004_msg_tuner_deverr( void );
void dtvd_tuner_core_mtx_004_msg_agc_measure_fst( void );
void dtvd_tuner_core_mtx_004_msg_agc_measure( void );
void dtvd_tuner_core_mtx_004_msg_agc_deverr( void );
void dtvd_tuner_core_mtx_004_msg_tmcc_deverr( void );

void dtvd_tuner_core_mtx_004_msg_state_deverr( void );

void dtvd_tuner_core_mtx_005_tune_dtv( void );
void dtvd_tuner_core_mtx_005_scan_dtv( void );
void dtvd_tuner_core_mtx_005_stop( void );
void dtvd_tuner_core_mtx_005_eco_set( void );
void dtvd_tuner_core_mtx_005_msg_tuner_tsout( void );
void dtvd_tuner_core_mtx_005_msg_tuner_nonstd( void );
void dtvd_tuner_core_mtx_005_msg_tuner_sync( void );
void dtvd_tuner_core_mtx_005_msg_tuner_async( void );
void dtvd_tuner_core_mtx_005_msg_tuner_deverr( void );
void dtvd_tuner_core_mtx_005_msg_tuner_errcore( void );
void dtvd_tuner_core_mtx_005_msg_agc_deverr( void );
void dtvd_tuner_core_mtx_005_msg_tmcc_deverr( void );
void dtvd_tuner_core_mtx_005_msg_cn_deverr( void );
void dtvd_tuner_core_mtx_005_msg_ber_deverr( void );

void dtvd_tuner_core_mtx_005_msg_autoeco_notice( void );
void dtvd_tuner_core_mtx_005_msg_autoeco_deverr( void );

void dtvd_tuner_core_mtx_005_msg_state_deverr( void );

void dtvd_tuner_core_mtx_006_tune_dtv( void );
void dtvd_tuner_core_mtx_006_scan_dtv( void );
void dtvd_tuner_core_mtx_006_stop( void );
void dtvd_tuner_core_mtx_006_eco_set( void );
void dtvd_tuner_core_mtx_006_msg_tuner_tsout( void );
void dtvd_tuner_core_mtx_006_msg_tuner_nonstd( void );
void dtvd_tuner_core_mtx_006_msg_tuner_sync( void );
void dtvd_tuner_core_mtx_006_msg_tuner_async( void );
void dtvd_tuner_core_mtx_006_msg_tuner_deverr( void );
void dtvd_tuner_core_mtx_006_msg_agc_deverr( void );
void dtvd_tuner_core_mtx_006_msg_tmcc_deverr( void );

void dtvd_tuner_core_mtx_006_msg_state_deverr( void );

void dtvd_tuner_core_mtx_007_tune_dtv( void );
void dtvd_tuner_core_mtx_007_scan_dtv( void );
void dtvd_tuner_core_mtx_007_stop( void );
void dtvd_tuner_core_mtx_007_eco_set( void );
void dtvd_tuner_core_mtx_007_tunerdd_pre( void );
void dtvd_tuner_core_mtx_007_msg_tuner_stop( void );
void dtvd_tuner_core_mtx_007_msg_tuner_deverr( void );
void dtvd_tuner_core_mtx_007_msg_agc_stop( void );
void dtvd_tuner_core_mtx_007_msg_cn_stop( void );
void dtvd_tuner_core_mtx_007_msg_ber_stop( void );
void dtvd_tuner_core_mtx_008_msg_tuner_stop( void );
void dtvd_tuner_core_mtx_008_msg_tuner_deverr( void );
void dtvd_tuner_core_mtx_008_msg_tuner_stop_omt( void );
void dtvd_tuner_core_mtx_008_msg_tuner_deverr_omt( void );
void dtvd_tuner_core_mtx_008_msg_agc_stop( void );
void dtvd_tuner_core_mtx_008_msg_agc_stop_omt( void );
void dtvd_tuner_core_mtx_008_msg_cn_stop( void );
void dtvd_tuner_core_mtx_008_msg_cn_stop_omt( void );
void dtvd_tuner_core_mtx_008_msg_ber_stop( void );
void dtvd_tuner_core_mtx_008_msg_ber_stop_omt( void );
void dtvd_tuner_core_mtx_009_tune_dtv( void );
void dtvd_tuner_core_mtx_009_scan_dtv( void );
void dtvd_tuner_core_mtx_009_stop( void );
void dtvd_tuner_core_mtx_009_msg_agc_stop( void );
void dtvd_tuner_core_mtx_009_msg_cn_stop( void );
void dtvd_tuner_core_mtx_009_msg_ber_stop( void );
void dtvd_tuner_core_mtx_010_stop_omt( void );
void dtvd_tuner_core_mtx_010_tune_dtv_omt( void );
void dtvd_tuner_core_mtx_010_agc_start_omt( void );
void dtvd_tuner_core_mtx_010_agc_stop_omt( void );
void dtvd_tuner_core_mtx_010_agc_get_omt( void );
void dtvd_tuner_core_mtx_010_msg_tuner_tune_set( void );
void dtvd_tuner_core_mtx_010_msg_tuner_tsout( void );
void dtvd_tuner_core_mtx_010_msg_tuner_tune_ng( void );
void dtvd_tuner_core_mtx_010_msg_tuner_nonstd( void );
void dtvd_tuner_core_mtx_010_msg_tuner_sync( void );
void dtvd_tuner_core_mtx_010_msg_tuner_async( void );
void dtvd_tuner_core_mtx_010_msg_tuner_deverr( void );
void dtvd_tuner_core_mtx_010_msg_tuner_deverr_omt( void );
void dtvd_tuner_core_mtx_010_msg_agc_measure_fst_omt( void );
void dtvd_tuner_core_mtx_010_msg_agc_start_omt( void );
void dtvd_tuner_core_mtx_010_msg_agc_measure_stop_omt( void );
void dtvd_tuner_core_mtx_010_msg_agc_measure_omt( void );
void dtvd_tuner_core_mtx_010_msg_tmcc_deverr( void );

void dtvd_tuner_core_mtx_010_msg_state_deverr( void );

void dtvd_tuner_core_mtx_011_stop_omt( void );
void dtvd_tuner_core_mtx_011_tune_dtv_omt( void );
void dtvd_tuner_core_mtx_011_ber_start_omt( void );
void dtvd_tuner_core_mtx_011_ber_stop_omt( void );
void dtvd_tuner_core_mtx_011_ber_get_omt( void );
void dtvd_tuner_core_mtx_011_cnr_start_omt( void );
void dtvd_tuner_core_mtx_011_cnr_stop_omt( void );
void dtvd_tuner_core_mtx_011_cnr_get_omt( void );
void dtvd_tuner_core_mtx_011_agc_start_omt( void );
void dtvd_tuner_core_mtx_011_agc_stop_omt( void );
void dtvd_tuner_core_mtx_011_agc_get_omt( void );
void dtvd_tuner_core_mtx_011_msg_tuner_tsout( void );
void dtvd_tuner_core_mtx_011_msg_tuner_nonstd( void );
void dtvd_tuner_core_mtx_011_msg_tuner_sync( void );
void dtvd_tuner_core_mtx_011_msg_tuner_async( void );
void dtvd_tuner_core_mtx_011_msg_tuner_deverr( void );
void dtvd_tuner_core_mtx_011_msg_agc_measure_fst_omt( void );
void dtvd_tuner_core_mtx_011_msg_agc_start_omt( void );
void dtvd_tuner_core_mtx_011_msg_agc_measure_stop_omt( void );
void dtvd_tuner_core_mtx_011_msg_agc_measure_omt( void );
void dtvd_tuner_core_mtx_011_msg_tmcc_deverr( void );
void dtvd_tuner_core_mtx_011_msg_cn_start_omt( void );
void dtvd_tuner_core_mtx_011_msg_cn_stop_omt( void );
void dtvd_tuner_core_mtx_011_msg_cn_measure_omt( void );
void dtvd_tuner_core_mtx_011_msg_ber_start_omt( void );
void dtvd_tuner_core_mtx_011_msg_ber_stop_omt( void );
void dtvd_tuner_core_mtx_011_msg_ber_measure_omt( void );

void dtvd_tuner_core_mtx_011_msg_state_deverr( void );

void dtvd_tuner_core_mtx_011_msg_autoeco_notice( void );
void dtvd_tuner_core_mtx_011_msg_autoeco_deverr( void );

void dtvd_tuner_core_mtx_012_tunerdd_pre( void );
void dtvd_tuner_core_mtx_012_msg_tuner_stop_omt( void );
void dtvd_tuner_core_mtx_012_msg_tuner_deverr_omt( void );
void dtvd_tuner_core_mtx_012_msg_agc_stop_omt( void );
void dtvd_tuner_core_mtx_012_msg_cn_stop_omt( void );
void dtvd_tuner_core_mtx_012_msg_ber_stop_omt( void );
void dtvd_tuner_core_mtx_013_stop_omt( void );
void dtvd_tuner_core_mtx_013_tune_dtv_omt( void );
void dtvd_tuner_core_mtx_013_ber_start_omt( void );
void dtvd_tuner_core_mtx_013_ber_stop_omt( void );
void dtvd_tuner_core_mtx_013_ber_get_omt( void );
void dtvd_tuner_core_mtx_013_cnr_start_omt( void );
void dtvd_tuner_core_mtx_013_cnr_stop_omt( void );
void dtvd_tuner_core_mtx_013_cnr_get_omt( void );
void dtvd_tuner_core_mtx_013_agc_start_omt( void );
void dtvd_tuner_core_mtx_013_agc_stop_omt( void );
void dtvd_tuner_core_mtx_013_agc_get_omt( void );
void dtvd_tuner_core_mtx_013_msg_agc_stop_omt( void );
void dtvd_tuner_core_mtx_013_msg_cn_stop_omt( void );
void dtvd_tuner_core_mtx_013_msg_ber_stop_omt( void );
void dtvd_tuner_core_mtx_014_tune_dtv( void );
void dtvd_tuner_core_mtx_014_scan_dtv( void );
void dtvd_tuner_core_mtx_014_stop( void );
void dtvd_tuner_core_mtx_014_eco_set( void );
void dtvd_tuner_core_mtx_014_msg_tuner_nonstd( void );
void dtvd_tuner_core_mtx_014_msg_tuner_async( void );
void dtvd_tuner_core_mtx_014_msg_tuner_deverr( void );
void dtvd_tuner_core_mtx_014_msg_agc_deverr( void );
void dtvd_tuner_core_mtx_014_msg_tmcc_deverr( void );
void dtvd_tuner_core_mtx_014_msg_cn_measure( void );
void dtvd_tuner_core_mtx_014_msg_cn_deverr( void );
void dtvd_tuner_core_mtx_014_msg_ber_deverr( void );

void dtvd_tuner_core_mtx_014_msg_autoeco_notice( void );
void dtvd_tuner_core_mtx_014_msg_autoeco_deverr( void );

void dtvd_tuner_core_mtx_014_msg_state_deverr( void );

void dtvd_tuner_ant_mtx_000_msg_core_init( void );
void dtvd_tuner_ant_mtx_000_msg_core_init_omt( void );
void dtvd_tuner_ant_mtx_001_msg_core_init( void );
void dtvd_tuner_ant_mtx_001_msg_core_end( void );
void dtvd_tuner_ant_mtx_001_msg_core_set( void );

void dtvd_tuner_ant_mtx_001_msg_core_init_omt( void );

void dtvd_tuner_tuner_mtx_000_msg_core_pwron( void );
void dtvd_tuner_tuner_mtx_001_timeout_pwron( void );
void dtvd_tuner_tuner_mtx_002_msg_core_pwroff( void );
void dtvd_tuner_tuner_mtx_002_msg_core_init( void );
void dtvd_tuner_tuner_mtx_002_msg_core_init_omt( void );
void dtvd_tuner_tuner_mtx_003_msg_core_init( void );
void dtvd_tuner_tuner_mtx_003_msg_core_end( void );
void dtvd_tuner_tuner_mtx_003_msg_core_tune_dtv( void );
void dtvd_tuner_tuner_mtx_003_msg_core_stop( void );
void dtvd_tuner_tuner_mtx_003_msg_core_init_omt( void );
void dtvd_tuner_tuner_mtx_003_msg_core_tune_dtv_omt( void );
void dtvd_tuner_tuner_mtx_003_msg_core_stop_omt( void );
void dtvd_tuner_tuner_mtx_003_msg_core_eco_set( void );
void dtvd_tuner_tuner_mtx_003_msg_tmcc_stop( void );
void dtvd_tuner_tuner_mtx_004_msg_core_stop( void );

void dtvd_tuner_tuner_mtx_004_msg_core_stop_omt( void );

void dtvd_tuner_tuner_mtx_004_msg_core_eco_set( void );
void dtvd_tuner_tuner_mtx_004_tuner_int( void );
void dtvd_tuner_tuner_mtx_004_timeout_serach( void );
void dtvd_tuner_tuner_mtx_004_timeout_eco( void );
void dtvd_tuner_tuner_mtx_005_msg_core_stop( void );
void dtvd_tuner_tuner_mtx_005_msg_core_stop_omt( void );
void dtvd_tuner_tuner_mtx_005_msg_core_eco_set( void );
void dtvd_tuner_tuner_mtx_005_tunerdd_state_mode_ok( void );
void dtvd_tuner_tuner_mtx_005_tunerdd_state_sync( void );
void dtvd_tuner_tuner_mtx_005_tunerdd_state_tmcc_ok( void );
void dtvd_tuner_tuner_mtx_005_tunerdd_state_async( void );
void dtvd_tuner_tuner_mtx_005_msg_state_confirm( void );
void dtvd_tuner_tuner_mtx_005_timeout_sync( void );
void dtvd_tuner_tuner_mtx_005_timeout_eco( void );
void dtvd_tuner_tuner_mtx_006_msg_core_stop( void );
void dtvd_tuner_tuner_mtx_006_msg_core_stop_omt( void );
void dtvd_tuner_tuner_mtx_006_msg_core_eco_set( void );
void dtvd_tuner_tuner_mtx_006_msg_tmcc_infoget( void );
void dtvd_tuner_tuner_mtx_006_tunerdd_state_async( void );

void dtvd_tuner_tuner_mtx_006_msg_state_confirm( void );
void dtvd_tuner_tuner_mtx_006_timeout_sync( void );

void dtvd_tuner_tuner_mtx_006_timeout_eco( void );
void dtvd_tuner_tuner_mtx_007_msg_core_stop( void );
void dtvd_tuner_tuner_mtx_007_msg_core_stop_omt( void );
void dtvd_tuner_tuner_mtx_007_msg_core_eco_set( void );
void dtvd_tuner_tuner_mtx_007_msg_tmcc_infoget( void );
void dtvd_tuner_tuner_mtx_007_tunerdd_state_ts_start( void );
void dtvd_tuner_tuner_mtx_007_tunerdd_state_async( void );
void dtvd_tuner_tuner_mtx_007_msg_state_confirm( void );
void dtvd_tuner_tuner_mtx_007_timeout_ts( void );
void dtvd_tuner_tuner_mtx_007_timeout_eco( void );
void dtvd_tuner_tuner_mtx_008_msg_core_stop( void );
void dtvd_tuner_tuner_mtx_008_msg_core_stop_omt( void );
void dtvd_tuner_tuner_mtx_008_msg_core_eco_set( void );
void dtvd_tuner_tuner_mtx_008_msg_tmcc_stop( void );
void dtvd_tuner_tuner_mtx_008_tunerdd_state_mode_ok( void );
void dtvd_tuner_tuner_mtx_008_tunerdd_state_sync( void );
void dtvd_tuner_tuner_mtx_008_tunerdd_state_tmcc_ok( void );

void dtvd_tuner_tuner_mtx_008_msg_state_confirm( void );
void dtvd_tuner_tuner_mtx_008_timeout_sync( void );

void dtvd_tuner_tuner_mtx_008_timeout_eco( void );
void dtvd_tuner_tuner_mtx_009_msg_tmcc_stop( void );
void dtvd_tuner_tuner_mtx_010_msg_core_stop_omt( void );

void dtvd_tuner_agc_mtx_000_msg_core_init ( void );
void dtvd_tuner_agc_mtx_000_msg_core_init_omt ( void );
void dtvd_tuner_agc_mtx_001_msg_core_end ( void );
void dtvd_tuner_agc_mtx_001_msg_core_measure_fst ( void );
void dtvd_tuner_agc_mtx_001_msg_core_stop ( void );
void dtvd_tuner_agc_mtx_001_msg_core_measure_fst_omt ( void );
void dtvd_tuner_agc_mtx_001_msg_core_stop_omt ( void );
void dtvd_tuner_agc_mtx_002_msg_core_measure ( void );
void dtvd_tuner_agc_mtx_002_msg_core_stop ( void );
void dtvd_tuner_agc_mtx_002_timeout_agc ( void );
void dtvd_tuner_agc_mtx_003_msg_core_stop ( void );
void dtvd_tuner_agc_mtx_003_timeout_agc ( void );
void dtvd_tuner_agc_mtx_004_msg_core_measure ( void );
void dtvd_tuner_agc_mtx_004_msg_core_stop ( void );
void dtvd_tuner_agc_mtx_004_timeout_agc_watch ( void );

void dtvd_tuner_agc_mtx_005_msg_core_start_omt( void );
void dtvd_tuner_agc_mtx_005_msg_core_stop_omt( void );
void dtvd_tuner_agc_mtx_005_timeout_agc( void );
void dtvd_tuner_agc_mtx_006_msg_core_stop_omt ( void );
void dtvd_tuner_agc_mtx_006_timeout_agc ( void );
void dtvd_tuner_agc_mtx_007_msg_core_start_omt ( void );
void dtvd_tuner_agc_mtx_007_msg_core_stop_omt ( void );
void dtvd_tuner_agc_mtx_007_msg_core_measure_omt ( void );
void dtvd_tuner_agc_mtx_007_msg_core_measure_stop_omt ( void );
void dtvd_tuner_agc_mtx_008_msg_core_start_omt ( void );
void dtvd_tuner_agc_mtx_008_msg_core_stop_omt ( void );

void dtvd_tuner_cn_mtx_000_msg_core_init ( void );
void dtvd_tuner_cn_mtx_000_msg_core_init_omt ( void );
void dtvd_tuner_cn_mtx_001_msg_core_end ( void );
void dtvd_tuner_cn_mtx_001_msg_core_set ( void );
void dtvd_tuner_cn_mtx_001_msg_core_measure ( void );
void dtvd_tuner_cn_mtx_001_msg_core_stop ( void );
void dtvd_tuner_cn_mtx_001_msg_core_stop_omt ( void );
void dtvd_tuner_cn_mtx_001_msg_core_start_omt ( void );
void dtvd_tuner_cn_mtx_002_msg_core_measure ( void );
void dtvd_tuner_cn_mtx_002_msg_core_stop ( void );
void dtvd_tuner_cn_mtx_002_timeout_cn ( void );
void dtvd_tuner_cn_mtx_003_msg_core_measure ( void );
void dtvd_tuner_cn_mtx_003_msg_core_stop ( void );
void dtvd_tuner_cn_mtx_003_timeout_cn_second ( void );
void dtvd_tuner_cn_mtx_004_msg_core_measure ( void );
void dtvd_tuner_cn_mtx_004_msg_core_stop ( void );
void dtvd_tuner_cn_mtx_004_timeout_cn_second ( void );
void dtvd_tuner_cn_mtx_004_timeout_cn_monitor ( void );
void dtvd_tuner_cn_mtx_005_msg_core_stop_omt ( void );
void dtvd_tuner_cn_mtx_005_msg_core_measure_omt ( void );
void dtvd_tuner_cn_mtx_006_msg_core_stop_omt ( void );
void dtvd_tuner_cn_mtx_006_timeout_cn_second ( void );

void dtvd_tuner_ber_mtx_000_msg_core_init ( void );
void dtvd_tuner_ber_mtx_000_msg_core_init_omt ( void );
void dtvd_tuner_ber_mtx_001_msg_core_end ( void );
void dtvd_tuner_ber_mtx_001_msg_core_set ( void );
void dtvd_tuner_ber_mtx_001_msg_core_start ( void );
void dtvd_tuner_ber_mtx_001_msg_core_stop ( void );
void dtvd_tuner_ber_mtx_001_msg_core_start_omt ( void );
void dtvd_tuner_ber_mtx_001_msg_core_stop_omt ( void );
void dtvd_tuner_ber_mtx_002_msg_core_stop ( void );
void dtvd_tuner_ber_mtx_002_timeout_ber_fst ( void );
void dtvd_tuner_ber_mtx_003_msg_core_stop ( void );
void dtvd_tuner_ber_mtx_003_timeout_ber ( void );
void dtvd_tuner_ber_mtx_003_timeout_ber_watch ( void );
void dtvd_tuner_ber_mtx_004_msg_core_stop_omt ( void );
void dtvd_tuner_ber_mtx_004_msg_core_get_omt ( void );
void dtvd_tuner_ber_mtx_005_msg_core_stop_omt ( void );
void dtvd_tuner_ber_mtx_005_msg_core_getstop_omt ( void );
void dtvd_tuner_ber_mtx_005_timeout_ber ( void );
void dtvd_tuner_ber_mtx_006_msg_core_stop_omt ( void );
void dtvd_tuner_ber_mtx_006_msg_core_measure_omt ( void );

void dtvd_tuner_tmcc_mtx_000_msg_tuner_init ( void );
void dtvd_tuner_tmcc_mtx_001_msg_tuner_end ( void );
void dtvd_tuner_tmcc_mtx_001_msg_tuner_infoget ( void );
void dtvd_tuner_tmcc_mtx_001_msg_tuner_stop ( void );
void dtvd_tuner_tmcc_mtx_002_msg_tuner_stop ( void );
void dtvd_tuner_tmcc_mtx_002_timeout_tmcc_get ( void );
void dtvd_tuner_tmcc_mtx_003_msg_tuner_stop ( void );
void dtvd_tuner_tmcc_mtx_003_timeout_tmcc_watch ( void );

void dtvd_tuner_state_mtx_000_msg_tuner_init ( void );
void dtvd_tuner_state_mtx_001_msg_tuner_end ( void );
void dtvd_tuner_state_mtx_001_msg_tuner_start ( void );
void dtvd_tuner_state_mtx_002_msg_tuner_stop ( void );
void dtvd_tuner_state_mtx_002_msg_tuner_confirm ( void );
void dtvd_tuner_state_mtx_002_timeout_state ( void );

void dtvd_tuner_autoeco_mtx_000_msg_core_init( void );
void dtvd_tuner_autoeco_mtx_001_msg_core_end( void );
void dtvd_tuner_autoeco_mtx_001_msg_core_start( void );
void dtvd_tuner_autoeco_mtx_002_msg_core_stop( void );
void dtvd_tuner_autoeco_mtx_002_msg_cn_notice( void );

void dtvd_tuner_autoeco_mtx_001_msg_core_start_omt( void );
void dtvd_tuner_autoeco_mtx_003_msg_core_stop_omt( void );
void dtvd_tuner_autoeco_mtx_003_msg_timeout_autoeco_watch_omt( void );

#endif 

