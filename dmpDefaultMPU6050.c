/*
 * Copyright (C) 2012 Invensense, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include "inv_mpu_iio.h"
#include "dmpKey.h"
#include "dmpmap.h"

#define CFG_OUT_STEPDET         (2016)
#define CFG_OUT_3QUAT           (2046)
#define OUT_3QUAT_DAT           (2056)
#define CFG_OUT_6QUAT           (2081)
#define OUT_6QUAT_DAT           (2091)
#define CFG_OUT_PQUAT           (2116)
#define OUT_PQUAT_DAT           (2125)
#define CFG_OUT_ACCL            (2150)
#define OUT_ACCL_DAT            (2159)
#define CFG_OUT_GYRO            (2184)
#define OUT_GYRO_DAT            (2193)
#define CFG_OUT_CPASS           (2218)
#define OUT_CPASS_DAT           (2227)
#define CFG_OUT_PRESS           (2252)
#define OUT_PRESS_DAT           (2261)
#define CFG_PED_ENABLE          (2350)
#define CFG_PEDSTEP_DET         (2600)
#define CFG_DISPLAY_ORIENT_INT  (1754)
#define CFG_PED_INT             (2588)
#define CFG_FIFO_INT            (2348)
#define CFG_AUTH                (1051)
#define FCFG_1                  (1078)
#define FCFG_3                  (1103)
#define FCFG_2                  (1082)
#define FCFG_7                  (1089)
#define FCFG_6                  (1121)
#define CFG_EXT_GYRO_BIAS       (1200)
#define CFG_7                   (1419)
#define SMD_TP2                 (1382)
#define SMD_TP1                 (1359)
#define CFG_MOTION_BIAS         (1421)
#define CFG_GYRO_RAW_DATA       (2193)

#define D_0_22                  (22+512)
#define D_0_24                  (24+512)

#define D_0_36                  (36)
#define D_0_52                  (52)
#define D_0_96                  (96)
#define D_0_104                 (104)
#define D_0_108                 (108)
#define D_0_163                 (163)
#define D_0_188                 (188)
#define D_0_192                 (192)
#define D_0_224                 (224)
#define D_0_228                 (228)
#define D_0_232                 (232)
#define D_0_236                 (236)

#define D_1_2                   (256 + 2)
#define D_1_4                   (256 + 4)
#define D_1_8                   (256 + 8)
#define D_1_10                  (256 + 10)
#define D_1_24                  (256 + 24)
#define D_1_28                  (256 + 28)
#define D_1_36                  (256 + 36)
#define D_1_40                  (256 + 40)
#define D_1_44                  (256 + 44)
#define D_1_72                  (256 + 72)
#define D_1_74                  (256 + 74)
#define D_1_79                  (256 + 79)
#define D_1_88                  (256 + 88)
#define D_1_90                  (256 + 90)
#define D_1_92                  (256 + 92)
#define D_1_96                  (256 + 96)
#define D_1_98                  (256 + 98)
#define D_1_106                 (256 + 106)
#define D_1_108                 (256 + 108)
#define D_1_112                 (256 + 112)
#define D_1_128                 (256 + 144)
#define D_1_152                 (256 + 12)
#define D_1_160                 (256 + 160)
#define D_1_176                 (256 + 176)
#define D_1_178                 (256 + 178)
#define D_1_218                 (256 + 218)
#define D_1_232                 (256 + 232)
#define D_1_236                 (256 + 236)
#define D_1_240                 (256 + 240)
#define D_1_244                 (256 + 244)
#define D_1_250                 (256 + 250)
#define D_1_252                 (256 + 252)
#define D_2_12                  (512 + 12)
#define D_2_96                  (512 + 96)
#define D_2_108                 (512 + 108)
#define D_2_208                 (512 + 208)
#define D_2_224                 (512 + 224)
#define D_2_236                 (512 + 236)
#define D_2_244                 (512 + 244)
#define D_2_248                 (512 + 248)
#define D_2_252                 (512 + 252)

#define D_EXT_GYRO_BIAS_X       (61 * 16)
#define D_EXT_GYRO_BIAS_Y       (61 * 16 + 4)
#define D_EXT_GYRO_BIAS_Z       (61 * 16 + 8)
#define D_ACT0                  (40 * 16)
#define D_ACSX                  (40 * 16 + 4)
#define D_ACSY                  (40 * 16 + 8)
#define D_ACSZ                  (40 * 16 + 12)

#define FLICK_MSG               (45 * 16 + 4)
#define FLICK_COUNTER           (45 * 16 + 8)
#define FLICK_LOWER             (45 * 16 + 12)
#define FLICK_UPPER             (46 * 16 + 12)

#define D_SMD_ENABLE            (18 * 16)
#define D_SMD_MOT_THLD          (20 * 16)
#define D_SMD_DELAY_THLD        (21 * 16 + 4)
#define D_SMD_DELAY2_THLD       (21 * 16 + 12)
#define D_SMD_EXE_STATE         (22 * 16)
#define D_SMD_DELAY_CNTR        (21 * 16)

#define D_AUTH_OUT              (992)
#define D_AUTH_IN               (996)
#define D_AUTH_A                (1000)
#define D_AUTH_B                (1004)

#define D_PEDSTD_BP_B           (768 + 0x1C)
#define D_PEDSTD_HP_A           (768 + 0x78)
#define D_PEDSTD_HP_B           (768 + 0x7C)
#define D_PEDSTD_BP_A4          (768 + 0x40)
#define D_PEDSTD_BP_A3          (768 + 0x44)
#define D_PEDSTD_BP_A2          (768 + 0x48)
#define D_PEDSTD_BP_A1          (768 + 0x4C)
#define D_PEDSTD_INT_THRSH      (768 + 0x68)
#define D_PEDSTD_CLIP           (768 + 0x6C)
#define D_PEDSTD_SB             (768 + 0x28)
#define D_PEDSTD_SB_TIME        (768 + 0x2C)
#define D_PEDSTD_PEAKTHRSH      (768 + 0x98)
#define D_PEDSTD_TIML           (768 + 0x2A)
#define D_PEDSTD_TIMH           (768 + 0x2E)
#define D_PEDSTD_PEAK           (768 + 0X94)
#define D_PEDSTD_STEPCTR        (768 + 0x60)
#define D_PEDSTD_TIMECTR        (964)
#define D_PEDSTD_DECI           (768 + 0xA0)

#define D_HOST_NO_MOT           (976)
#define D_ACCEL_BIAS            (660)

#define D_ORIENT_GAP            (76)

#define D_TILT0_H               (48)
#define D_TILT0_L               (50)
#define D_TILT1_H               (52)
#define D_TILT1_L               (54)
#define D_TILT2_H               (56)
#define D_TILT2_L               (58)
#define D_TILT3_H               (60)
#define D_TILT3_L               (62)

#define D_BM_BATCH_CNTR         (27 * 16 + 4)
#define D_BM_BATCH_THLD         (27 * 16 + 8)
#define D_BM_ENABLE             (28 * 16 + 6)
#define D_BM_NUMWORD_TOFILL     (28 * 16 + 4)

#define D_SO_DATA               (4 * 16 + 2)
#define D_P_HW_ID               (22 * 16 + 10)
#define D_P_INIT                (22 * 16 + 2)

#define D_DMP_RUN_CNTR          (24*16)

#define D_ODR_S0                (45*16+8)
#define D_ODR_S1                (45*16+12)
#define D_ODR_S2                (45*16+10)
#define D_ODR_S3                (45*16+14)
#define D_ODR_S4                (46*16+8)
#define D_ODR_S5                (46*16+12)
#define D_ODR_S6                (46*16+10)
#define D_ODR_S7                (46*16+14)

#define D_ODR_CNTR_S0           (45*16)
#define D_ODR_CNTR_S1           (45*16+4)
#define D_ODR_CNTR_S2           (45*16+2)
#define D_ODR_CNTR_S3           (45*16+6)
#define D_ODR_CNTR_S4           (46*16)
#define D_ODR_CNTR_S5           (46*16+4)
#define D_ODR_CNTR_S6           (46*16+2)
#define D_ODR_CNTR_S7           (46*16+6)

static const struct tKeyLabel dmpTConfig[] = {
	{KEY_CFG_OUT_ACCL,              CFG_OUT_ACCL},
	{KEY_CFG_OUT_GYRO,              CFG_OUT_GYRO},
	{KEY_CFG_OUT_3QUAT,             CFG_OUT_3QUAT},
	{KEY_CFG_OUT_6QUAT,             CFG_OUT_6QUAT},
	{KEY_CFG_OUT_PQUAT,             CFG_OUT_PQUAT},
	{KEY_CFG_PED_ENABLE,            CFG_PED_ENABLE},
	{KEY_CFG_FIFO_INT,              CFG_FIFO_INT},
	{KEY_CFG_AUTH,                  CFG_AUTH},
	{KEY_FCFG_1,                    FCFG_1},
	{KEY_FCFG_3,                    FCFG_3},
	{KEY_FCFG_2,                    FCFG_2},
	{KEY_CFG_DISPLAY_ORIENT_INT,    CFG_DISPLAY_ORIENT_INT},
	{KEY_FCFG_7,                    FCFG_7},
	{KEY_FCFG_6,                    FCFG_6},
	{KEY_CFG_EXT_GYRO_BIAS,         CFG_EXT_GYRO_BIAS},
	{KEY_CFG_7,                     CFG_7},
	{KEY_CFG_MOTION_BIAS,           CFG_MOTION_BIAS},
	{KEY_CFG_DISPLAY_ORIENT_INT,    CFG_DISPLAY_ORIENT_INT},
	{KEY_CFG_GYRO_RAW_DATA,         CFG_GYRO_RAW_DATA},
	{KEY_CFG_PEDSTEP_DET,           CFG_PEDSTEP_DET},
	{KEY_D_0_22,                    D_0_22},
	{KEY_D_0_96,                    D_0_96},
	{KEY_D_0_104,                   D_0_104},
	{KEY_D_0_108,                   D_0_108},
	{KEY_D_1_36,                    D_1_36},
	{KEY_D_1_40,                    D_1_40},
	{KEY_D_1_44,                    D_1_44},
	{KEY_D_1_72,                    D_1_72},
	{KEY_D_1_74,                    D_1_74},
	{KEY_D_1_79,                    D_1_79},
	{KEY_D_1_88,                    D_1_88},
	{KEY_D_1_90,                    D_1_90},
	{KEY_D_1_92,                    D_1_92},
	{KEY_D_1_160,                   D_1_160},
	{KEY_D_1_176,                   D_1_176},
	{KEY_D_1_218,                   D_1_218},
	{KEY_D_1_232,                   D_1_232},
	{KEY_D_1_250,                   D_1_250},
	{KEY_DMP_SH_TH_Y,               DMP_SH_TH_Y},
	{KEY_DMP_SH_TH_X,               DMP_SH_TH_X},
	{KEY_DMP_SH_TH_Z,               DMP_SH_TH_Z},
	{KEY_DMP_ORIENT,                DMP_ORIENT},
	{KEY_D_AUTH_OUT,                D_AUTH_OUT},
	{KEY_D_AUTH_IN,                 D_AUTH_IN},
	{KEY_D_AUTH_A,                  D_AUTH_A},
	{KEY_D_AUTH_B,                  D_AUTH_B},
	{KEY_D_ACT0,                    D_ACT0},
	{KEY_D_ACSX,                    D_ACSX},
	{KEY_D_ACSY,                    D_ACSY},
	{KEY_D_ACSZ,                    D_ACSZ},
	{KEY_D_PEDSTD_BP_B,             D_PEDSTD_BP_B},
	{KEY_D_PEDSTD_HP_A,             D_PEDSTD_HP_A},
	{KEY_D_PEDSTD_HP_B,             D_PEDSTD_HP_B},
	{KEY_D_PEDSTD_BP_A4,            D_PEDSTD_BP_A4},
	{KEY_D_PEDSTD_BP_A3,            D_PEDSTD_BP_A3},
	{KEY_D_PEDSTD_BP_A2,            D_PEDSTD_BP_A2},
	{KEY_D_PEDSTD_BP_A1,            D_PEDSTD_BP_A1},
	{KEY_D_PEDSTD_INT_THRSH,        D_PEDSTD_INT_THRSH},
	{KEY_D_PEDSTD_CLIP,             D_PEDSTD_CLIP},
	{KEY_D_PEDSTD_SB,               D_PEDSTD_SB},
	{KEY_D_PEDSTD_SB_TIME,          D_PEDSTD_SB_TIME},
	{KEY_D_PEDSTD_PEAKTHRSH,        D_PEDSTD_PEAKTHRSH},
	{KEY_D_PEDSTD_TIML,             D_PEDSTD_TIML},
	{KEY_D_PEDSTD_TIMH,             D_PEDSTD_TIMH},
	{KEY_D_PEDSTD_PEAK,             D_PEDSTD_PEAK},
	{KEY_D_PEDSTD_STEPCTR,          D_PEDSTD_STEPCTR},
	{KEY_D_PEDSTD_TIMECTR,          D_PEDSTD_TIMECTR},
	{KEY_D_PEDSTD_DECI,             D_PEDSTD_DECI},
	{KEY_D_HOST_NO_MOT,             D_HOST_NO_MOT},
	{KEY_D_ACCEL_BIAS,              D_ACCEL_BIAS},
	{KEY_D_ORIENT_GAP,              D_ORIENT_GAP},
	{KEY_D_TILT0_H,                 D_TILT0_H},
	{KEY_D_TILT0_L,                 D_TILT0_L},
	{KEY_D_TILT1_H,                 D_TILT1_H},
	{KEY_D_TILT1_L,                 D_TILT1_L},
	{KEY_D_TILT2_H,                 D_TILT2_H},
	{KEY_D_TILT2_L,                 D_TILT2_L},
	{KEY_D_TILT3_H,                 D_TILT3_H},
	{KEY_D_TILT3_L,                 D_TILT3_L},
	{KEY_CFG_EXT_GYRO_BIAS_X,       D_EXT_GYRO_BIAS_X},
	{KEY_CFG_EXT_GYRO_BIAS_Y,       D_EXT_GYRO_BIAS_Y},
	{KEY_CFG_EXT_GYRO_BIAS_Z,       D_EXT_GYRO_BIAS_Z},
	{KEY_CFG_PED_INT,               CFG_PED_INT},
	{KEY_SMD_ENABLE,                D_SMD_ENABLE},
	{KEY_SMD_ACCEL_THLD,            D_SMD_MOT_THLD},
	{KEY_SMD_DELAY_THLD,            D_SMD_DELAY_THLD},
	{KEY_SMD_DELAY2_THLD,           D_SMD_DELAY2_THLD},
	{KEY_SMD_ENABLE_TESTPT1,        SMD_TP1},
	{KEY_SMD_ENABLE_TESTPT2,        SMD_TP2},
	{KEY_SMD_EXE_STATE,             D_SMD_EXE_STATE},
	{KEY_SMD_DELAY_CNTR,            D_SMD_DELAY_CNTR},
	{KEY_BM_ENABLE,                 D_BM_ENABLE},
	{KEY_BM_BATCH_CNTR,             D_BM_BATCH_CNTR},
	{KEY_BM_BATCH_THLD,             D_BM_BATCH_THLD},
	{KEY_BM_NUMWORD_TOFILL,         D_BM_NUMWORD_TOFILL},
	{KEY_SO_DATA,                   D_SO_DATA},
	{KEY_P_HW_ID,                   D_P_HW_ID},
	{KEY_P_INIT,                    D_P_INIT},
	{KEY_CFG_OUT_CPASS,             CFG_OUT_CPASS},
	{KEY_CFG_OUT_PRESS,             CFG_OUT_PRESS},
	{KEY_CFG_OUT_STEPDET,           CFG_OUT_STEPDET},
	{KEY_CFG_3QUAT_ODR,             D_ODR_S1},
	{KEY_CFG_6QUAT_ODR,             D_ODR_S2},
	{KEY_CFG_PQUAT_ODR,             D_ODR_S3},
	{KEY_CFG_ACCL_ODR,              D_ODR_S4},
	{KEY_CFG_GYRO_ODR,              D_ODR_S5},
	{KEY_CFG_CPASS_ODR,             D_ODR_S6},
	{KEY_CFG_PRESS_ODR,             D_ODR_S7},
	{KEY_ODR_CNTR_3QUAT,            D_ODR_CNTR_S1},
	{KEY_ODR_CNTR_6QUAT,            D_ODR_CNTR_S2},
	{KEY_ODR_CNTR_PQUAT,            D_ODR_CNTR_S3},
	{KEY_ODR_CNTR_ACCL,             D_ODR_CNTR_S4},
	{KEY_ODR_CNTR_GYRO,             D_ODR_CNTR_S5},
	{KEY_ODR_CNTR_CPASS,            D_ODR_CNTR_S6},
	{KEY_ODR_CNTR_PRESS,            D_ODR_CNTR_S7},
	{KEY_DMP_RUN_CNTR,              D_DMP_RUN_CNTR},
	{KEY_TEST_01,                   OUT_ACCL_DAT},
	{KEY_TEST_02,                   OUT_GYRO_DAT},
	{KEY_TEST_03,                   OUT_CPASS_DAT},
	{KEY_TEST_04,                   OUT_PRESS_DAT},
	{KEY_TEST_05,                   OUT_3QUAT_DAT},
	{KEY_TEST_06,                   OUT_6QUAT_DAT},
	{KEY_TEST_07,                   OUT_PQUAT_DAT}
};
#define NUM_LOCAL_KEYS (sizeof(dmpTConfig)/sizeof(dmpTConfig[0]))

static struct tKeyLabel keys[NUM_KEYS];

unsigned short inv_dmp_get_address(unsigned short key)
{
	static int isSorted;

	if (!isSorted) {
		int kk;
		for (kk = 0; kk < NUM_KEYS; ++kk) {
			keys[kk].addr = 0xffff;
			keys[kk].key = kk;
		}
		for (kk = 0; kk < NUM_LOCAL_KEYS; ++kk)
			keys[dmpTConfig[kk].key].addr = dmpTConfig[kk].addr;
		isSorted = 1;
	}
	if (key >= NUM_KEYS) {
		pr_err("ERROR!! key not exist=%d!\n", key);
		return 0xffff;
	}
	if (0xffff == keys[key].addr)
		pr_err("ERROR!!key not local=%d!\n", key);
	return keys[key].addr;
}
