#ifndef _DTVD_TMCC_INFO_H_
#define _DTVD_TMCC_INFO_H_

enum TMCC_MODULATION
{
    QPSK_RATE_1_2 = 0,
    QPSK_RATE_2_3,
    QAM16_1_2,
    NOTKNOW_MODULE_RATE,
};

enum TMCC_MODULATION TMCC_get_modulation(void);

#endif

