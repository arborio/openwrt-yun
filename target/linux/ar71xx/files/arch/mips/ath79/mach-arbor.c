/*
 *  Arbor shield board support
 *
 *  Copyright (C) 2014 B. Griffin <bgriffin@arbor-io.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */
#include <asm/mach-ath79/ar71xx_regs.h>
#include <asm/mach-ath79/ath79.h>
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "common.h"
#include "gpio.h"
#include "linux/gpio.h"

#define ARBOR_GPIO_LED_WLAN				0
#define ARBOR_GPIO_LED_WAN				17
#define ARBOR_GPIO_LED_SYS				7

#define ARBOR_GPIO_BTN_JUMPSTART			11

#define ARBOR_KEYS_POLL_INTERVAL			20	/* msecs */
#define ARBOR_KEYS_DEBOUNCE_INTERVAL		(3 * ARBOR_KEYS_POLL_INTERVAL)

#define ARBOR_MAC0_OFFSET		0x0000
#define ARBOR_MAC1_OFFSET		0x0006
#define ARBOR_CALDATA_OFFSET		0x1000
#define ARBOR_WMAC_MAC_OFFSET		0x1002

static struct gpio_led arbor_leds_gpio[] __initdata = {
	{
		.name		= "arbor:green:wan",
		.gpio		= ARBOR_GPIO_LED_WAN,
		.active_low	= 0,
	},
	{
		.name		= "arbor:blue:wlan",
		.gpio		= ARBOR_GPIO_LED_WLAN,
		.active_low	= 0,
	},
	{
		.name		= "arbor:white:sys",
		.gpio		= ARBOR_GPIO_LED_SYS,
		.active_low	= 0,
	},
};

static struct gpio_keys_button arbor_gpio_keys[] __initdata = {
	{
		.desc			= "jumpstart button",
		.type			= EV_KEY,
		.code			= KEY_WPS_BUTTON,
		.debounce_interval = ARBOR_KEYS_DEBOUNCE_INTERVAL,
		.gpio			= ARBOR_GPIO_BTN_JUMPSTART,
		.active_low		= 1,
	}
};

static void __init arbor_common_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(NULL);
	ath79_register_wmac(art + ARBOR_CALDATA_OFFSET,
			    art + ARBOR_WMAC_MAC_OFFSET);

	ath79_setup_ar933x_phy4_switch(true, true);

	ath79_init_mac(ath79_eth0_data.mac_addr, art + ARBOR_MAC0_OFFSET, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, art + ARBOR_MAC1_OFFSET, 0);

	ath79_register_mdio(0, 0x0);

	/* LAN ports */
	ath79_register_eth(1);

	/* WAN port */
	ath79_register_eth(0);
}

static void __init arbor_setup(void)
{
	arbor_common_setup();

	ath79_gpio_function_disable(AR724X_GPIO_FUNC_ETH_SWITCH_LED0_EN |
				AR724X_GPIO_FUNC_ETH_SWITCH_LED1_EN |
				AR724X_GPIO_FUNC_ETH_SWITCH_LED2_EN |
				AR724X_GPIO_FUNC_ETH_SWITCH_LED3_EN |
				AR724X_GPIO_FUNC_ETH_SWITCH_LED4_EN);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(arbor_leds_gpio),
				 arbor_leds_gpio);
	ath79_register_gpio_keys_polled(-1, ARBOR_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(arbor_gpio_keys),
					arbor_gpio_keys);
	ath79_register_usb();
}

MIPS_MACHINE(ATH79_MACH_ARBOR, "ARBOR", "Arbor",
		arbor_setup);
