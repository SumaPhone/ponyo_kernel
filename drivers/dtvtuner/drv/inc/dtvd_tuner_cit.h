/*
     Copyright 2011 Smart Wireless Corporation, All rights resered,

     Filename: dtvd_tuner_cit.h

     Description:

    -------------------------------------------------------------------------------
    Revision History                         
    Data                                               Author Name                                 Brief Description
    2011-7-18                                    leijun                                                DTV CIT test head document
    -------------------------------------------------------------------------------

*/

#ifndef DTVD_TUNER_CIT_H_
#define DTVD_TUNER_CIT_H_

enum MONITOR{
    CN_TEST = 0,
    BER_TEST,
};

enum static_varible_num
{
    SCANNING_CHANNEL_NUM = 0,
    CN_MONITORING_NUM,
    MER_MONITORING_NUM,
    MER_LAST_VALUE_NUM,   
};

signed int dtvd_tuner_reglist_com_read_CIT_TEST(char *value, enum MONITOR monitor);

signed int dtvd_tuner_rssi_com_read_CIT_TEST(char *value);

signed int get_signal_strong(void);

signed int dtvd_tuner_cn_com_read_CIT_TEST(long *value);

signed int dtvd_tuner_ber_com_read_CIT_TEST( long *value );

signed int dtvd_tuner_ber_com_read_CIT( long *value );

signed int dtvd_tuner_core_search_channel(char *search_channels);

signed int dtvd_tuner_vber_com_read_TEST( long *value );	

signed int dtvd_tuner_mer_com_read( unsigned int *value );	

signed int dtv_request_channel_scan(unsigned int kind);

void set_cit_static_variable(int num, void *data);

bool current_channel_isSPec(unsigned char current_channel);

#endif

