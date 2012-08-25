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
#include "dtvd_tuner_cn.h"

static const DTVD_TUNER_CN_SYMTOTIME_t c_symcount[8] = {
    {   2 , 0 , { 0 , 0 , 0 } },
    {   3 , 1 , { 0 , 0 , 0 } },
    {   6 , 2 , { 0 , 0 , 0 } },
    {  11 , 3 , { 0 , 0 , 0 } },
    {  21 , 4 , { 0 , 0 , 0 } },
    {  41 , 5 , { 0 , 0 , 0 } },
    {  81 , 6 , { 0 , 0 , 0 } },
    { 162 , 7 , { 0 , 0 , 0 } }
};

void dtvd_tuner_cn_com_set_status
(
    signed int next_state                               
)
{
    DTVD_DEBUG_MSG_ENTER( next_state, 0, 0 );

    tdtvd_tuner_cn.status = next_state;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

signed int dtvd_tuner_cn_com_read( void )
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;
    unsigned long length;
    unsigned char addr;
    unsigned char data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= D_DTVD_TUNER_REG_FEC_SUBA_MERCTRL;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= 0x04;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    tdtvd_tuner_cn.cn_old.ext = tdtvd_tuner_cn.cn_new.ext;
    tdtvd_tuner_cn.cn_old.up = tdtvd_tuner_cn.cn_new.up;
    tdtvd_tuner_cn.cn_old.low = tdtvd_tuner_cn.cn_new.low;

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= D_DTVD_TUNER_REG_FEC_SUBA_MERDT0;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_FEC_SUBD;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       &data  );        

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    tdtvd_tuner_cn.cn_new.ext = data;

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= D_DTVD_TUNER_REG_FEC_SUBA_MERDT1;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_FEC_SUBD;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       &data  );        

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    tdtvd_tuner_cn.cn_new.up = data;

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= D_DTVD_TUNER_REG_FEC_SUBA_MERDT2;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_FEC_SUBD;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       &data  );        

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    tdtvd_tuner_cn.cn_new.low = data;

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= D_DTVD_TUNER_REG_FEC_SUBA_MERCTRL;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= 0x00;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_cn_com_watch_timer_start( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_WATCH_CN,
                            tdtvd_tuner_cn.cycle_time );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

signed int dtvd_tuner_cn_com_start_set( void )
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= D_DTVD_TUNER_REG_FEC_SUBA_MERCTRL;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= 0x01;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= D_DTVD_TUNER_REG_FEC_SUBA_MERCTRL;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= 0x00;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;

}

void dtvd_tuner_cn_com_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_cn.wait_time = tdtvd_tuner_nonvola.cn_wait;

    tdtvd_tuner_cn.measure_time = (unsigned int)( tdtvd_tuner_nonvola.cn_cycle *
                                    D_DTVD_TUNER_COEFFICIENT_CN_CYCLE_TIME );

    tdtvd_tuner_cn.cycle_time = D_DTVD_TUNER_CN_CYCLE_TIME;

    tdtvd_tuner_cn.ave_count = tdtvd_tuner_nonvola.cn_ave_count;

    tdtvd_tuner_cn.alpha = tdtvd_tuner_nonvola.cn_alpha;

    tdtvd_tuner_cn.beta = tdtvd_tuner_nonvola.cn_beta;

    tdtvd_tuner_cn.gamma = tdtvd_tuner_nonvola.cn_gamma;

    tdtvd_tuner_cn.stabilize_times = 0;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_com_average( void )
{
    unsigned int   data;
    unsigned int   new_data;
    unsigned int   old_data;
    unsigned int   old_ave_data;
    unsigned int   ext;
    unsigned int   up;
    unsigned int   low;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    ext = tdtvd_tuner_cn.cn_new.ext;
    up = tdtvd_tuner_cn.cn_new.up;
    low = tdtvd_tuner_cn.cn_new.low;

    new_data = ( ( ( ext << 16 ) |                       
                 ( up << 8 ) ) |                         
                 ( low ) );                              

    ext = tdtvd_tuner_cn.cn_old.ext;
    up = tdtvd_tuner_cn.cn_old.up;
    low = tdtvd_tuner_cn.cn_old.low;

    old_data = ( ( ( ext << 16 ) |                       
                 ( up << 8 ) ) |                         
                 ( low ) );                              

    ext = tdtvd_tuner_cn.cn_old_ave.ext;
    up = tdtvd_tuner_cn.cn_old_ave.up;
    low = tdtvd_tuner_cn.cn_old_ave.low;

    old_ave_data = ( ( ( ext << 16 ) |                   
                     ( up << 8 ) ) |                     
                     ( low ) );                          

    data = ( ( tdtvd_tuner_cn.alpha * old_data ) +                   
             ( tdtvd_tuner_cn.beta * new_data ) +                    
             ( tdtvd_tuner_cn.gamma * old_ave_data ) ) /10;          

    tdtvd_tuner_cn.cn_old_ave.ext = ( unsigned char)( ( data & 0x00ff0000 ) >> 16 );

    tdtvd_tuner_cn.cn_old_ave.up = ( unsigned char)( ( data & 0x0000ff00 ) >> 8 );

    tdtvd_tuner_cn.cn_old_ave.low = ( unsigned char)( ( data & 0x000000ff ) );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_com_average_set( void )
{

    tdtvd_tuner_monitor.rx.cna.value_x.ext = tdtvd_tuner_cn.cn_old_ave.ext;
    tdtvd_tuner_monitor.rx.cna.value_comp.ext = tdtvd_tuner_cn.cn_old_ave.ext;

    tdtvd_tuner_monitor.rx.cna.value_x.up = tdtvd_tuner_cn.cn_old_ave.up;
    tdtvd_tuner_monitor.rx.cna.value_comp.up = tdtvd_tuner_cn.cn_old_ave.up;

    tdtvd_tuner_monitor.rx.cna.value_x.low = tdtvd_tuner_cn.cn_old_ave.low;
    tdtvd_tuner_monitor.rx.cna.value_comp.low = tdtvd_tuner_cn.cn_old_ave.low;

    tdtvd_tuner_monitor.rx.cna.value_x.result = D_DTVD_TUNER_GET_RESULT_OK;
    tdtvd_tuner_monitor.rx.cna.value_y.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;
    tdtvd_tuner_monitor.rx.cna.value_comp.result = D_DTVD_TUNER_GET_RESULT_OK;

    return;

}

signed int dtvd_tuner_cn_com_symcount_set
(
    unsigned char symcount  
)
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= D_DTVD_TUNER_REG_FEC_SUBD;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBD;
    i2c_data.data= symcount;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;

}

signed int dtvd_tuner_cn_com_symcount_read( void )
{
    signed int ret;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;
    unsigned long length;
    unsigned char addr;
    unsigned char data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    i2c_data.adr = D_DTVD_TUNER_REG_FEC_SUBA;
    i2c_data.data= D_DTVD_TUNER_REG_FEC_SUBA_MERSTEP;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_FEC_SUBD;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       &data  );        

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    tdtvd_tuner_cn.symcount = data;

    DTVD_DEBUG_MSG_EXIT();
    return D_DTVD_TUNER_OK;
}

void dtvd_tuner_cn_com_result_set( void )
{

    tdtvd_tuner_monitor.rx.cna.value_x.ext = tdtvd_tuner_cn.cn_new.ext;
    tdtvd_tuner_monitor.rx.cna.value_comp.ext = tdtvd_tuner_cn.cn_new.ext;

    tdtvd_tuner_monitor.rx.cna.value_x.up = tdtvd_tuner_cn.cn_new.up;
    tdtvd_tuner_monitor.rx.cna.value_comp.up = tdtvd_tuner_cn.cn_new.up;

    tdtvd_tuner_monitor.rx.cna.value_x.low = tdtvd_tuner_cn.cn_new.low;
    tdtvd_tuner_monitor.rx.cna.value_comp.low = tdtvd_tuner_cn.cn_new.low;

    tdtvd_tuner_monitor.rx.cna.value_x.result = D_DTVD_TUNER_GET_RESULT_OK;
    tdtvd_tuner_monitor.rx.cna.value_y.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;
    tdtvd_tuner_monitor.rx.cna.value_comp.result = D_DTVD_TUNER_GET_RESULT_OK;

    return;

}

void dtvd_tuner_cn_com_measure_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_cn.cn_old.ext = 0;
    tdtvd_tuner_cn.cn_old.up = 0;
    tdtvd_tuner_cn.cn_old.low = 0;

    tdtvd_tuner_cn.cn_old_ave.ext = 0;
    tdtvd_tuner_cn.cn_old_ave.up = 0;
    tdtvd_tuner_cn.cn_old_ave.low = 0;

    tdtvd_tuner_cn.cn_new.ext = 0;
    tdtvd_tuner_cn.cn_new.up = 0;
    tdtvd_tuner_cn.cn_new.low = 0;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_com_measure_monitor_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_monitor.rx.cna.state = D_DTVD_TUNER_MEASURE_STATE_OFF;

    tdtvd_tuner_monitor.rx.cna.value_x.ext = 0;
    tdtvd_tuner_monitor.rx.cna.value_comp.ext = 0;

    tdtvd_tuner_monitor.rx.cna.value_x.up = 0;
    tdtvd_tuner_monitor.rx.cna.value_comp.up = 0;

    tdtvd_tuner_monitor.rx.cna.value_x.low = 0;
    tdtvd_tuner_monitor.rx.cna.value_comp.low = 0;

    tdtvd_tuner_monitor.rx.cna.value_x.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;
    tdtvd_tuner_monitor.rx.cna.value_y.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;
    tdtvd_tuner_monitor.rx.cna.value_comp.result = D_DTVD_TUNER_GET_RESULT_NG_NONE;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_com_symcount_chg_timer
(
    unsigned char symcount  
)
{
    unsigned int count;
    unsigned int symcount_max;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    symcount_max = ( sizeof( c_symcount ) / sizeof( c_symcount[0]) );

    for( count = 0; count < symcount_max; count++ )
    {
        if( c_symcount[count].symcount == symcount )
        {

            tdtvd_tuner_cn.measure_time = c_symcount[count].timer;
            break;
        }
    }

    if ( count >= symcount_max )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_CN_COM,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_cn_com_measure_time_set( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_cn.measure_time = tdtvd_tuner_nonvola.cn_cycle;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

