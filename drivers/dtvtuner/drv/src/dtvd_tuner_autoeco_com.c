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
#include "dtvd_tuner_autoeco.h"

void dtvd_tuner_autoeco_com_set_status
(
    signed int next_state                                      
)
{
    DTVD_DEBUG_MSG_ENTER( next_state, 0, 0 );

    tdtvd_tuner_autoeco.status = next_state;

    DTVD_DEBUG_MSG_EXIT();
    return;
}

void dtvd_tuner_autoeco_com_init( void )
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    tdtvd_tuner_autoeco.qpsk12.agc_n2e =
        tdtvd_tuner_nonvola.auto_n2e_ifagc_qp12;  
    tdtvd_tuner_autoeco.qpsk12.agc_e2n =
        tdtvd_tuner_nonvola.auto_e2n_ifagc_qp12;  

    tdtvd_tuner_autoeco.qpsk23.agc_n2e =
        tdtvd_tuner_nonvola.auto_n2e_ifagc_qp23;  
    tdtvd_tuner_autoeco.qpsk23.agc_e2n =
        tdtvd_tuner_nonvola.auto_e2n_ifagc_qp23;  

    tdtvd_tuner_autoeco.qam12.agc_n2e =
        tdtvd_tuner_nonvola.auto_n2e_ifagc_qam12; 
    tdtvd_tuner_autoeco.qam12.agc_e2n =
        tdtvd_tuner_nonvola.auto_e2n_ifagc_qam12; 

    tdtvd_tuner_autoeco.qpsk12.cn_n2e =
        tdtvd_tuner_nonvola.auto_n2e_cn_qp12;     
    tdtvd_tuner_autoeco.qpsk12.cn_e2n =
        tdtvd_tuner_nonvola.auto_e2n_cn_qp12;     

    tdtvd_tuner_autoeco.qpsk23.cn_n2e =
        tdtvd_tuner_nonvola.auto_n2e_cn_qp23;     
    tdtvd_tuner_autoeco.qpsk23.cn_e2n =
        tdtvd_tuner_nonvola.auto_e2n_cn_qp23;     

    tdtvd_tuner_autoeco.qam12.cn_n2e =
        tdtvd_tuner_nonvola.auto_n2e_cn_qam12;    
    tdtvd_tuner_autoeco.qam12.cn_e2n =
        tdtvd_tuner_nonvola.auto_e2n_cn_qam12;    

    DTVD_DEBUG_MSG_EXIT();
    return;
}

signed int dtvd_tuner_autoeco_com_agc_read( unsigned char *data )
{
    signed int ret;
    unsigned long length;
    unsigned char addr;
    DTVD_TUNER_COM_I2C_DATA_t i2c_data;

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( data == NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_AUTOECO_COM,
                                  0, 0, 0, 0, 0, 0 );
        return D_DTVD_TUNER_NG;
    }

    i2c_data.adr = D_DTVD_TUNER_REG_AGC_SUBA;
    i2c_data.data= 0x3A;
    ret = dtvd_tuner_com_dev_i2c_write( D_DTVD_TUNER_REG_NO1, &i2c_data );
    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return D_DTVD_TUNER_NG;
    }

    *data = 0x00;

    length = D_DTVD_TUNER_REG_NO1;

    addr = D_DTVD_TUNER_REG_AGC_SUBD;

    ret = dtvd_tuner_com_dev_i2c_read( length,          
                                       &addr,           
                                       data  );         

    if( ret != D_DTVD_TUNER_OK )
    {
        DTVD_DEBUG_MSG_EXIT();
        return ret;
    }

    DTVD_DEBUG_MSG_EXIT();
    return ret;
}

void dtvd_tuner_autoeco_com_eco_judge
(
    unsigned char agc_data,                                      
    unsigned int  cn_data,                                       
    unsigned int  *result                                        
)
{

    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    if( result == NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_AUTOECO_COM,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    *result = D_DTVD_TUNER_AUTOECO_NON;

    DTVD_DEBUG_MSG_EXIT();
    return ;
}

void dtvd_tuner_autoeco_com_thr_judge
(
    DTVD_TUNER_AUTOECO_THR_t  **thr                      
)
{
    DTVD_DEBUG_MSG_ENTER( 0, 0, 0 );

    DTVD_DEBUG_MSG_EXIT();
    return;
}

