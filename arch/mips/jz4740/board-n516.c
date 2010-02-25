/*
 * linux/arch/mips/jz4740/board-516.c
 *
 * JZ4740 n516 board setup routines.
 *
 * Copyright (c) 2009, Yauhen Kharuzhy <jekhor@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/init.h>
#include <linux/sched.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/mtd/mtd.h>
#include <linux/mmc/jz4740_mmc.h>
#include <linux/mtd/jz4740_nand.h>
#include <linux/leds.h>

#include <asm/cpu.h>
#include <asm/bootinfo.h>
#include <asm/mipsregs.h>
#include <asm/reboot.h>
#include <asm/sizes.h>

#include <asm/mach-jz4740/gpio.h>
#include <asm/mach-jz4740/pm.h>
#include <asm/mach-jz4740/platform.h>
#include <asm/mach-jz4740/regs.h>

#include <asm/mach-jz4740/board-n516.h>

//n = 0,1,2,3
#define GPIO_PXPIN(n)	(GPIO_BASE + (0x00 + (n)*0x100)) /* PIN Level Register */
#define GPIO_PXDAT(n)	(GPIO_BASE + (0x10 + (n)*0x100)) /* Port Data Register */
#define GPIO_PXDATS(n)	(GPIO_BASE + (0x14 + (n)*0x100)) /* Port Data Set Register */
#define GPIO_PXDATC(n)	(GPIO_BASE + (0x18 + (n)*0x100)) /* Port Data Clear Register */
#define GPIO_PXIM(n)	(GPIO_BASE + (0x20 + (n)*0x100)) /* Interrupt Mask Register */
#define GPIO_PXIMS(n)	(GPIO_BASE + (0x24 + (n)*0x100)) /* Interrupt Mask Set Reg */
#define GPIO_PXIMC(n)	(GPIO_BASE + (0x28 + (n)*0x100)) /* Interrupt Mask Clear Reg */
#define GPIO_PXPE(n)	(GPIO_BASE + (0x30 + (n)*0x100)) /* Pull Enable Register */
#define GPIO_PXPES(n)	(GPIO_BASE + (0x34 + (n)*0x100)) /* Pull Enable Set Reg. */
#define GPIO_PXPEC(n)	(GPIO_BASE + (0x38 + (n)*0x100)) /* Pull Enable Clear Reg. */
#define GPIO_PXFUN(n)	(GPIO_BASE + (0x40 + (n)*0x100)) /* Function Register */
#define GPIO_PXFUNS(n)	(GPIO_BASE + (0x44 + (n)*0x100)) /* Function Set Register */
#define GPIO_PXFUNC(n)	(GPIO_BASE + (0x48 + (n)*0x100)) /* Function Clear Register */
#define GPIO_PXSEL(n)	(GPIO_BASE + (0x50 + (n)*0x100)) /* Select Register */
#define GPIO_PXSELS(n)	(GPIO_BASE + (0x54 + (n)*0x100)) /* Select Set Register */
#define GPIO_PXSELC(n)	(GPIO_BASE + (0x58 + (n)*0x100)) /* Select Clear Register */
#define GPIO_PXDIR(n)	(GPIO_BASE + (0x60 + (n)*0x100)) /* Direction Register */
#define GPIO_PXDIRS(n)	(GPIO_BASE + (0x64 + (n)*0x100)) /* Direction Set Register */
#define GPIO_PXDIRC(n)	(GPIO_BASE + (0x68 + (n)*0x100)) /* Direction Clear Register */
#define GPIO_PXTRG(n)	(GPIO_BASE + (0x70 + (n)*0x100)) /* Trigger Register */
#define GPIO_PXTRGS(n)	(GPIO_BASE + (0x74 + (n)*0x100)) /* Trigger Set Register */
#define GPIO_PXTRGC(n)	(GPIO_BASE + (0x78 + (n)*0x100)) /* Trigger Set Register */
#define GPIO_PXFLG(n)	(GPIO_BASE + (0x80 + (n)*0x100)) /* Port Flag Register */
#define GPIO_PXFLGC(n)	(GPIO_BASE + (0x14 + (n)*0x100)) /* Port Flag Clear Register */

#define REG_GPIO_PXPIN(n)	REG32(GPIO_PXPIN((n)))  /* PIN level */
#define REG_GPIO_PXDAT(n)	REG32(GPIO_PXDAT((n)))  /* 1: interrupt pending */
#define REG_GPIO_PXDATS(n)	REG32(GPIO_PXDATS((n)))
#define REG_GPIO_PXDATC(n)	REG32(GPIO_PXDATC((n)))
#define REG_GPIO_PXIM(n)	REG32(GPIO_PXIM((n)))   /* 1: mask pin interrupt */
#define REG_GPIO_PXIMS(n)	REG32(GPIO_PXIMS((n)))
#define REG_GPIO_PXIMC(n)	REG32(GPIO_PXIMC((n)))
#define REG_GPIO_PXPE(n)	REG32(GPIO_PXPE((n)))   /* 1: disable pull up/down */
#define REG_GPIO_PXPES(n)	REG32(GPIO_PXPES((n)))
#define REG_GPIO_PXPEC(n)	REG32(GPIO_PXPEC((n)))
#define REG_GPIO_PXFUN(n)	REG32(GPIO_PXFUN((n)))  /* 0:GPIO or intr, 1:FUNC */
#define REG_GPIO_PXFUNS(n)	REG32(GPIO_PXFUNS((n)))
#define REG_GPIO_PXFUNC(n)	REG32(GPIO_PXFUNC((n)))
#define REG_GPIO_PXSEL(n)	REG32(GPIO_PXSEL((n))) /* 0:GPIO/Fun0,1:intr/fun1*/
#define REG_GPIO_PXSELS(n)	REG32(GPIO_PXSELS((n)))
#define REG_GPIO_PXSELC(n)	REG32(GPIO_PXSELC((n)))
#define REG_GPIO_PXDIR(n)	REG32(GPIO_PXDIR((n))) /* 0:input/low-level-trig/falling-edge-trig, 1:output/high-level-trig/rising-edge-trig */
#define REG_GPIO_PXDIRS(n)	REG32(GPIO_PXDIRS((n)))
#define REG_GPIO_PXDIRC(n)	REG32(GPIO_PXDIRC((n)))
#define REG_GPIO_PXTRG(n)	REG32(GPIO_PXTRG((n))) /* 0:level-trigger, 1:edge-trigger */
#define REG_GPIO_PXTRGS(n)	REG32(GPIO_PXTRGS((n)))
#define REG_GPIO_PXTRGC(n)	REG32(GPIO_PXTRGC((n)))
#define REG_GPIO_PXFLG(n)	REG32(GPIO_PXFLG((n))) /* interrupt flag */
#define REG_GPIO_PXFLGC(n)	REG32(GPIO_PXFLGC((n))) /* interrupt flag */


/* 
 * __gpio_as_sleep set all pins to pull-disable, and set all pins as input
 * except sdram, nand flash pins and the pins which can be used as CS1_N  
 * to CS4_N for chip select. 
 */
#define __gpio_as_sleep()	              \
do {	                                      \
	REG_GPIO_PXFUNC(1) = ~0x9fffffff;     \
	REG_GPIO_PXSELC(1) = ~0x9fffffff;     \
	REG_GPIO_PXDIRC(1) = ~0x9fffffff;     \
	REG_GPIO_PXPES(1)  =  0xffffffff;     \
	REG_GPIO_PXFUNC(2) =  0xc8ffffff;     \
	REG_GPIO_PXSELC(2) =  0xc8ffffff;     \
	REG_GPIO_PXDIRC(2) =  0xc8ffffff;     \
	REG_GPIO_PXPES(2)  =  0xffffffff;     \
	REG_GPIO_PXFUNC(3) =  0xefc57f7f;     \
	REG_GPIO_PXSELC(3) =  0xefc57f7f;     \
	REG_GPIO_PXDIRC(3) =  0xefc57f7f;     \
	REG_GPIO_PXPES(3)  =  0xfbffffff;     \
	REG_GPIO_PXPEC(3)  =  0x04000000;     \
} while (0)

static long n516_panic_blink(long time)
{
	gpio_set_value(GPIO_LED_EN, 1);
	mdelay(200);
	gpio_set_value(GPIO_LED_EN, 0);
	mdelay(200);

	return 400;
}

void n516_do_sleep(unsigned long *ptr)
{
	unsigned char i;

        /* Print messages of GPIO registers for debug */
	for(i=0;i<4;i++) {
		pr_debug("run dat:%x pin:%x fun:%x sel:%x dir:%x pull:%x msk:%x trg:%x\n",        \
			REG_GPIO_PXDAT(i),REG_GPIO_PXPIN(i),REG_GPIO_PXFUN(i),REG_GPIO_PXSEL(i), \
			REG_GPIO_PXDIR(i),REG_GPIO_PXPE(i),REG_GPIO_PXIM(i),REG_GPIO_PXTRG(i));
	}

        /* Save GPIO registers */
	for(i = 1; i < 4; i++) {
		*ptr++ = REG_GPIO_PXFUN(i);
		*ptr++ = REG_GPIO_PXSEL(i);
		*ptr++ = REG_GPIO_PXDIR(i);
		*ptr++ = REG_GPIO_PXPE(i);
		*ptr++ = REG_GPIO_PXIM(i);
		*ptr++ = REG_GPIO_PXDAT(i);
		*ptr++ = REG_GPIO_PXTRG(i);
	}
	gpio_set_value(GPIO_LED_EN, 1);
	mdelay(50);
	gpio_set_value(GPIO_LED_EN, 0);

	__gpio_as_sleep();
	gpio_direction_input(JZ_GPIO_PORTB(27));
	jz_gpio_enable_pullup(JZ_GPIO_PORTB(28));
	jz_gpio_enable_pullup(JZ_GPIO_PORTB(29));
	jz_gpio_enable_pullup(JZ_GPIO_PORTB(30));
	jz_gpio_enable_pullup(JZ_GPIO_PORTB(31));
	jz_gpio_enable_pullup(JZ_GPIO_PORTC(27));
}

static void n516_do_resume(unsigned long *ptr)
{
	unsigned char i;

	gpio_set_value(GPIO_LED_EN, 1);
	mdelay(50);

	/* Restore GPIO registers */
	for(i = 1; i < 4; i++) {
		 REG_GPIO_PXFUNS(i) = *ptr;
		 REG_GPIO_PXFUNC(i) = ~(*ptr++);

		 REG_GPIO_PXSELS(i) = *ptr;
		 REG_GPIO_PXSELC(i) = ~(*ptr++);

		 REG_GPIO_PXDIRS(i) = *ptr;
		 REG_GPIO_PXDIRC(i) = ~(*ptr++);

		 REG_GPIO_PXPES(i) = *ptr;
		 REG_GPIO_PXPEC(i) = ~(*ptr++);

		 REG_GPIO_PXIMS(i)=*ptr;
		 REG_GPIO_PXIMC(i)=~(*ptr++);
	
		 REG_GPIO_PXDATS(i)=*ptr;
		 REG_GPIO_PXDATC(i)=~(*ptr++);
	
		 REG_GPIO_PXTRGS(i)=*ptr;
		 REG_GPIO_PXTRGC(i)=~(*ptr++);
	}

}

/*
void n516_setup_wakeup_ints(void)
{
	__gpio_as_irq_fall_edge(GPIO_LPC_INT);
	__gpio_unmask_irq(GPIO_LPC_INT);
	__gpio_unmask_irq(MSC_HOTPLUG_PIN);
	__gpio_unmask_irq(GPIO_USB_DETECT);
	__gpio_unmask_irq(GPIO_CHARG_STAT_N);
}
*/

//static void __init board_cpm_setup(void)
//{
//	/* Stop unused module clocks here.
//	 * We have started all module clocks at arch/mips/jz4740/setup.c.
//	 */
//
////	__cpm_stop_uart1();
//	__cpm_stop_uhc();
//	__cpm_stop_ipu();
//	__cpm_stop_udc();
//	__cpm_stop_cim();
//	__cpm_stop_sadc();
////	__cpm_stop_aic1();
////	__cpm_stop_aic2();
//	__cpm_stop_ssi();
////	__cpm_stop_tcu();
//}

static void __init board_gpio_setup(void)
{
	/*
	 * Most of the GPIO pins should have been initialized by the boot-loader
	 */

	/*
	 * Initialize MSC pins
	 */
//	__gpio_as_msc();

	/*
	 * Initialize LCD pins
	 */
//	__gpio_as_lcd_16bit();

	/*
	 * Initialize I2C pins
	 */
//	__gpio_as_i2c();

	/*
	 * Initialize Other pins
	 */
//	__gpio_as_output(GPIO_SD_VCC_EN_N);
//	__gpio_clear_pin(GPIO_SD_VCC_EN_N);


//	__gpio_as_input(GPIO_SD_CD_N);
//	__gpio_disable_pull(GPIO_SD_CD_N);

//	__gpio_as_input(GPIO_SD_WP);
//	__gpio_disable_pull(GPIO_SD_WP);

//	__gpio_as_input(GPIO_CHARG_STAT_N);
//	__gpio_as_input(GPIO_USB_DETECT);
//	__gpio_as_input(GPIO_HPHONE_PLUG);

//	__gpio_as_output(GPIO_DISP_OFF_N);
//	__gpio_as_output(GPIO_SPK_SHUD);

//	__gpio_as_output(GPIO_LED_EN);

	/* Disable display controller on booting */
	/*
	__gpio_as_output(GPIO_DISPLAY_STBY);
	__gpio_as_output(GPIO_DISPLAY_RST_L);
	__gpio_clear_pin(GPIO_DISPLAY_STBY);
	__gpio_clear_pin(GPIO_DISPLAY_RST_L);
	*/
	/* GPIO_DISPLAY_STBY */
	jz_gpio_port_direction_output(JZ_GPIO_PORTC(0), (1 << 22));
	/* GPIO_DISPLAY_RST_L */
	jz_gpio_port_direction_output(JZ_GPIO_PORTB(0), (1 << 18));
	/* GPIO_DISPLAY_STBY */
	jz_gpio_port_set_value(JZ_GPIO_PORTC(0), 0, (1 << 22));
	/* GPIO_DISPLAY_RST_L */
	jz_gpio_port_set_value(JZ_GPIO_PORTB(0), 0, (1 << 18));

	jz_gpio_enable_pullup(JZ_GPIO_PORTD(26));
	gpio_request(GPIO_USB_DETECT, "USB VBUS detect");
	gpio_direction_input(GPIO_USB_DETECT);
	gpio_free(GPIO_USB_DETECT);
}

static const struct i2c_board_info n516_keys_board_info = {
	.type		= "LPC524",
	.addr		= 0x54,
};

static const struct i2c_board_info n516_lm75a_board_info = {
	.type		= "lm75a",
	.addr		= 0x48,
};

static struct jz4740_mmc_platform_data n516_mmc_pdata = {
	.gpio_card_detect	= GPIO_SD_CD_N,
	.card_detect_active_low = 1,
	.gpio_read_only		= -1,
	.gpio_power		= GPIO_SD_VCC_EN_N,
	.power_active_low = 1,
};


static struct gpio_led n516_leds[] = {
	{
		.name = "n516:blue:power",
		.gpio = GPIO_LED_EN,
		.default_state = LEDS_GPIO_DEFSTATE_ON,
		.default_trigger = "nand-disk",
	}
};

static struct gpio_led_platform_data n516_leds_pdata = {
	.leds = n516_leds,
	.num_leds = ARRAY_SIZE(n516_leds),
};

static struct platform_device n516_leds_device = {
	.name = "leds-gpio",
	.id = -1,
	.dev = {
		.platform_data = &n516_leds_pdata,
	},
};

static struct nand_ecclayout n516_ecclayout = {
	.eccbytes = 36,
	.eccpos = {
		6,  7,  8,  9,  10, 11, 12, 13,
		14, 15, 16, 17, 18, 19, 20, 21,
		22, 23, 24, 25, 26, 27, 28, 29,
		30, 31, 32, 33, 34, 35, 36, 37,
		38, 39, 40, 41},
	.oobfree = {
		{.offset = 2,
		 .length = 4},
		{.offset = 42,
		 .length = 22}}
};

static struct jz_nand_platform_data n516_nand_pdata = {
	.ecc_layout = &n516_ecclayout,
	/* cmdline parsing only */
	.num_partitions = 0,
	.busy_gpio = 94,
};

static struct platform_device *n516_devices[] __initdata = {
	&jz4740_nand_device,
	&n516_leds_device,
	&jz4740_mmc_device,
	&jz4740_i2s_device,
	&jz4740_codec_device,
	&jz4740_rtc_device,
	&jz4740_usb_gdt_device,
	&jz4740_i2c_device,
};

extern int jz_gpiolib_init(void);
extern int jz_init_clocks(unsigned long extal);

static int n516_setup_platform(void)
{
	if (jz_gpiolib_init())
		panic("Failed to initalize jz gpio\n");

	jz_init_clocks(12000000);

	//	board_cpm_setup();
	board_gpio_setup();

	panic_blink = n516_panic_blink;
	//	jz_board_do_sleep = n516_do_sleep;
	//	jz_board_setup_wakeup_ints = n516_setup_wakeup_ints;
	//	jz_board_do_resume = n516_do_resume;
	i2c_register_board_info(0, &n516_keys_board_info, 1);
	i2c_register_board_info(0, &n516_lm75a_board_info, 1);
	jz4740_mmc_device.dev.platform_data = &n516_mmc_pdata;
	jz4740_nand_device.dev.platform_data = &n516_nand_pdata;

	return platform_add_devices(n516_devices, ARRAY_SIZE(n516_devices));
}
arch_initcall(n516_setup_platform);
