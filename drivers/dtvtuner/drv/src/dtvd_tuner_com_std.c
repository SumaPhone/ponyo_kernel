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

void _dtvd_tuner_memset
(
    void* dest,                         
    unsigned int val,                   
    unsigned int count                  
)
{
    if( dest == D_DTVD_TUNER_NULL )
    {
        return;
    }

    memset( dest, (signed int)val, (size_t)count );
    return;
}

void _dtvd_tuner_memcpy
(
    void* dest,                         
    const void* src,                    
    unsigned int count                  
)
{
    if( dest == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_STD,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    if( src == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_STD,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    memcpy( dest, src, (size_t)count );
    return;
}

void _dtvd_tuner_memmove
(
    void* dest,                         
    const void* src,                    
    unsigned int count                  
)
{
    if( dest == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_STD,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    if( src == D_DTVD_TUNER_NULL )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_STD,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    memmove( dest, src, (size_t)count );
    return;
}

#ifndef _D_DTVD_TUNER_NO_BUFFOVER_CHECK

void dtvd_tuner_memset
(
    void* dest,                         
    unsigned int val,                   
    unsigned int count,                 
    unsigned int destsize               
)
{
    if( dest == D_DTVD_TUNER_NULL )
    {
        return;
    }

    if( count > destsize )
    {
    }
    else
    {
        _dtvd_tuner_memset( dest, val, count );
    }
    return;
}
#endif 

#ifndef _D_DTVD_TUNER_NO_BUFFOVER_CHECK

void dtvd_tuner_memcpy
(
    void* dest,                         
    const void* src,                    
    unsigned int count,                 
    unsigned int destsize               
)
{

    if( (dest == D_DTVD_TUNER_NULL) || (src == D_DTVD_TUNER_NULL) )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_STD,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    if( count > destsize )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_STD,
                                  count, destsize, 0, 0, 0, 0 );
    }
    else
    {
        _dtvd_tuner_memcpy( dest, src, count );
    }
    return;
}
#endif 

#ifndef _D_DTVD_TUNER_NO_BUFFOVER_CHECK

void dtvd_tuner_memmove
(
    void* dest,                         
    const void* src,                    
    unsigned int count,                 
    unsigned int destsize               
)
{

    if( (dest == D_DTVD_TUNER_NULL) || (src == D_DTVD_TUNER_NULL) )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_STD,
                                  0, 0, 0, 0, 0, 0 );
        return;
    }

    if( count > destsize )
    {

        DTVD_TUNER_SYSERR_RANK_A( D_DTVD_TUNER_SYSERR_DRV_PARAM,
                                  DTVD_TUNER_COM_STD,
                                  count, destsize, 0, 0, 0, 0 );
    }
    else
    {

        if( count > 0 )
        {
            _dtvd_tuner_memmove( dest, src, count );
        }
    }
    return;
}
#endif 

