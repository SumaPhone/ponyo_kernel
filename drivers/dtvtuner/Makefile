# Makefile for dtvtuner

#target module name
DRV_NAME = dtvtuner

#
# Compile options for release version.
#

EXTRA_CFLAGS += -DNDEBUG
EXTRA_CFLAGS += -g0
EXTRA_CFLAGS += -Idrivers/dtvtuner/common
EXTRA_CFLAGS += -Idrivers/dtvtuner/drv/inc


obj-$(CONFIG_DTVTUNER_DRIVER) += $(DRV_NAME).o

$(DRV_NAME)-objs = \
	drv/src/dtvd_tuner_agc_api.o \
	drv/src/dtvd_tuner_agc_com.o \
	drv/src/dtvd_tuner_agc_mtx000.o \
	drv/src/dtvd_tuner_agc_mtx001.o \
	drv/src/dtvd_tuner_agc_mtx002.o \
	drv/src/dtvd_tuner_agc_mtx003.o \
	drv/src/dtvd_tuner_agc_mtx004.o \
	drv/src/dtvd_tuner_agc_mtx005.o \
	drv/src/dtvd_tuner_agc_mtx006.o \
	drv/src/dtvd_tuner_agc_mtx007.o \
	drv/src/dtvd_tuner_agc_mtx008.o \
	drv/src/dtvd_tuner_agc_trans.o \
	drv/src/dtvd_tuner_ant_api.o \
	drv/src/dtvd_tuner_ant_com.o \
	drv/src/dtvd_tuner_ant_mtx000.o \
	drv/src/dtvd_tuner_ant_mtx001.o \
	drv/src/dtvd_tuner_ant_trans.o \
	drv/src/dtvd_tuner_autoeco_api.o \
	drv/src/dtvd_tuner_autoeco_com.o \
	drv/src/dtvd_tuner_autoeco_mtx000.o \
	drv/src/dtvd_tuner_autoeco_mtx001.o \
	drv/src/dtvd_tuner_autoeco_mtx002.o \
	drv/src/dtvd_tuner_autoeco_mtx003.o \
	drv/src/dtvd_tuner_autoeco_trans.o \
	drv/src/dtvd_tuner_ber_api.o \
	drv/src/dtvd_tuner_ber_com.o \
	drv/src/dtvd_tuner_ber_mtx000.o \
	drv/src/dtvd_tuner_ber_mtx001.o \
	drv/src/dtvd_tuner_ber_mtx002.o \
	drv/src/dtvd_tuner_ber_mtx003.o \
	drv/src/dtvd_tuner_ber_mtx004.o \
	drv/src/dtvd_tuner_ber_mtx005.o \
	drv/src/dtvd_tuner_ber_mtx006.o \
	drv/src/dtvd_tuner_ber_trans.o \
	drv/src/dtvd_tuner_cn_api.o \
	drv/src/dtvd_tuner_cn_com.o \
	drv/src/dtvd_tuner_cn_mtx000.o \
	drv/src/dtvd_tuner_cn_mtx001.o \
	drv/src/dtvd_tuner_cn_mtx002.o \
	drv/src/dtvd_tuner_cn_mtx003.o \
	drv/src/dtvd_tuner_cn_mtx004.o \
	drv/src/dtvd_tuner_cn_mtx005.o \
	drv/src/dtvd_tuner_cn_mtx006.o \
	drv/src/dtvd_tuner_cn_trans.o \
	drv/src/dtvd_tuner_com_dev.o \
	drv/src/dtvd_tuner_com_std.o \
	drv/src/dtvd_tuner_core_api.o \
	drv/src/dtvd_tuner_core_com.o \
	drv/src/dtvd_tuner_core_mtx000.o \
	drv/src/dtvd_tuner_core_mtx001.o \
	drv/src/dtvd_tuner_core_mtx002.o \
	drv/src/dtvd_tuner_core_mtx003.o \
	drv/src/dtvd_tuner_core_mtx004.o \
	drv/src/dtvd_tuner_core_mtx005.o \
	drv/src/dtvd_tuner_core_mtx006.o \
	drv/src/dtvd_tuner_core_mtx007.o \
	drv/src/dtvd_tuner_core_mtx008.o \
	drv/src/dtvd_tuner_core_mtx009.o \
	drv/src/dtvd_tuner_core_mtx010.o \
	drv/src/dtvd_tuner_core_mtx011.o \
	drv/src/dtvd_tuner_core_mtx012.o \
	drv/src/dtvd_tuner_core_mtx013.o \
	drv/src/dtvd_tuner_core_mtx014.o \
	drv/src/dtvd_tuner_core_trans.o \
	drv/src/dtvd_tuner_driver.o \
	drv/src/dtvd_tuner_main.o \
	drv/src/dtvd_tuner_main_anlz.o \
	drv/src/dtvd_tuner_main_mtx.o \
	drv/src/dtvd_tuner_msg.o \
	drv/src/dtvd_tuner_ram.o \
	drv/src/dtvd_tuner_rom.o \
	drv/src/dtvd_tuner_state_api.o \
	drv/src/dtvd_tuner_state_com.o \
	drv/src/dtvd_tuner_state_mtx000.o \
	drv/src/dtvd_tuner_state_mtx001.o \
	drv/src/dtvd_tuner_state_mtx002.o \
	drv/src/dtvd_tuner_state_trans.o \
	drv/src/dtvd_tuner_timer.o \
	drv/src/dtvd_tuner_tmcc_api.o \
	drv/src/dtvd_tuner_tmcc_com.o \
	drv/src/dtvd_tuner_tmcc_mtx000.o \
	drv/src/dtvd_tuner_tmcc_mtx001.o \
	drv/src/dtvd_tuner_tmcc_mtx002.o \
	drv/src/dtvd_tuner_tmcc_mtx003.o \
	drv/src/dtvd_tuner_tmcc_trans.o \
	drv/src/dtvd_tuner_tsprcv.o \
	drv/src/dtvd_tuner_tsprcv_api.o \
	drv/src/dtvd_tuner_tsprcv_trans.o \
	drv/src/dtvd_tuner_tuner_api.o \
	drv/src/dtvd_tuner_tuner_com.o \
	drv/src/dtvd_tuner_tuner_mtx000.o \
	drv/src/dtvd_tuner_tuner_mtx001.o \
	drv/src/dtvd_tuner_tuner_mtx002.o \
	drv/src/dtvd_tuner_tuner_mtx003.o \
	drv/src/dtvd_tuner_tuner_mtx004.o \
	drv/src/dtvd_tuner_tuner_mtx005.o \
	drv/src/dtvd_tuner_tuner_mtx006.o \
	drv/src/dtvd_tuner_tuner_mtx007.o \
	drv/src/dtvd_tuner_tuner_mtx008.o \
	drv/src/dtvd_tuner_tuner_mtx009.o \
	drv/src/dtvd_tuner_tuner_mtx010.o \
	drv/src/dtvd_tuner_tuner_trans.o  \
        drv/src/dtvd_tuner_cit_com.o \
        drv/src/dtvd_tmcc_info.o

