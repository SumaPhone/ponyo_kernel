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

#ifndef _DTVD_TUNER_HW_H_
#define _DTVD_TUNER_HW_H_

//#include <linux/spi/spi-subpmic.h>

#define D_DTVD_TUNER_DEV_TUNER      0x11                
#define D_DTVD_TUNER_DEV_I2C            0                   
#define D_DTVD_TUNER_MCBSP          OMAP_MCBSP5

#define D_DTVD_TUNER_DEV_GPIO       DCOM_DEV_PORT_CAM   

#define D_DTVD_TUNER_INT_PORT       113              
#define D_DTVD_TUNER_INT_MASK_CLEAR (0)             
#define D_DTVD_TUNER_INT_MASK_SET   (1)             
#define D_DTVD_TUNER_PORT_NTV_RST   39              
#define D_DTVD_TUNER_PORT_DTV_INT   INT_34XX_GPIO_BANK1

#define D_DTVD_TUNER_INFO_ID        D_HCM_D_DTV_TUNERS

#define D_DTVD_TUNER_INFO_NO        0               

#define D_DTVD_TUNER_REG_VERSION                    0x00 
#define D_DTVD_TUNER_REG_AGCCTRL                    0x01 
#define D_DTVD_TUNER_REG_AGCLEVEL                   0x03 
#define D_DTVD_TUNER_REG_AGC_SUBA                   0x04 
#define D_DTVD_TUNER_REG_AGC_SUBD                   0x05 
#define D_DTVD_TUNER_REG_AGC_SUBA_IFA_HI            0x00 
#define D_DTVD_TUNER_REG_AGC_SUBA_IFA_LO            0x01 
#define D_DTVD_TUNER_REG_AGC_SUBA_IFB_HI            0x02 
#define D_DTVD_TUNER_REG_AGC_SUBA_IFB_LO            0x03 
#define D_DTVD_TUNER_REG_AGC_SUBA_RFA_HI            0x04 
#define D_DTVD_TUNER_REG_AGC_SUBA_RFA_LO            0x05 
#define D_DTVD_TUNER_REG_AGC_SUBA_RFB_HI            0x06 
#define D_DTVD_TUNER_REG_AGC_SUBA_RFB_LO            0x07 
#define D_DTVD_TUNER_REG_AGC_SUBA_DTS               0x08 
#define D_DTVD_TUNER_REG_AGC_SUBA_MINIFAGC          0x09 
#define D_DTVD_TUNER_REG_AGC_SUBA_MAXIFAGC          0x0A 
#define D_DTVD_TUNER_REG_AGC_SUBA_AGAIN             0x0B 
#define D_DTVD_TUNER_REG_AGC_SUBA_IFREF_HI          0x0E 
#define D_DTVD_TUNER_REG_AGC_SUBA_IFREF_LO          0x0F 
#define D_DTVD_TUNER_REG_AGC_SUBA_MINRFAGC          0x12 
#define D_DTVD_TUNER_REG_AGC_SUBA_MAXRFAGC          0x13 
#define D_DTVD_TUNER_REG_AGC_SUBA_BGAIN             0x14 
#define D_DTVD_TUNER_REG_AGC_SUBA_RFREF_HI          0x15 
#define D_DTVD_TUNER_REG_AGC_SUBA_RFREF_LO          0x16 
#define D_DTVD_TUNER_REG_AGC_SUBA_IFSAMPLE          0x29 
#define D_DTVD_TUNER_REG_AGC_SUBA_POWAGC            0x2D 
#define D_DTVD_TUNER_REG_AGC_SUBA_IFAGCDAC          0x3A 
#define D_DTVD_TUNER_REG_AGC_SUBA_RFAGCDAC          0x3B 
#define D_DTVD_TUNER_REG_MODE_CTRL                  0x06 
#define D_DTVD_TUNER_REG_MODE_STAT                  0x07 
#define D_DTVD_TUNER_REG_STATE_INIT                 0x08 
#define D_DTVD_TUNER_REG_IQINV                      0x09 
#define D_DTVD_TUNER_REG_SYNC_STATE                 0x0A 
#define D_DTVD_TUNER_REG_AFC_M2A                    0x17 
#define D_DTVD_TUNER_REG_AFC_M2B                    0x18 
#define D_DTVD_TUNER_REG_AFC_M3A                    0x1A 
#define D_DTVD_TUNER_REG_AFC_M3B                    0x1B 
#define D_DTVD_TUNER_REG_FILTER                     0x1C 
#define D_DTVD_TUNER_REG_AFC_CLKA                   0x20 
#define D_DTVD_TUNER_REG_AFC_CLKB                   0x21 
#define D_DTVD_TUNER_REG_AFC_CLK2A                  0x26 
#define D_DTVD_TUNER_REG_AFC_CLK2B                  0x27 
#define D_DTVD_TUNER_REG_SYNC_SUBA                  0x28 
#define D_DTVD_TUNER_REG_SYNC_SUBD1                 0x29 
#define D_DTVD_TUNER_REG_SYNC_SUBD2                 0x2A 
#define D_DTVD_TUNER_REG_SYNC_SUBD3                 0x2B 
#define D_DTVD_TUNER_REG_SYNC_SUBA_IFGAIN           0x00 
#define D_DTVD_TUNER_REG_SYNC_SUBA_CLKOFF           0x2A 
#define D_DTVD_TUNER_REG_SYNC_SUBA_CROFF            0x2D 
#define D_DTVD_TUNER_REG_SYNC_SUBA_MODE_M2G4        0x43 
#define D_DTVD_TUNER_REG_SYNC_SUBA_MODE_M2G8        0x44 
#define D_DTVD_TUNER_REG_SYNC_SUBA_MODE_M3G4        0x47 
#define D_DTVD_TUNER_REG_SYNC_SUBA_MODE_M3G8        0x48 
#define D_DTVD_TUNER_REG_SYNC_SUBA_MODE_M3G16       0x49 
#define D_DTVD_TUNER_REG_SYNC_SUBA_MODE_THRES       0x4B 
#define D_DTVD_TUNER_REG_SYNC_SUBA_MODE_TIMER       0x4C 
#define D_DTVD_TUNER_REG_SYNC_SUBA_S4_TIMER         0x54 
#define D_DTVD_TUNER_REG_SYNC_SUBA_S1_TULD          0x6B 
#define D_DTVD_TUNER_REG_SYNC_SUBA_S1_TIMER         0x6C 
#define D_DTVD_TUNER_REG_SYNC_SUBA_S3_TIMER         0x6D 
#define D_DTVD_TUNER_REG_WIN_CTRL                   0x3A 
#define D_DTVD_TUNER_REG_WIN_SUBA                   0x3B 
#define D_DTVD_TUNER_REG_WIN_SUBD                   0x3C 
#define D_DTVD_TUNER_REG_WIN_SUBA_WIN_PEAKTH1       0x05 
#define D_DTVD_TUNER_REG_WIN_SUBA_WIN_PEAKTH2       0x06 
#define D_DTVD_TUNER_REG_WIN_SUBA_WIN_TIMER         0x07 
#define D_DTVD_TUNER_REG_DETECT_SUBA                0x40 
#define D_DTVD_TUNER_REG_DETECT_SUBD                0x41 
#define D_DTVD_TUNER_REG_DETECT_SUBA_SEARCH_TIMER   0x0D 
#define D_DTVD_TUNER_REG_DETECT_SUBA_SEARCH_THRES   0x35 
#define D_DTVD_TUNER_REG_DETECT_SUBA_LOCK_THRES     0x58 
#define D_DTVD_TUNER_REG_DETECT_SUBA_LOSS_THRES     0x59 
#define D_DTVD_TUNER_REG_DETECT_SUBA_LOCK_FRM       0x5A 
#define D_DTVD_TUNER_REG_DETECT_SUBA_LOSS_FRM       0x5B 
#define D_DTVD_TUNER_REG_SEGCNT                     0x44 
#define D_DTVD_TUNER_REG_CNCNT                      0x45 
#define D_DTVD_TUNER_REG_CNDAT0                     0x46 
#define D_DTVD_TUNER_REG_CNDAT1                     0x47 
#define D_DTVD_TUNER_REG_CNMODE                     0x48 
#define D_DTVD_TUNER_REG_FEC_SUBA                   0x50 
#define D_DTVD_TUNER_REG_FEC_SUBD                   0x51 
#define D_DTVD_TUNER_REG_FEC_SUBA_MERCTRL           0x50 
#define D_DTVD_TUNER_REG_FEC_SUBA_MERSTEP           0x51 
#define D_DTVD_TUNER_REG_FEC_SUBA_MERDT0            0x52 
#define D_DTVD_TUNER_REG_FEC_SUBA_MERDT1            0x53 
#define D_DTVD_TUNER_REG_FEC_SUBA_MERDT2            0x54 
#define D_DTVD_TUNER_REG_FEC_SUBA_MEREND            0x58 
#define D_DTVD_TUNER_REG_FEC_SUBA_VBERSET0          0xA4 
#define D_DTVD_TUNER_REG_FEC_SUBA_VBERSET1          0xA5 
#define D_DTVD_TUNER_REG_FEC_SUBA_VBERSET2          0xA6 
#define D_DTVD_TUNER_REG_FEC_SUBA_PEREN             0xB0 
#define D_DTVD_TUNER_REG_FEC_SUBA_PERRST            0xB1 
#define D_DTVD_TUNER_REG_FEC_SUBA_PERSNUM0          0xB2 
#define D_DTVD_TUNER_REG_FEC_SUBA_PERSNUM1          0xB3 
#define D_DTVD_TUNER_REG_FEC_SUBA_PERFLG            0xB6 
#define D_DTVD_TUNER_REG_FEC_SUBA_PERERR0           0xB7 
#define D_DTVD_TUNER_REG_FEC_SUBA_PERERR1           0xB8 
#define D_DTVD_TUNER_REG_FEC_SUBA_PCLKSEL           0xD2 
#define D_DTVD_TUNER_REG_FEC_SUBA_TSOUT             0xD5 
#define D_DTVD_TUNER_REG_FEC_SUBA_PBER              0xD6 
#define D_DTVD_TUNER_REG_FEC_SUBA_SBER              0xD7 
#define D_DTVD_TUNER_REG_FEC_SUBA_SBER_IRQ_MASK     0xDA 
#define D_DTVD_TUNER_REG_FEC_SUBA_TSERR_IRQ_MASK    0xDB 
#define D_DTVD_TUNER_REG_FEC_SUBA_SBERSET0          0xDC 
#define D_DTVD_TUNER_REG_FEC_SUBA_SBERSET1          0xDD 
#define D_DTVD_TUNER_REG_VBERON                     0x52 
#define D_DTVD_TUNER_REG_VBERXRST                   0x53 
#define D_DTVD_TUNER_REG_VBERFLG                    0x54 
#define D_DTVD_TUNER_REG_VBERDT0                    0x55 
#define D_DTVD_TUNER_REG_VBERDT1                    0x56 
#define D_DTVD_TUNER_REG_VBERDT2                    0x57 
#define D_DTVD_TUNER_REG_RSBERON                    0x5E 
#define D_DTVD_TUNER_REG_RSBERXRST                  0x5F 
#define D_DTVD_TUNER_REG_RSBERCEFLG                 0x60 
#define D_DTVD_TUNER_REG_TSERR_IRQ_RST              0x62 
#define D_DTVD_TUNER_REG_RSBERDT0                   0x64 
#define D_DTVD_TUNER_REG_RSBERDT1                   0x65 
#define D_DTVD_TUNER_REG_RSBERDT2                   0x66 
#define D_DTVD_TUNER_REG_TMCC_IRQ                   0x6C 
#define D_DTVD_TUNER_REG_TMCC_SUBA                  0x6D 
#define D_DTVD_TUNER_REG_TMCC_SUBD                  0x6E 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCCLOCK         0x14 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC_IRQ_MASK    0x35 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC_IRQ_RST     0x36 
#define D_DTVD_TUNER_REG_TMCC_SUBA_EMG_INV          0x38 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC0            0x80 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC1            0x81 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC2            0x82 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC3            0x83 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC4            0x84 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC5            0x85 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC6            0x86 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC7            0x87 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC8            0x88 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC9            0x89 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC10           0x8A 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC11           0x8B 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC12           0x8C 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC13           0x8D 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC14           0x8E 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC15           0x8F 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC16           0x90 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC17           0x91 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC18           0x92 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC19           0x93 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC20           0x94 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC21           0x95 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC22           0x96 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC23           0x97 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC24           0x98 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC25           0x99 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC26           0x9A 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC27           0x9B 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC28           0x9C 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC29           0x9D 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC30           0x9E 
#define D_DTVD_TUNER_REG_TMCC_SUBA_TMCC31           0x9F 
#define D_DTVD_TUNER_REG_TMCC_SUBA_FEC_IN           0xA0 
#define D_DTVD_TUNER_REG_SBER_IRQ                   0x6F 
#define D_DTVD_TUNER_REG_RESET                      0x70 
#define D_DTVD_TUNER_REG_STANDBY                    0x73 
#define D_DTVD_TUNER_REG_ANALOG_PWDN                0xD0 
#define D_DTVD_TUNER_REG_TUNERSEL                   0xD3 
#define D_DTVD_TUNER_REG_DACCTRL                    0xD4 

#define D_DTVD_TUNER_REG_SEARCH_CTRL                0xE6 
#define D_DTVD_TUNER_REG_SEARCH_CHANNEL             0xE7 
#define D_DTVD_TUNER_REG_SEARCH_SUBA                0xE8 
#define D_DTVD_TUNER_REG_SEARCH_SUBD                0xE9 
#define D_DTVD_TUNER_REG_SEARCH_SUBA_CHANNEL1       0x01 
#define D_DTVD_TUNER_REG_SEARCH_SUBA_CHANNEL2       0x02 
#define D_DTVD_TUNER_REG_SEARCH_SUBA_CHANNEL3       0x03 
#define D_DTVD_TUNER_REG_SEARCH_SUBA_CHANNEL4       0x04 
#define D_DTVD_TUNER_REG_SEARCH_SUBA_CHANNEL5       0x05 
#define D_DTVD_TUNER_REG_SEARCH_SUBA_CHANNEL6       0x06 
#define D_DTVD_TUNER_REG_SEARCH_SUBA_CHANNEL7       0x07 
#define D_DTVD_TUNER_REG_AOUT                       0xEA 
#define D_DTVD_TUNER_REG_GPIO_DAT                   0xEB 
#define D_DTVD_TUNER_REG_GPIO_OUTSEL                0xEC 
#define D_DTVD_TUNER_REG_SEARCH_IRQ                 0xED 
#define D_DTVD_TUNER_REG_SEARCH_IRQCTL              0xEE 
#define D_DTVD_TUNER_REG_SEARCH_MODE                0xF0 
#define D_DTVD_TUNER_REG_SEARCH_MDCNT               0xF1 
#define D_DTVD_TUNER_REG_SEARCH_CNT                 0xF2 
#define D_DTVD_TUNER_REG_SEARCH_START               0xF3 
#define D_DTVD_TUNER_REG_SEARCH_END                 0xF4 
#define D_DTVD_TUNER_REG_STB_ENABLE                 0xF8 
#define D_DTVD_TUNER_REG_STB_MODE                   0xF9 
#define D_DTVD_TUNER_REG_STB_WAIT                   0xFA 
#define D_DTVD_TUNER_REG_STB_TUWAIT                 0xFB 
#define D_DTVD_TUNER_REG_STB_EMGWAIT                0xFC 
#define D_DTVD_TUNER_REG_TUTHREW                    0xFE 

#define D_DTVD_TUNER_RF_REG_MC_RG1          0x09 

#define D_DTVD_TUNER_RF_REG_SYNTH_RG5       0x33 
#define D_DTVD_TUNER_RF_REG_SYNTH_RG6       0x34 
#define D_DTVD_TUNER_RF_REG_SYNTH_RG7       0x35 
#define D_DTVD_TUNER_RF_REG_SYNTH_RG8       0x36 

#define D_DTVD_TUNER_RF_DAT_SEQ_RESET       0xF0 
#define D_DTVD_TUNER_RF_DAT_SEQ_START       0xF4 

#define D_DTVD_TUNER_COEFFICIENT_STATE_TIME     50
#define D_DTVD_TUNER_COEFFICIENT_SEARCH_TIME    100
#define D_DTVD_TUNER_COEFFICIENT_SYNC_TIME      1000    

#define D_DTVD_TUNER_COEFFICIENT_AGC_CYCLE_TIME 100     
#define D_DTVD_TUNER_COEFFICIENT_CN_CYCLE_TIME  100     
#define D_DTVD_TUNER_COEFFICIENT_BER_CYCLE_TIME 100     
#define D_DTVD_TUNER_COEFFICIENT_REFCLK_TIME    10      

#define D_DTVD_TUNER_COEFFICIENT_LOCKWAIT_TIME  10      

#define D_DTVD_TUNER_PCOM_OPEN_RETRY_SLEEP_TIME     100     

#define D_DTVD_TUNER_PCOM_OPEN_RETRY_MAX             40     

#define D_DTVD_TUNER_DATALEN_MAX                    256

#define D_DTVD_TUNER_RF_DATALEN_MAX                  32         
#define D_DTVD_TUNER_NO00                          0x00         
#define D_DTVD_TUNER_NO01                          0x01         
#define D_DTVD_TUNER_SET_NO1                          1         
#if 0
#define D_DTVD_TUNER_PSCNT_LDO6          SUBPMIC_LDOON1
#define D_DTVD_TUNER_PSCNT_LDO7          SUBPMIC_LDOON1
#define D_DTVD_TUNER_PSCNT_LDO14         SUBPMIC_LDOON2
#define D_DTVD_TUNER_PSCNT_DCDC1         SUBPMIC_DCDCON

#define D_DTVD_TUNER_PSCNT_LDO6_ON       SUBPMIC_LDOON1_LDO6ON
#define D_DTVD_TUNER_PSCNT_LDO7_ON       SUBPMIC_LDOON1_LDO7ON
#define D_DTVD_TUNER_PSCNT_LDO14_ON      0x20
#define D_DTVD_TUNER_PSCNT_DCDC1_ON      SUBPMIC_DCDCON_DD1ON

#define D_DTVD_TUNER_PSCNT_LDO6_OFF      ~D_DTVD_TUNER_PSCNT_LDO6_ON
#define D_DTVD_TUNER_PSCNT_LDO7_OFF      ~D_DTVD_TUNER_PSCNT_LDO7_ON
#define D_DTVD_TUNER_PSCNT_LDO14_OFF     ~D_DTVD_TUNER_PSCNT_LDO14_ON
#define D_DTVD_TUNER_PSCNT_DCDC1_OFF     ~D_DTVD_TUNER_PSCNT_DCDC1_ON
#endif

#define D_DTVD_TUNER_PSCNT_LDO6          0
#define D_DTVD_TUNER_PSCNT_LDO7          1
#define D_DTVD_TUNER_PSCNT_LDO14         2
#define D_DTVD_TUNER_PSCNT_DCDC1         3

#define D_DTVD_TUNER_PSCNT_LDO6_ON       1
#define D_DTVD_TUNER_PSCNT_LDO7_ON       1
#define D_DTVD_TUNER_PSCNT_LDO14_ON      0x20
#define D_DTVD_TUNER_PSCNT_DCDC1_ON      1

#define D_DTVD_TUNER_PSCNT_LDO6_OFF      1
#define D_DTVD_TUNER_PSCNT_LDO7_OFF      1
#define D_DTVD_TUNER_PSCNT_LDO14_OFF     1
#define D_DTVD_TUNER_PSCNT_DCDC1_OFF     1

#endif 

