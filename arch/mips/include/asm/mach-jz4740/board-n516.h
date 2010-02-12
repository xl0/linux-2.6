/*
 *  linux/include/asm-mips/mach-jz4740/board-n516.h
 *
 *  JZ4730-based N516 board definition.
 *
 *  Copyright (C) 2009, Yauhen Kharuzhy <jekhor@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __ASM_JZ4740_N516_H__
#define __ASM_JZ4740_N516_H__

#include <asm/mach-jz4740/gpio.h>

/*====================================================================== 
 * Frequencies of on-board oscillators
 */
#define JZ_EXTAL		12000000  /* Main extal freq: 12 MHz */
#define JZ_EXTAL_RTC		32768     /* RTC extal freq: 32.768 KHz */


/*====================================================================== 
 * GPIO
 */
#define GPIO_SD_VCC_EN_N	JZ_GPIO_PORTD(17)
#define GPIO_SD_CD_N		JZ_GPIO_PORTD(7)
#define GPIO_SD_WP		JZ_GPIO_PORTD(15)
#define GPIO_USB_DETECT		JZ_GPIO_PORTD(19)
//#define GPIO_DC_DETE_N		103 /* GPD7 */
#define GPIO_CHARG_STAT_N	JZ_GPIO_PORTD(16)
#define GPIO_DISP_OFF_N		JZ_GPIO_PORTD(1)
#define GPIO_LED_EN       	JZ_GPIO_PORTD(28)
#define GPIO_LPC_INT		JZ_GPIO_PORTD(14)
#define GPIO_HPHONE_PLUG	JZ_GPIO_PORTD(20)
#define GPIO_SPK_SHUD		JZ_GPIO_PORTD(21)

#define GPIO_UDC_HOTPLUG	GPIO_USB_DETECT

/* Display */
#define GPIO_DISPLAY_RST_L	JZ_GPIO_PORTB(18)
#define GPIO_DISPLAY_RDY	JZ_GPIO_PORTB(17)
#define GPIO_DISPLAY_STBY	JZ_GPIO_PORTC(22)
#define GPIO_DISPLAY_ERR	JZ_GPIO_PORTC(23)

#endif /* __ASM_JZ4740_N516_H__ */
