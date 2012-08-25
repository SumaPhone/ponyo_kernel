/* Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#ifndef MT9D112_H
#define MT9D112_H

#include <linux/types.h>
#include <mach/camera.h>

extern struct mt9d112_reg mt9d112_regs;

enum mt9d112_width {
	WORD_LEN,
	BYTE_LEN,
	BYTE_POLL,
	OTPM_POLL
};

struct mt9d112_i2c_reg_conf {
	unsigned short waddr;
	unsigned short wdata;
	enum mt9d112_width width;
	unsigned short mdelay_time;
};

struct mt9p111_i2c_reg_burst_mode_conf{
	unsigned short waddr;
	unsigned short wdata1;
	unsigned short wdata2;
	unsigned short wdata3;
	unsigned short wdata4;
	unsigned short wdata5;
	unsigned short wdata6;
	unsigned short wdata7;
	unsigned short wdata8;
	enum mt9d112_width width;
	unsigned short mdelay_time;
};

struct mt9d112_reg {
   	const struct mt9d112_i2c_reg_conf *init_tbl;
	uint16_t init_tbl_size;
	
	const struct mt9d112_i2c_reg_conf *init_tbl1;
	uint16_t init_tbl1_size;
	const struct mt9d112_i2c_reg_conf *init_tbl2;
	uint16_t init_tbl2_size;
	const struct mt9p111_i2c_reg_burst_mode_conf *init_patchbust_mode_tbl;
	uint16_t init_patchbust_mode_tbl_size;

   	const struct mt9d112_i2c_reg_conf *preview_tbl;
	uint16_t preview_tbl_size;
   	const struct mt9d112_i2c_reg_conf *snapshot_tbl;
	uint16_t snapshot_tbl_size;
	//wb
   	const struct mt9d112_i2c_reg_conf *awb_tbl;
	uint16_t awb_tbl_size;
   	const struct mt9d112_i2c_reg_conf *MWB_Cloudy_tbl;
	uint16_t MWB_Cloudy_tbl_size;
   	const struct mt9d112_i2c_reg_conf *MWB_Day_light_tbl;
	uint16_t MWB_Day_light_tbl_size;
   	const struct mt9d112_i2c_reg_conf *MWB_FLUORESCENT_tbl;
	uint16_t MWB_FLUORESCENT_tbl_size;
   	const struct mt9d112_i2c_reg_conf *MWB_INCANDESCENT_tbl;
	uint16_t MWB_INCANDESCENT_tbl_size;
	//effect
   	const struct mt9d112_i2c_reg_conf *EFFECT_OFF_tbl;
	uint16_t EFFECT_OFF_tbl_size;
   	const struct mt9d112_i2c_reg_conf *EFFECT_MONO_tbl;
	uint16_t EFFECT_MONO_tbl_size;
   	const struct mt9d112_i2c_reg_conf *EFFECT_SEPIA_tbl;
	uint16_t EFFECT_SEPIA_tbl_size;
   	const struct mt9d112_i2c_reg_conf *EFFECT_NEGATIVE_tbl;
	uint16_t EFFECT_NEGATIVE_tbl_size;
   	const struct mt9d112_i2c_reg_conf *EFFECT_SOLARIZE_tbl;
	uint16_t EFFECT_SOLARIZE_tbl_size;
	//brightness
	const struct mt9d112_i2c_reg_conf *BRIGHTNESS_L3_tbl;
	uint16_t BRIGHTNESS_L3_tbl_size;
	const struct mt9d112_i2c_reg_conf *BRIGHTNESS_L2_tbl;
	uint16_t BRIGHTNESS_L2_tbl_size;
	const struct mt9d112_i2c_reg_conf *BRIGHTNESS_L1_tbl;
	uint16_t BRIGHTNESS_L1_tbl_size;
	const struct mt9d112_i2c_reg_conf *BRIGHTNESS_H0_tbl;
	uint16_t BRIGHTNESS_H0_tbl_size;
	const struct mt9d112_i2c_reg_conf *BRIGHTNESS_H1_tbl;
	uint16_t BRIGHTNESS_H1_tbl_size;
	const struct mt9d112_i2c_reg_conf *BRIGHTNESS_H2_tbl;
	uint16_t BRIGHTNESS_H2_tbl_size;
	const struct mt9d112_i2c_reg_conf *BRIGHTNESS_H3_tbl;
	uint16_t BRIGHTNESS_H3_tbl_size;

	//af
   	const struct mt9d112_i2c_reg_conf *VCM_Enable_full_scan_tbl;
	uint16_t VCM_Enable_full_scan_tbl_size;
	const struct mt9d112_i2c_reg_conf *Foucs_Marco_tbl;
	uint16_t Foucs_Marco_tbl_size;
	const struct mt9d112_i2c_reg_conf *Foucs_Infinity_tbl;
	uint16_t Foucs_Infinity_tbl_size;
   	const struct mt9d112_i2c_reg_conf *AF_Trigger_tbl;
	uint16_t AF_Trigger_tbl_size;
   	const struct mt9d112_i2c_reg_conf *VCM_Enable_Continue_scan_tbl;
	uint16_t VCM_Enable_Continue_scan_tbl_size;
	//scene
   	const struct mt9d112_i2c_reg_conf *SCENE_AUTO_tbl;
	uint16_t SCENE_AUTO_tbl_size;
   	const struct mt9d112_i2c_reg_conf *SCENE_NIGHT_tbl;
	uint16_t SCENE_NIGHT_tbl_size;

};

#endif /* MT9D112_H */
