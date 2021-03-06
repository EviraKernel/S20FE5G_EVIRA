/******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 *****************************************************************************/

#include "mp_precomp.h"

#include "../phydm_precomp.h"

#if (RTL8188E_SUPPORT == 1)

void odm_config_rf_reg_8188e(struct dm_struct *dm, u32 addr, u32 data,
			     enum rf_path RF_PATH, u32 reg_addr)
{
#if (DM_ODM_SUPPORT_TYPE == ODM_AP)
#ifndef SMP_SYNC
	unsigned long x;
#endif
	struct rtl8192cd_priv *priv = dm->priv;
#endif

	if (addr == 0xffe) {
#ifdef CONFIG_LONG_DELAY_ISSUE
		ODM_sleep_msx(50);
#else
		ODM_delay_msx(50);
#endif
	} else if (addr == 0xfd)
		ODM_delay_msx(5);
	else if (addr == 0xfc)
		ODM_delay_msx(1);
	else if (addr == 0xfb)
		ODM_delay_usx(50);
	else if (addr == 0xfa)
		ODM_delay_usx(5);
	else if (addr == 0xf9)
		ODM_delay_usx(1);
	else {
#if (DM_ODM_SUPPORT_TYPE == ODM_AP)
		SAVE_INT_AND_CLI(x);
		odm_set_rf_regx(dm, RF_PATH, reg_addr, RFREGOFFSETMASK, data);
		RESTORE_INT(x);
#else
		odm_set_rf_regx(dm, RF_PATH, reg_addr, RFREGOFFSETMASK, data);
#endif
		/* Add 1us delay between BB/RF register setting. */
		ODM_delay_usx(1);
	}
}

void odm_config_rf_radio_a_8188e(struct dm_struct *dm, u32 addr, u32 data)
{
	u32 content = 0x1000; /* RF_Content: radioa_txt */
	u32 maskfor_phy_set = (u32)(content & 0xE000);

	odm_config_rf_reg_8188e(dm, addr, data, RF_PATH_A, addr | maskfor_phy_set);

	PHYDM_DBG(dm, ODM_COMP_INIT,
		  "===> odm_config_rf_with_header_filex: [RadioA] %08X %08X\n",
		  addr, data);
}

void odm_config_rf_radio_b_8188e(struct dm_struct *dm, u32 addr, u32 data)
{
	u32 content = 0x1001; /* RF_Content: radiob_txt */
	u32 maskfor_phy_set = (u32)(content & 0xE000);

	odm_config_rf_reg_8188e(dm, addr, data, RF_PATH_B, addr | maskfor_phy_set);

	PHYDM_DBG(dm, ODM_COMP_INIT,
		  "===> odm_config_rf_with_header_filex: [RadioB] %08X %08X\n",
		  addr, data);
}

void odm_config_mac_8188e(struct dm_struct *dm, u32 addr, u8 data)
{
	odm_write_1bytex(dm, addr, data);
	PHYDM_DBG(dm, ODM_COMP_INIT,
		  "===> odm_config_mac_with_header_filex: [MAC_REG] %08X %08X\n",
		  addr, data);
}

void odm_config_bb_agc_8188e(struct dm_struct *dm, u32 addr, u32 bitmask,
			     u32 data)
{
	odm_set_bb_regx(dm, addr, bitmask, data);
	/* Add 1us delay between BB/RF register setting. */
	ODM_delay_usx(1);

	PHYDM_DBG(dm, ODM_COMP_INIT,
		  "===> odm_config_bb_with_header_filex: [AGC_TAB] %08X %08X\n",
		  addr, data);
}

void odm_config_bb_phy_reg_pg_8188e(struct dm_struct *dm, u32 band, u32 rf_path,
				    u32 tx_num, u32 addr, u32 bitmask, u32 data)
{
	if (addr == 0xfe) {
#ifdef CONFIG_LONG_DELAY_ISSUE
		ODM_sleep_msx(50);
#else
		ODM_delay_msx(50);
#endif
	} else if (addr == 0xfd)
		ODM_delay_msx(5);
	else if (addr == 0xfc)
		ODM_delay_msx(1);
	else if (addr == 0xfb)
		ODM_delay_usx(50);
	else if (addr == 0xfa)
		ODM_delay_usx(5);
	else if (addr == 0xf9)
		ODM_delay_usx(1);
	else {
		PHYDM_DBG(dm, ODM_COMP_INIT,
			  "===> odm_config_bb_with_header_filex: [PHY_REG] %08X %08X %08X\n",
			  addr, bitmask, data);

#if (DM_ODM_SUPPORT_TYPE & ODM_CE)
		phy_store_tx_power_by_ratex(dm->adapter, band, rf_path, tx_num, addr, bitmask, data);
#elif (DM_ODM_SUPPORT_TYPE & ODM_WIN)
		PHY_StoreTxPowerByRate(dm->adapter, band, rf_path, tx_num, addr, bitmask, data);
#endif
	}
}

void odm_config_bb_txpwr_lmt_8188e(struct dm_struct *dm, u8 *regulation,
				   u8 *band, u8 *bandwidth, u8 *rate_section,
				   u8 *rf_path, u8 *channel, u8 *power_limit)
{
#if (DM_ODM_SUPPORT_TYPE & ODM_CE)
	phy_set_tx_power_limitx(dm, regulation, band,
			       bandwidth, rate_section, rf_path, channel, power_limit);
#elif (DM_ODM_SUPPORT_TYPE & ODM_WIN)
	PHY_SetTxPowerxLimit(dm, regulation, band,
			    bandwidth, rate_section, rf_path, channel, power_limit);
#endif
}

void odm_config_bb_phy_8188e(struct dm_struct *dm, u32 addr, u32 bitmask,
			     u32 data)
{
	if (addr == 0xfe) {
#ifdef CONFIG_LONG_DELAY_ISSUE
		ODM_sleep_msx(50);
#else
		ODM_delay_msx(50);
#endif
	} else if (addr == 0xfd)
		ODM_delay_msx(5);
	else if (addr == 0xfc)
		ODM_delay_msx(1);
	else if (addr == 0xfb)
		ODM_delay_usx(50);
	else if (addr == 0xfa)
		ODM_delay_usx(5);
	else if (addr == 0xf9)
		ODM_delay_usx(1);
	else {
		if (addr == 0xa24)
			dm->rf_calibrate_info.rega24 = data;
		odm_set_bb_regx(dm, addr, bitmask, data);

		/* Add 1us delay between BB/RF register setting. */
		ODM_delay_usx(1);
		PHYDM_DBG(dm, ODM_COMP_INIT,
			  "===> odm_config_bb_with_header_filex: [PHY_REG] %08X %08X\n",
			  addr, data);
	}
}
#endif
