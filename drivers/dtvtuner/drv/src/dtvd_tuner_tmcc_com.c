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
#include "dtvd_tuner_tmcc.h"

#include "dtvd_tuner_core.h"

void dtvd_tuner_tmcc_com_set_status
(
    signed int next_state                               
)
{

    tdtvd_tuner_tmcc.status = next_state;

    return;
}

signed int dtvd_tuner_tmcc_com_read
(
    void
)
{
    signed int                  ret;
    signed int                  cnt;
    unsigned char               addr[ D_DTVD_TUNER_TMCC_MEASURE_NUM ];
    unsigned char               data[ D_DTVD_TUNER_TMCC_MEASURE_NUM ];
    unsigned char               addr2;

    DTVD_TUNER_COM_I2C_DATA_t   i2c_data;
    DTVD_TUNER_TMCC_INFO_t      *info = &tdtvd_tuner_monitor.tmcc.info;

    ret = D_DTVD_TUNER_OK;

    data[ 0 ] = 0x00;

    addr[ 0 ]  = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC0;
    addr[ 1 ]  = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC1;
    addr[ 2 ]  = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC2;
    addr[ 3 ]  = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC3;
    addr[ 4 ]  = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC4;
    addr[ 5 ]  = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC5;
    addr[ 6 ]  = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC6;
    addr[ 7 ]  = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC7;
    addr[ 8 ]  = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC8;
    addr[ 9 ]  = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC9;
    addr[ 10 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC10;
    addr[ 11 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC11;
    addr[ 12 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC12;
    addr[ 13 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC13;
    addr[ 14 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC14;
    addr[ 15 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC15;
    addr[ 16 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC16;
    addr[ 17 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC17;
    addr[ 18 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC18;
    addr[ 19 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC19;
    addr[ 20 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC20;
    addr[ 21 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC21;
    addr[ 22 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC22;
    addr[ 23 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC23;
    addr[ 24 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC24;
    addr[ 25 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC25;
    addr[ 26 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC26;
    addr[ 27 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC27;
    addr[ 28 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC28;
    addr[ 29 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC29;
    addr[ 30 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC30;
    addr[ 31 ] = D_DTVD_TUNER_REG_TMCC_SUBA_TMCC31;

    i2c_data.adr  = D_DTVD_TUNER_REG_TMCC_SUBA;      
    addr2         = D_DTVD_TUNER_REG_TMCC_SUBD;      

    for( cnt = 0; cnt < D_DTVD_TUNER_TMCC_MEASURE_NUM; cnt++ )
    {

        i2c_data.data = addr[ cnt ];
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
        if( ret != D_DTVD_TUNER_OK )
        {
            return ret;
        }

        ret = dtvd_tuner_com_dev_i2c_read(
            D_DTVD_TUNER_REG_NO1, &addr2, &data[ cnt ] );
        if( ret != D_DTVD_TUNER_OK )
        {
            return ret;
        }
    }

    info->system = (unsigned char)( (unsigned char)( data[4] & 0x60 ) >> 5 );

    info->cntdwn = (unsigned char)( (unsigned char)( data[4] & 0x1E ) >> 1 );

    info->emgflg = (unsigned char)( data[4] & 0x01 );

    info->curr.part                 = (unsigned char)( (unsigned char)( data[5] & 0x10 ) >> 4 );

    info->curr.layer_a.modulation   = (unsigned char)( (unsigned char)( data[6] & 0x70 ) >> 4 );

    info->curr.layer_a.coderate     = (unsigned char)( (unsigned char)( data[7] & 0x70 ) >> 4 );

    info->curr.layer_a.interleave   = (unsigned char)( (unsigned char)( data[8] & 0x70 ) >> 4 );

    info->curr.layer_a.seg          = (unsigned char)( (unsigned char)( data[9] & 0xF0 ) >> 4 );

    info->curr.layer_b.modulation   = (unsigned char)( (unsigned char)( data[10] & 0x70 ) >> 4 );

    info->curr.layer_b.coderate     = (unsigned char)( (unsigned char)( data[11] & 0x70 ) >> 4 );

    info->curr.layer_b.interleave   = (unsigned char)( (unsigned char)( data[12] & 0x70 ) >> 4 );

    info->curr.layer_b.seg          = (unsigned char)( (unsigned char)( data[13] & 0xF0 ) >> 4 );

    info->next.part                 = (unsigned char)( data[5] & 0x01 );

    info->next.layer_a.modulation   = (unsigned char)( data[6] & 0x07 );

    info->next.layer_a.coderate     = (unsigned char)( data[7] & 0x07 );

    info->next.layer_a.interleave   = (unsigned char)( data[8] & 0x07 );

    info->next.layer_a.seg          = (unsigned char)( data[9] & 0x0F );

    info->next.layer_b.modulation   = (unsigned char)( data[10] & 0x07 );

    info->next.layer_b.coderate     = (unsigned char)( data[11] & 0x07 );

    info->next.layer_b.interleave   = (unsigned char)( data[12] & 0x07 );

    info->next.layer_b.seg          = (unsigned char)( data[13] & 0x0F );

    return ret;
}

unsigned char dtvd_tuner_tmcc_com_jdg_std( void )
{
    static const DTVD_TUNER_TMCCD_TRANS_PARAM_t standard[9] = {

        {
          D_DTVD_TUNER_TMCC_MODULATION_QPSK,    
          D_DTVD_TUNER_TMCC_CODERATE_1_2,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_1,       
          1                                     
        },

        {
          D_DTVD_TUNER_TMCC_MODULATION_QPSK,    
          D_DTVD_TUNER_TMCC_CODERATE_1_2,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_2,       
          1                                     
        },

        {
          D_DTVD_TUNER_TMCC_MODULATION_QPSK,    
          D_DTVD_TUNER_TMCC_CODERATE_1_2,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_3,       
          1                                     
        },

        {
          D_DTVD_TUNER_TMCC_MODULATION_QPSK,    
          D_DTVD_TUNER_TMCC_CODERATE_2_3,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_1,       
          1                                     
        },

        {
          D_DTVD_TUNER_TMCC_MODULATION_QPSK,    
          D_DTVD_TUNER_TMCC_CODERATE_2_3,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_2,       
          1                                     
        },

        {
          D_DTVD_TUNER_TMCC_MODULATION_QPSK,    
          D_DTVD_TUNER_TMCC_CODERATE_2_3,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_3,       
          1                                     
        },

        {
          D_DTVD_TUNER_TMCC_MODULATION_16QAM,   
          D_DTVD_TUNER_TMCC_CODERATE_1_2,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_1,       
          1                                     
        },

        {
          D_DTVD_TUNER_TMCC_MODULATION_16QAM,   
          D_DTVD_TUNER_TMCC_CODERATE_1_2,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_2,       
          1                                     
        },

        {
          D_DTVD_TUNER_TMCC_MODULATION_16QAM,   
          D_DTVD_TUNER_TMCC_CODERATE_1_2,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_3,       
          1                                     
        }
    };

    static const DTVD_TUNER_TMCCD_TRANS_PARAM_t omt_standard[3] = {

        {
          D_DTVD_TUNER_TMCC_MODULATION_QPSK,    
          D_DTVD_TUNER_TMCC_CODERATE_1_2,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_0,       
          1                                     
        },

        {
          D_DTVD_TUNER_TMCC_MODULATION_QPSK,    
          D_DTVD_TUNER_TMCC_CODERATE_2_3,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_0,       
          1                                     
        },

        {
          D_DTVD_TUNER_TMCC_MODULATION_16QAM,   
          D_DTVD_TUNER_TMCC_CODERATE_1_2,       
          D_DTVD_TUNER_TMCC_INTERLEAVE_0,       
          1                                     
        }
    };

    signed int cnt;
    signed int ret;

    for( cnt=0; cnt<9; cnt++ )
    {
        ret = memcmp( &standard[cnt],
                      &tdtvd_tuner_monitor.tmcc.info.curr.layer_a,
                      sizeof( DTVD_TUNER_TMCCD_TRANS_PARAM_t ) );
        if( ret == 0 )
        {

            tdtvd_tuner_monitor.tmcc.nonstd = D_DTVD_TUNER_TMCC_INFOGET_STANDARD;

            return D_DTVD_TUNER_TRUE;
        }
    }

    if ( tdtvd_tuner_core.omt_flag == D_DTVD_TUNER_ON )
    {

        for( cnt=0; cnt<3; cnt++ )

        {
            ret = memcmp( &omt_standard[cnt],
                          &tdtvd_tuner_monitor.tmcc.info.curr.layer_a,
                          sizeof( DTVD_TUNER_TMCCD_TRANS_PARAM_t ) );
            if( ret == 0 )
            {

                tdtvd_tuner_monitor.tmcc.nonstd = D_DTVD_TUNER_TMCC_INFOGET_STANDARD;

                return D_DTVD_TUNER_TRUE;
            }
        }
    }

    tdtvd_tuner_monitor.tmcc.nonstd = D_DTVD_TUNER_TMCC_INFOGET_NONSTD;

    return D_DTVD_TUNER_FALSE;
}

signed int dtvd_tuner_tmcc_com_read_valid_correct
(
    signed int *result       
)
{
    signed int                  ret;
    unsigned char               addr;
    unsigned char               data;
    DTVD_TUNER_COM_I2C_DATA_t   i2c_data;

    ret = D_DTVD_TUNER_OK;

    if( result == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TMCC_COM,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_TMCC_SUBA;          
    i2c_data.data= D_DTVD_TUNER_REG_TMCC_SUBA_FEC_IN;   
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    addr = D_DTVD_TUNER_REG_TMCC_SUBD;                  
    ret = dtvd_tuner_com_dev_i2c_read( D_DTVD_TUNER_REG_NO1, &addr, &data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    if( ( (unsigned char)( data & 0x01 ) == 1 ) &&              
        ( ( ( (unsigned char)( data & 0x02 ) ) >> 1  ) == 1 ) ) 
    {
        *result = D_DTVD_TUNER_OK;
    }
    else
    {
        *result = D_DTVD_TUNER_NG;
    }

    return ret;
}

signed int dtvd_tuner_tmcc_com_set_tmcclock
(
    unsigned char lock     
)
{
    signed int                  ret;
    unsigned char               addr;
    unsigned char               data;
    DTVD_TUNER_COM_I2C_DATA_t   i2c_data;

    ret = D_DTVD_TUNER_OK;

    i2c_data.adr    = D_DTVD_TUNER_REG_TMCC_SUBA;           
    i2c_data.data   = D_DTVD_TUNER_REG_TMCC_SUBA_TMCCLOCK;  
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    addr = D_DTVD_TUNER_REG_TMCC_SUBD;                      
    ret = dtvd_tuner_com_dev_i2c_read( D_DTVD_TUNER_REG_NO1, &addr, &data );
    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    if( lock == D_DTVD_TUNER_ON )
    {

        i2c_data.data= (unsigned char)( data | 0x01 );
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    }
    else
    {

        i2c_data.data= (unsigned char)( data & (unsigned char)( ~0x01 ) );
        ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    }

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return ret;
}

signed int dtvd_tuner_tmcc_com_get_tmcc
(
    signed int *result      
)
{
    signed int      ret;

    if( result == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_TMCC_COM,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    ret = dtvd_tuner_tmcc_com_read_valid_correct( result );
    if( ret != D_DTVD_TUNER_OK )
    {
        return ret;
    }

    if( *result != D_DTVD_TUNER_OK )
    {
        return ret;
    }

    ret = dtvd_tuner_tmcc_com_set_tmcclock( D_DTVD_TUNER_ON );
    if( ret != D_DTVD_TUNER_OK )
    {
        return ret;
    }

    ret = dtvd_tuner_tmcc_com_read();

    ret += dtvd_tuner_tmcc_com_set_tmcclock( D_DTVD_TUNER_OFF );

    if( ret != D_DTVD_TUNER_OK )
    {
        return D_DTVD_TUNER_NG;
    }

    return ret;
}

void dtvd_tuner_tmcc_com_init( void )
{

    tdtvd_tuner_tmcc.wait_time  = tdtvd_tuner_nonvola.wait_tmcc;

    tdtvd_tuner_tmcc.cycle_time = D_DTVD_TUNER_TIMER_TMCC_CYCLE_MSEC;

    return;
}

void dtvd_tuner_tmcc_com_watch_timer_start( void )
{

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_WATCH_TMCC,
                            tdtvd_tuner_tmcc.cycle_time );
    return;
}

void dtvd_tuner_tmcc_com_get_timer_start( void )
{

    dtvd_tuner_timer_start( D_DTVD_TUNER_TIMER_TMCC,
                            tdtvd_tuner_tmcc.wait_time );
    return;
}

