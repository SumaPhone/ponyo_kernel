/*
     Copyright 2011 Smart Wireless Corporation, All rights resered,

     Filename: dtvd_tuner_cit_com.c

     Description:

    -------------------------------------------------------------------------------
    Revision History                         
    Data                      Author Name                  Brief Description
    2011-7-18                 leijun                       DTV CIT test code
    -------------------------------------------------------------------------------

*/

#include "dtvd_tuner_com.h"
#include "dtvd_tuner_cn.h"
#include "dtvd_tuner_ber.h"
#include "dtvd_tuner_cit.h"
#include "dtvd_tuner_core.h"
#include "dtvd_tuner_pro.h"
#include "dtvd_tuner_tuner.h"
#include "dtvd_tuner_mtx.h"

static unsigned char SCANNING_CHANNEL = 0;
static unsigned char CN_MONITORING = 0;
static unsigned char MER_MONITORING = 0;
static unsigned int STATIC_MER_LAST_VALUE = 0;

/******************************************************************************
                                 REGLIST  partion
******************************************************************************/
signed int dtvd_tuner_reglist_com_read_CIT_TEST( char *value, enum MONITOR monitor)
{
    char addr;
    char reglist;
    signed int ret;
    int count = 50;
	
    addr = 0x0a;

    while(1)
    {
        ret = dtvd_tuner_com_dev_i2c_read(1, &addr, &reglist);
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
        if (monitor == CN_TEST)
        {
            if(reglist < 0x07)
            {
                msleep(100);
                count--;
                if (count < 0)
                {
                    printk("***%s***signal not locked\n", __func__);
                    return -2;			
                }
                continue;
            }
            else
                break;
        }else if (monitor == BER_TEST){
            if(reglist < 0x09)
            {
                msleep(100);
                count--;
                if (count < 0)
                {
                    printk("****%s***signal not lock mer\n", __func__);
                    return -2;
                }
                continue;
            }
            else
                break;
       } 
    }
   
    *value = reglist;
	
    return D_DTVD_TUNER_OK;
}


/******************************************************************************
                               RSSI  partion
******************************************************************************/
signed int dtvd_tuner_rssi_com_read_CIT_TEST( char *value_cit )
{

    DTVD_TUNER_COM_I2C_DATA_t i2c_data;
    signed int ret;
    unsigned char addr;

    unsigned char lna_state;
    unsigned char agc_dt;
    unsigned char lna_dt;
    unsigned char mix_dt;
    unsigned char lsi_dt;

    char *value = value_cit; 
 
    i2c_data.adr = 0x32;
    i2c_data.data= 0x20;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    addr = 0x33;
    ret = dtvd_tuner_com_dev_i2c_read(1, &addr, &lna_dt);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    *value = lna_dt;
    value++;

    i2c_data.adr = 32;
    i2c_data.data= 0x21;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    addr = 0x33;
    ret = dtvd_tuner_com_dev_i2c_read(1, &addr, &mix_dt);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    *value = mix_dt;
    value++;

    i2c_data.adr = 0x04;
    i2c_data.data= 0x3a;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    addr = 0x05;
    ret = dtvd_tuner_com_dev_i2c_read(1, &addr, &agc_dt);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    *value = agc_dt;
    value++;

    addr = 0xe0;
    ret = dtvd_tuner_com_dev_i2c_read(1, &addr, &lsi_dt);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    
    lna_state = lsi_dt & 0x10;
    *value = lna_state;
    
    return 0;
}

#if 0
signed int rssi_covert_normal(unsigned char *raw)
{
    int lna_dt,   mix_dt,  agc_dt;
    int lna_state;

    char *temp = raw;
    int value;

    lna_dt = *temp ;
    temp++;
    mix_dt = *temp;
    temp++;
    agc_dt = *temp;
    temp++;
    lna_state = *temp;

    if (lna_state == 0x01)
    {
        value = -(70129 +10000 *  (lna_dt * 3 - 10) +10000 *  (mix_dt - 3) + 2500 *  (agc_dt)) / 9772;
//        printk("*******RSSI_NEGATIVE_DT****rssi=%d\n", value);
    }
    else
    {
        value = (78067 - (10000 * (lna_dt * 3 - 10) + 10000 * (mix_dt - 3) + 2500 * (agc_dt ))) / 9185;	
//        printk("*******RSSI_POSITIVE_DT****rssi=%d\n", value);				
    }    

    return value;
 
}


//this is use rssi for signal strong 
signed int get_signal_strong(void)
{

    int ret;
    int rssi;
    unsigned char RSSI_VALUE_RAW[4];
	
    ret = dtvd_tuner_rssi_com_read_CIT_TEST(RSSI_VALUE_RAW);
    if (ret != D_DTVD_TUNER_OK){
        printk("*****read rssi raw value fail**%s\n", __func__);
        return D_DTVD_TUNER_NG; 
    }

    rssi =  rssi_covert_normal(RSSI_VALUE_RAW);

    return rssi;

}
#endif

#if 0
//these code for C\N monitor
signed int cn_covert_normal(long cn_read_value)
{
    int ret ;
    int A = 0;
    int B = 0;
    int C = 0;

    if(cn_read_value>=18187 ) { 
        A= 10; 
        B=1461; 
        C=18187;
    }else if(cn_read_value>=16534) {
        A= 20; 
        B=1653; 
        C=16534; 
    }else if(cn_read_value>=14823){ 
        A= 30; 
        B=1711; 
        C=14823; 
    }else if(cn_read_value>=13161){ 
        A= 40; 
        B=1662; 
        C=13161; 
    }else if( cn_read_value>=11622 ) { 
        A= 50; 
        B=1539; 
        C=11622; 
    }else if( cn_read_value>=10279 ) { 
            A= 60; 
            B=1343; 
            C=10279; 
    }else if( cn_read_value>= 9089 ) { 
            A= 70; 
            B=1190; 
            C= 9089; 
    }else if( cn_read_value>= 8042 ) { 
            A= 80; 
            B=1047; 
            C= 8042; 
    }else if( cn_read_value>= 7137 ) { 
            A= 90; 
            B= 905; 
            C= 7137; 
    }else if( cn_read_value>= 6342 ) { 
            A=100; 
            B= 795; 
            C= 6342; 
    }else if( cn_read_value>= 5641 ) { 
            A=110; 
            B= 701; 
            C= 5641; 
    }else if( cn_read_value>= 5030 ) { 
            A=120; 
            B= 611; 
            C= 5030; 
    }else if( cn_read_value>= 4474 ) { 
            A=130; 
            B= 556; 
            C= 4474; 
    }else if( cn_read_value>= 3988 ) { 
            A=140; 
            B= 486; 
            C= 3988; 
    }else if( cn_read_value>= 3556 ) { 
            A=150; 
            B= 432; 
            C= 3556; 
    }else if( cn_read_value>= 3180 ) { 
            A=160; 
            B= 376; 
            C= 3180; 
    }else if( cn_read_value>= 2841 ) { 
            A=170; 
            B= 339; 
            C= 2841; 
    }else if( cn_read_value>= 2541 ) { 
            A=180; 
            B= 300; 
            C= 2541; 
    }else if( cn_read_value>= 2276 ) { 
            A=190; 
            B= 265; 
            C= 2276; 
    }else if( cn_read_value>= 2038 ) { 
            A=200; 
            B= 238; 
            C= 2038; 
    }else if( cn_read_value>= 1800 ) { 
            A=210; 
            B= 238; 
            C= 1800; 
    }else if( cn_read_value>= 1625 ) { 
            A=220; 
            B= 175; 
            C= 1625; 
    }else if( cn_read_value>= 1462 ) { 
            A=230; 
            B= 163; 
            C= 1462; 
    }else if( cn_read_value>= 1324 ) { 
            A=240; 
            B= 138; 
            C= 1324; 
    }else if( cn_read_value>= 1175 ) { 
            A=250; 
            B= 149; 
            C= 1175; 
    }else if( cn_read_value>= 1063 ) { 
            A=260; 
            B= 112; 
            C= 1063; 
    }else if( cn_read_value>=  980 ) { 
            A=270; 
            B=  83; 
            C=  980; 
    }else if( cn_read_value>=  907 ) { 
            A=280; 
            B=  73; 
            C=  907; 
    }else if( cn_read_value>=  840 ) { 
            A=290; 
            B=  67; 
            C=  840; 
    }else if( cn_read_value>=  788 ) { 
            A=300; 
            B=  52; 
            C=  788; 
    }

    if(cn_read_value >= 19648 )
        ret = 0 ;
    else if( cn_read_value < 788 ) 
        ret = 300 ;
    else               
        ret = (int)( A-(10*((int)(cn_read_value)-C)-B/2)/B) ;

    return ret/10;

}

//this is use C\N for signal strong
signed int get_signal_strong(void)
{
    int ret;
    long value;
    int cn;

    ret = dtvd_tuner_cn_com_read_CIT_TEST(&value);    
    if (ret != D_DTVD_TUNER_OK){
        return D_DTVD_TUNER_NG; 
    }

    cn = cn_covert_normal(value);
    printk("****cn=%d db\n", cn);

    return cn;

}
#endif

#if 1
// this code is for MER covert to C\N

//this is use C\N for signal strong
signed int get_signal_strong(void)
{
    int ret;
    unsigned int value;

    ret = dtvd_tuner_mer_com_read(&value);	    
    if (ret != D_DTVD_TUNER_OK){
        return ret; 
    }

    if (value < 0xa000)
        STATIC_MER_LAST_VALUE = value;
    else 
        value = STATIC_MER_LAST_VALUE;

    return value;
}

#endif

/******************************************************************************
                                 C/N  partion
******************************************************************************/
signed int dtvd_tuner_cn_com_read_CIT_TEST( long *value )
{

    DTVD_TUNER_COM_I2C_DATA_t i2c_data;
    signed int ret;
    unsigned char addr;
    unsigned long length;
    unsigned char data;
    signed int count = 300;
 
    *value = 0;

    ret = dtvd_tuner_reglist_com_read_CIT_TEST( &data, CN_TEST); //lock the signal
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return -2;
    }

    if (!CN_MONITORING){

        i2c_data.adr = D_DTVD_TUNER_REG_CNMODE;
        i2c_data.data= 0x04;
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
    
        //this code for layer
        i2c_data.adr = 0x50;
        i2c_data.data= 0xd6;
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        length = 1;
        addr = 0x51;
        ret = dtvd_tuner_com_dev_i2c_read( length, &addr, &data);
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
        printk("CN Monitor get TS LAYER= 0x%x\n", data);

        i2c_data.adr = 0x50;
        i2c_data.data= 0xd6;
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        i2c_data.adr = 0x51;
        i2c_data.data= data | 0x07; //set TS all layer
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
    
        i2c_data.adr = D_DTVD_TUNER_REG_CNCNT;
        i2c_data.data= 0x08;	 // this 0x0f is a value is not constant
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        CN_MONITORING++;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_CNCNT;
    i2c_data.data= 0x18;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_CNCNT;
    i2c_data.data= 0x08;	 // this 0x0f is a value is not constant
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    length = 1;
    addr = 0x45;
    while(1)
    {
        ret = dtvd_tuner_com_dev_i2c_read( length, &addr, &data);
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
        if (!((data & 0x40) >> 6))
        {
            msleep(10);
            count--;
            if (count < 0)
            {
                printk("****CN value read failed**%s\n", __func__);
                return D_DTVD_TUNER_NG;
            }
            continue;
        }
        break;
    }

    length = 1;	
    addr = 0x46;
    ret = dtvd_tuner_com_dev_i2c_read( length, &addr, &data);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    *value |= data;
    *value = (*value) << 8;

    addr = 0x47;
    ret = dtvd_tuner_com_dev_i2c_read( length, &addr, &data);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    *value |= data;

    return ret;

}


/*****************************************************************************
                                    SBER  partion
******************************************************************************/
signed int dtvd_tuner_ber_com_read_CIT( long *value )
{
    signed int ret;
    unsigned long length;
    unsigned char addr;
    unsigned char data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_RSBERDT0;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       &data  );        

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }
    *value |= data; 
    *value = (*value << 8);

    tdtvd_tuner_monitor.rx.ber.value.ext = data;

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;
    addr = D_DTVD_TUNER_REG_RSBERDT1;
    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                                                               &addr,           
                                                                               &data);        
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }
    *value |= data; 
    *value = (*value << 8);

    tdtvd_tuner_monitor.rx.ber.value.up = data;

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;
    addr = D_DTVD_TUNER_REG_RSBERDT2;
    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                                                              &addr,           
                                                                              &data);        
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }
    *value |= data; 

    tdtvd_tuner_monitor.rx.ber.value.low = data;

    tdtvd_tuner_monitor.rx.ber.value.result = D_DTVD_TUNER_GET_RESULT_OK;

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

signed int dtvd_tuner_ber_com_read_CIT_TEST( long *value )
{
    unsigned char data = 0x00;

    unsigned short period = 511;     //the 511 is for sber value convert, convert function is period/(period*204*8)
    signed int ret = D_DTVD_TUNER_OK;
    int test = 200;

    ret = dtvd_tuner_reglist_com_read_CIT_TEST( &data, BER_TEST); //lock the TMCC signal
    if (ret != D_DTVD_TUNER_OK)
        return D_DTVD_TUNER_NG;		

    ret = dtvd_tuner_ber_com_start_set_init(&period);
    if (ret != D_DTVD_TUNER_OK)
        return D_DTVD_TUNER_NG;

    ret = dtvd_tuner_ber_com_start_set();
    if (ret != D_DTVD_TUNER_OK)
        return D_DTVD_TUNER_NG;
//this code for compare sber, just for test	
/*	
    ret = dtvd_tuner_ber_com_read_CIT(value);
    if (ret != D_DTVD_TUNER_OK)
        return D_DTVD_TUNER_NG;
*/
    *value = 0;

    while(1)
    {
        ret = dtvd_tuner_ber_com_read_flag(&data);
        if (ret != D_DTVD_TUNER_OK)
            return D_DTVD_TUNER_NG;

        if (!(data & 0x1))
        {
            test--;
            if(test < 1)
            {
                printk("****SBER test fail*****ber flag = 0\n");            
                return D_DTVD_TUNER_NG;
            }
            msleep(100);
            continue;
        }
        else
        {
             printk("*********ber flag = 1\n");
        }
        break;
    }
   
    ret = dtvd_tuner_ber_com_read_CIT(value);
    if (ret != D_DTVD_TUNER_OK)
        return D_DTVD_TUNER_NG;

    return ret;
}


/*****************************************************************************
                                    VBER  partion
******************************************************************************/
signed int dtvd_tuner_vber_com_start_set_init( unsigned short * period )
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    if( period == NULL )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    //reset
    i2c_data.adr = 0x53;
    i2c_data.data= 0x00;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    i2c_data.adr = 0x53;
    i2c_data.data= 0x01;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= 0xA4;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= (unsigned char)( (unsigned short)( *period & 0xff0000 ) >> 16 );
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= 0xa5;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= (unsigned char)( (*period & 0xff00) >> 8 );
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= 0xa6;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= (unsigned char)( *period & 0xff );
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
	
    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_vber_com_start_set( void )	
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    i2c_data.adr = 0x52;
    i2c_data.data= 0x01;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

signed int dtvd_tuner_vber_com_read_flag( unsigned char *data )	
{
    signed int ret;
    unsigned long length;
    unsigned char addr;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ret = D_DTVD_TUNER_OK;
    length = D_DTVD_TUNER_REG_NO1;

    addr = 0x54;
    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                                                               &addr,           
                                                                                  data);         
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    return ret;
}

signed int dtvd_tuner_vber_com_read_CIT( long *value )	
{
    signed int ret;
    unsigned long length;
    unsigned char addr;
    unsigned char data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;
    addr = 0x55;
    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                                                               &addr,           
                                                                               &data);        
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    *value |= data; 
    *value = (*value << 8);

    data = 0x00;
    length = D_DTVD_TUNER_REG_NO1;
    addr = 0x56;
    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                                                              &addr,           
                                                                               &data);        
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    *value |= data; 
    *value = (*value << 8);

    data = 0x00;
    length = D_DTVD_TUNER_REG_NO1;
    addr = 0x57;
    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                                                               &addr,           
                                                                                &data);        
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    *value |= data; 
    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

signed int dtvd_tuner_vber_com_read_TEST( long *value )	
{
    unsigned char data = 0x00;
    unsigned short period = 511;
    signed int ret = D_DTVD_TUNER_OK;
    int test = 200;
	
    ret = dtvd_tuner_reglist_com_read_CIT_TEST( &data, BER_TEST); //lock the TMCC signal
    if (ret != D_DTVD_TUNER_OK)
        return D_DTVD_TUNER_NG;

    ret = dtvd_tuner_vber_com_start_set_init(&period);
    if (ret != D_DTVD_TUNER_OK)
        return D_DTVD_TUNER_NG;

    ret = dtvd_tuner_vber_com_start_set();
    if (ret != D_DTVD_TUNER_OK)
        return D_DTVD_TUNER_NG;

    *value = 0;

    while(1)
    {
        ret = dtvd_tuner_vber_com_read_flag(&data);
        if (ret != D_DTVD_TUNER_OK)
            return D_DTVD_TUNER_NG;

        if (!(data & 0x1))
        {
            test--;
            if(test < 1)
            {
                  printk("*******vber read flag failed ***%s\n", __func__);     
		return D_DTVD_TUNER_NG;
            }
            msleep(100);
            continue;
        }
        else
        {
             printk("*********ber flag = 1\n");
        }
        break;
    }
   
    ret = dtvd_tuner_vber_com_read_CIT(value);
    if (ret != D_DTVD_TUNER_OK)
        return D_DTVD_TUNER_NG;

    return ret;
}


/*****************************************************************************
                                 MER  partion
******************************************************************************/
//signed int mber_init_set(void)
signed int mer_init_set(void)
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    if (!MER_MONITORING)
    {
        i2c_data.adr = 0x50;
        i2c_data.data= 0x50;
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        i2c_data.adr = 0x51;
        i2c_data.data= 0x02;
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        i2c_data.adr = 0x50;
        i2c_data.data= 0x51;
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        i2c_data.adr = 0x51;
        i2c_data.data= 0x00;                      //symcount
        //i2c_data.data= 0x07;                         
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        MER_MONITORING++;
    }

    i2c_data.adr = 0x50;
    i2c_data.data= 0x50;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = 0x51;
    i2c_data.data= 0x03;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = 0x50;
    i2c_data.data= 0x50;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = 0x51;
    i2c_data.data= 0x02;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }		
	
    return ret;
}

signed int wait_flag(void)
{
    signed int ret;
    unsigned long length;
    unsigned char addr;
    unsigned char data;
    signed long count = 1000;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    data = 0x00;
    length = D_DTVD_TUNER_REG_NO1;

    while(1)
    {
        i2c_data.adr = 0x50;
        i2c_data.data= 0x58;
        ret = dtvd_tuner_com_dev_i2c_write( length, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        addr = 0x51;
        ret = dtvd_tuner_com_dev_i2c_read( length,          
                                                                                    &addr,           
                                                                                    &data  );        
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return ret;
        }

        if (!(data & 0x1))
        {
            count--;
            if(count < 0)
            {
                printk("***mer get failed\n");
                ret = D_DTVD_TUNER_NG;
                break;
            }
     
            msleep(1);
            continue;
        }
        else
            break;
    }
	
    return ret;
}

signed int read_result(unsigned int *value_callbak)
{
    signed int ret;
    unsigned long length;
    unsigned char addr;
    unsigned char data;
    unsigned int value = 0;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    *value_callbak = 0;

    data = 0x00;
    length = D_DTVD_TUNER_REG_NO1;

    i2c_data.adr = 0x50;
    i2c_data.data= 0x52;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    addr = 0x51;
    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                                                                 &addr,           
                                                                                 &data);        
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }
	
    value |= data;
    value = (value << 8);

    i2c_data.adr = 0x50;
    i2c_data.data= 0x53;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    addr = 0x51;
    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                                                               &addr,           
                                                                               &data);        
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }
	
    value |= data;

    *value_callbak = value;	

    return ret;

}

signed int dtvd_tuner_mer_com_read( unsigned int *value )	
{
    signed int ret;
    unsigned char data = 0x00;
    *value = 0;
	
    ret = dtvd_tuner_reglist_com_read_CIT_TEST( &data, BER_TEST); //lock the TMCC signal
    if (ret != D_DTVD_TUNER_OK)
        return ret;
	
    ret = mer_init_set();
    if (ret != D_DTVD_TUNER_OK)
        return ret;
	
    ret = wait_flag();
    if (ret != D_DTVD_TUNER_OK)
        return ret;
	
    ret = read_result(value);
    if (ret != D_DTVD_TUNER_OK)
        return ret;
	
    return 0;
}


/******************************************************************************
                            CHANNEL SCAN partion
*******************************************************************************/
static int write_to_register(unsigned char addr, unsigned char data)
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t  i2c_data;

    i2c_data.adr = addr;
    i2c_data.data= data;

    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
	
    return ret;
}

static int read_from_register(unsigned char addr, unsigned char *data)
{
   unsigned long length;
   signed int ret;

    length = D_DTVD_TUNER_REG_NO1;
    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       data  );         

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }
	
    return ret;
}

static int LNA_init(void)
{
    signed int ret;

    ret = write_to_register(0xe3, 0x00);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0xe0, 0x00); //LNA open
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
	
    return ret;
	
}

static int RF_reset(void)
{
    signed int ret;

    ret = write_to_register(0x08, 0x01);  // for sequence init
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
   	
    ret = write_to_register(0x31, 0x00);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0x36, 0x10);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0x32, 0x00);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0x33, 0xff);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0x32, 0x10);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0x33, 0x00);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0x31, 0x80);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }	

    ret = write_to_register(0x08, 0x00); // for sequence re-start
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }	
	
    return ret;
	
}

static int AGC_operating(void)
{
    signed int ret;
	
    ret = write_to_register(0x28, 0x16);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0x29, 0x00);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }	

    ret = write_to_register(0x2A, 0x00);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0x2B, 0xc0);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
	
    return ret;	
	
}

static int CLEAR_int(void)
{
    signed int ret;
    char data;

    ret = read_from_register(0xee, &data);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    ret = write_to_register(0xee, (data & 0xfe));
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
	
    return ret;
	
}

static int SEARCH_INIT(void)
{
    signed int ret;
    char data;
    ret = read_from_register(0x07, &data);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
	
    ret = write_to_register(0x07, (0xef & data));   //set perform mode detection
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0xF0, 0x01); // after judging whether OFDM exit, judges whether it is TMCC parameter
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0xF1, 0x21);   
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    } 	

    ret = write_to_register(0xF2, 0x21);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    } 	

    ret = write_to_register(0x40, 0x35);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    } 	
    ret = write_to_register(0x41, 0x28);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    } 	

    ret = write_to_register(0x40, 0x0d);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    } 	
    ret = read_from_register(0x41, &data);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
	
    ret = write_to_register(0x41, (0xf3 & data) | 0x08);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
	
    return ret;
	
}

static int START_scan(void)
{
    signed int ret;
    unsigned char data;
    unsigned char temp1;
    unsigned char temp2;
    int count = 300;

    if ((SCANNING_CHANNEL < 13) || (SCANNING_CHANNEL > 62))
        return D_DTVD_TUNER_NG;

    ret = write_to_register(0xf3, SCANNING_CHANNEL);  // set the start search channel; 
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0xf4, SCANNING_CHANNEL); // set the end search channel;   
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }	
	
    ret = write_to_register(0xe7, 0x01);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

//    ret = write_to_register(0xe6, 0x20);
    ret = write_to_register(0xe6, 0x10);	
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

//    ret = write_to_register(0xe6, 0x21);
    ret = write_to_register(0xe6, 0x00);  // clear the search result 
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = write_to_register(0xe6, 0x02); //search start
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }	

    while(1)
    {
        ret = read_from_register(0xed, &data);
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
		  
        if (data & 0x01)
            break;
        else
        {
            msleep(10);
            count--;
            if (count < 0)
            {
                printk("***scan channel fail, ****%s\n",  __func__);            
                return -2;
            }
            continue;
        }
    }

    ret = read_from_register(0xee, &temp1);   
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    temp2 = temp1 | 0x10;
    ret = write_to_register(0xee, temp2);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
    temp2 = temp1 & 0xef;
    ret = write_to_register(0xee, temp2);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }	
	
    return ret;
	
}

static int READ_scan_channel(unsigned char *channels)
{
    signed int ret;
	
    ret = write_to_register(0xe8, 0x01);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = read_from_register(0xe9, channels);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    channels++;
    ret = write_to_register(0xe8, 0x02);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = read_from_register(0xe9, channels);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    channels++;
    ret = write_to_register(0xe8, 0x03);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = read_from_register(0xe9, channels);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    channels++;
    ret = write_to_register(0xe8, 0x04);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = read_from_register(0xe9, channels);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    channels++;
    ret = write_to_register(0xe8, 0x05);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = read_from_register(0xe9, channels);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    channels++;
    ret = write_to_register(0xe8, 0x06);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = read_from_register(0xe9, channels);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    channels++;
    ret = write_to_register(0xe8, 0x07);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    ret = read_from_register(0xe9, channels);
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }
	
    return ret;	
   
}

signed int init_for_channel_scan(void)
{
    int ret;

    ret = LNA_init();
    if(ret != D_DTVD_TUNER_OK)
    {
        printk("****LNA_init wrong\n");
        return D_DTVD_TUNER_NG;
    }

    ret = RF_reset();
    if(ret != D_DTVD_TUNER_OK)
    {
        printk("****RF_reset wrong\n");
        return D_DTVD_TUNER_NG;
    }

    ret = AGC_operating();
    if(ret != D_DTVD_TUNER_OK)
    {
        printk("****AGC_operating wrong\n");
        return D_DTVD_TUNER_NG;
    }
    return ret;
}
EXPORT_SYMBOL(init_for_channel_scan);


signed int dtvd_tuner_core_search_channel(char *search_channels)
{
    int ret;
    unsigned char channels[7];
    char *temp = search_channels;
    int i;
	
    ret = LNA_init();
    if(ret != D_DTVD_TUNER_OK)
    {
        printk("****LNA_init wrong\n");
        return D_DTVD_TUNER_NG;
    }

    ret = RF_reset();
    if(ret != D_DTVD_TUNER_OK)
    {
        printk("****RF_reset wrong\n");
        return D_DTVD_TUNER_NG;
    }

    ret = AGC_operating();
    if(ret != D_DTVD_TUNER_OK)
    {
        printk("****AGC_operating wrong\n");
        return D_DTVD_TUNER_NG;
    }
	
    ret = CLEAR_int();
    if(ret != D_DTVD_TUNER_OK)
    {
        printk("****CLEAR_int wrong\n");
        return D_DTVD_TUNER_NG;
    }

    ret = SEARCH_INIT();
    if(ret != D_DTVD_TUNER_OK)
    {
        printk("****SEARCH_INIT wrong\n");
        return D_DTVD_TUNER_NG;
    }	
	
    ret = START_scan();
    if(ret != D_DTVD_TUNER_OK)
    {
        printk("****START_scan wrong\n");
        return ret;
    }

    ret = READ_scan_channel(channels);
    if(ret != D_DTVD_TUNER_OK)
    {
        printk("****READ_scan_channel wrong\n");
        return D_DTVD_TUNER_NG;
    }
/*	
    printk("\nscan the channel\n 1 chanels=0x%x, \n2 chanels=0x%x, \n"
		"3 chanels=0x%x,\n 4 chanels=0x%x, \n5 chanels=0x%x,\n"
		" 6 chanels=0x%x, \n7 chanels=0x%x\n", channels[0], 
		channels[1],channels[2],channels[3],channels[4],
		channels[5],channels[6]);
*/
    for(i = 0; i < 7; i++)
    {
        *temp = channels[i];
         temp++;
    }
	
    return ret;
	
}


void dtvd_tuner_channel_msg_tune_ok
(
    unsigned char agc, bool status       
)
{
    unsigned int                    len;
    DTVD_TUNER_MSGDATA_TUNE_OK_t    data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    len = sizeof( DTVD_TUNER_MSGDATA_TUNE_OK_t );

    dtvd_tuner_memset( &data, 0x00, len, len );

    data.seq_id = tdtvd_tuner_core.tune_seq_id;

    data.adj.mode = tdtvd_tuner_monitor.tune.adj.mode;

    data.adj.gi = tdtvd_tuner_monitor.tune.adj.gi;

    data.agc = (unsigned char)( 0xFF - agc );

    if (status)
        dtvd_tuner_core_trans_mdl_send_msg( D_DTVD_TUNER_MSGID_TUNE_OK,
                                                                                       &data, len );
    else
        dtvd_tuner_core_trans_mdl_send_msg( D_DTVD_TUNER_MSGID_TUNE_NG,
                                                                                       &data, len );		
		

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void set_cit_static_variable(int num, void *data)
{
    switch(num)
    {
    case SCANNING_CHANNEL_NUM:
        SCANNING_CHANNEL = *((unsigned char *)data);
        break;
    case CN_MONITORING_NUM:
        CN_MONITORING = *((unsigned char *)data);
        break;
    case MER_MONITORING_NUM:
        MER_MONITORING = *((unsigned char *)data);
        break;
    case MER_LAST_VALUE_NUM:
        STATIC_MER_LAST_VALUE = 0;
        break;	
    default:
        break;
    }
}


unsigned char get_scan_channel_num(void)
{
    return SCANNING_CHANNEL;
}
EXPORT_SYMBOL(get_scan_channel_num);

signed int check_channel_scan_out(char *search_channels)
{

    unsigned char *temp = search_channels;

    if ((SCANNING_CHANNEL < 13) || (SCANNING_CHANNEL > 62))
        return D_DTVD_TUNER_NG;
    if ((SCANNING_CHANNEL == 13) && ((*temp) & 0x10))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 14) && ((*temp) & 0x20))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 15) && ((*temp) & 0x40))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 16) && ((*temp) & 0x80))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 17) && ((*(temp + 1)) & 0x01))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 18) && ((*(temp + 1)) & 0x02))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 19) && ((*(temp + 1)) & 0x04))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 20) && ((*(temp + 1)) & 0x08))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 21) && ((*(temp + 1)) & 0x10))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 22) && ((*(temp + 1)) & 0x20))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 23) && ((*(temp + 1)) & 0x40))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 24) && ((*(temp + 1)) & 0x80))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 25) && ((*(temp + 2)) & 0x01))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 26) && ((*(temp + 2)) & 0x02))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 27) && ((*(temp + 2)) & 0x04))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 28) && ((*(temp + 2)) & 0x08))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 29) && ((*(temp + 2)) & 0x10))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 30) && ((*(temp + 2)) & 0x20))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 31) && ((*(temp + 2)) & 0x40))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 32) && ((*(temp + 2)) & 0x80))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 33) && ((*(temp + 3)) & 0x01))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 34) && ((*(temp + 3)) & 0x02))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 35) && ((*(temp + 3)) & 0x04))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 36) && ((*(temp + 3)) & 0x08))
        return D_DTVD_TUNER_OK;	
    else if((SCANNING_CHANNEL == 37) && ((*(temp + 3)) & 0x10))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 38) && ((*(temp + 3)) & 0x20))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 39) && ((*(temp + 3)) & 0x40))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 40) && ((*(temp + 3)) & 0x80))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 41) && ((*(temp + 4)) & 0x01))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 42) && ((*(temp + 4)) & 0x02))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 43) && ((*(temp + 4)) & 0x04))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 44) && ((*(temp + 4)) & 0x08))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 45) && ((*(temp + 4)) & 0x10))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 46) && ((*(temp + 4)) & 0x20))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 47) && ((*(temp + 4)) & 0x40))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 48) && ((*(temp + 4)) & 0x80))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 49) && ((*(temp + 5)) & 0x01))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 50) && ((*(temp + 5)) & 0x02))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 51) && ((*(temp + 5)) & 0x04))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 52) && ((*(temp + 5)) & 0x08))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 53) && ((*(temp + 5)) & 0x10))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 54) && ((*(temp + 5)) & 0x20))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 55) && ((*(temp + 5)) & 0x40))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 56) && ((*(temp + 5)) & 0x80))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 57) && ((*(temp + 6)) & 0x01))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 58) && ((*(temp + 6)) & 0x02))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 59) && ((*(temp + 6)) & 0x04))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 60) && ((*(temp + 6)) & 0x08))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 61) && ((*(temp + 6)) & 0x10))
        return D_DTVD_TUNER_OK;
    else if((SCANNING_CHANNEL == 62) && ((*(temp + 6)) & 0x20))
        return D_DTVD_TUNER_OK;
    else
        return D_DTVD_TUNER_NG;	

}
EXPORT_SYMBOL(check_channel_scan_out);

bool current_channel_isSPec(unsigned char current_channel)
{
    const unsigned char SpecChannel[] = {33};
    int i;
    for (i = 0; i < sizeof(SpecChannel); i++)
    {
        if (current_channel == SpecChannel[i])
            return true;
    }
    return false;
}

signed int dtv_request_channel_scan(unsigned int kind)
{

    unsigned char search_channels[7];
    signed int ret;
	
    ret = dtvd_tuner_core_search_channel(search_channels);
    if (ret == -2)
    {  
        dtvd_tuner_tuner_inevt_core_tune_ng( D_DTVD_TUNER_TUNE_NG_CAUSE8 );	
        ret = write_to_register(0xe6, 0x00);  // stop search channel 
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
        printk("*****channel scan failed -2!\n");		
    }
    else if (ret == D_DTVD_TUNER_OK)
    {
    
        dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_SEARCH );

        ret = write_to_register(0xe6, 0x10);  // reset search channel 
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }

        ret = write_to_register(0xe6, 0x00);  // stop search channel 
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
	
        ret = check_channel_scan_out(search_channels);
        if (ret != D_DTVD_TUNER_OK)
        {
            dtvd_tuner_channel_msg_tune_ok(0, false);		
        }else{
            dtvd_tuner_channel_msg_tune_ok(0, true);
        }

        return D_DTVD_TUNER_OK;
    }
    else
    {   
    
        dtvd_tuner_timer_stop( D_DTVD_TUNER_TIMER_SEARCH );

        ret = write_to_register(0xe6, 0x10);  // reset search channel 
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
		
        ret = write_to_register(0xe6, 0x00);  // stop search channel
        if( ret != D_DTVD_TUNER_OK )
        {
            DTVD_DEBUG_MSG_EXIT();
            return D_DTVD_TUNER_NG;
        }
        dtvd_tuner_tuner_inevt_core_tune_ng( D_DTVD_TUNER_TUNE_NG_CAUSE8 );	
        printk("*****channel scan failed -1!\n");		
        return D_DTVD_TUNER_NG;
    }
    return D_DTVD_TUNER_OK;
}

