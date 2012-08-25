#include "dtvd_tuner_com.h"
#include "dtvd_tmcc_info.h"
#include "dtvtuner.h"

enum TMCC_MODULATION TMCC_get_modulation(void)
{

    DTVD_TUNER_COM_I2C_DATA_t i2c_data;   
    int ret;
    unsigned char  module=0x00, rate=0x00;	
    unsigned char addr;
    unsigned char temp_module, temp_rate;

    i2c_data.adr = 0x6d;
    i2c_data.data= 0x86;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    addr = 0x6e;
    ret = dtvd_tuner_com_dev_i2c_read(1, &addr, &module);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    temp_module = (module & 0x70) >> 4;
    if (!((temp_module== 0x1) || (temp_module== 0x2)))
    {
        printk("get the invalid TMCC modulation(0x%x) in function %s\n", temp_module,  __func__);	
        return NOTKNOW_MODULE_RATE;	
    }

    i2c_data.adr = 0x6d;
    i2c_data.data= 0x87;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    addr = 0x6e;
    ret = dtvd_tuner_com_dev_i2c_read(1, &addr, &rate);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    temp_rate= (rate & 0x70) >> 4;
    if ( !((temp_rate== 0) || (temp_rate== 1)))
    {
        printk("get the invalid TMCC rate (0x%x), in function %s\n", temp_rate, __func__);
        return D_DTVD_TUNER_NG;	
    }

    if ((temp_module == 1) && (temp_rate == 0))
    {
        return QPSK_RATE_1_2;
    }
    else if ((temp_module == 1) && (temp_rate == 1))
    {
        return QPSK_RATE_2_3;
    }
    else if((temp_module == 2) && (temp_rate == 0))
    {
        return QAM16_1_2;
    }
    else
        return NOTKNOW_MODULE_RATE;
	
     return NOTKNOW_MODULE_RATE;

}

