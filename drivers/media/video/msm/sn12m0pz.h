
/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef SN12M0PZ_H
#define SN12M0PZ_H

#include <linux/types.h>

extern struct sn12m0pz_reg sn12m0pz_regs;

enum sn12m0pz_width {
	WORD_LEN,
	BYTE_LEN
};

struct sn12m0pz_i2c_reg_conf {
	unsigned short waddr;
	unsigned short wdata;
	enum sn12m0pz_width width;
	unsigned short mdelay_time;
};

struct sn12m0pz_reg {
   	const struct sn12m0pz_i2c_reg_conf *init_tbl;
	uint16_t init_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *preview_tbl;
	uint16_t preview_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *snapshot_tbl;
	uint16_t snapshot_tbl_size;
	//wb
   	const struct sn12m0pz_i2c_reg_conf *awb_tbl;
	uint16_t awb_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *MWB_Cloudy_tbl;
	uint16_t MWB_Cloudy_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *MWB_Day_light_tbl;
	uint16_t MWB_Day_light_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *MWB_FLUORESCENT_tbl;
	uint16_t MWB_FLUORESCENT_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *MWB_INCANDESCENT_tbl;
	uint16_t MWB_INCANDESCENT_tbl_size;
	//effect
   	const struct sn12m0pz_i2c_reg_conf *EFFECT_OFF_tbl;
	uint16_t EFFECT_OFF_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *EFFECT_MONO_tbl;
	uint16_t EFFECT_MONO_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *EFFECT_SEPIA_tbl;
	uint16_t EFFECT_SEPIA_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *EFFECT_NEGATIVE_tbl;
	uint16_t EFFECT_NEGATIVE_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *EFFECT_SOLARIZE_tbl;
	uint16_t EFFECT_SOLARIZE_tbl_size;
	//af
   	const struct sn12m0pz_i2c_reg_conf *VCM_Enable_full_scan_tbl;
	uint16_t VCM_Enable_full_scan_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *AF_Trigger_tbl;
	uint16_t AF_Trigger_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *VCM_Enable_Continue_scan_tbl;
	uint16_t VCM_Enable_Continue_scan_tbl_size;
	//scene
   	const struct sn12m0pz_i2c_reg_conf *SCENE_AUTO_tbl;
	uint16_t SCENE_AUTO_tbl_size;
   	const struct sn12m0pz_i2c_reg_conf *SCENE_NIGHT_tbl;
	uint16_t SCENE_NIGHT_tbl_size;

};

#endif /* MT9D112_H */

