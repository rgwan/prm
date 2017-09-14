/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

/* Set STM32 to 72 MHz. */
static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
}

static void gpio_setup(void)
{
	gpio_set(GPIOA, GPIO1 | GPIO2 | GPIO3);
	gpio_set(GPIOB, GPIO0 | GPIO1);
	gpio_set(GPIOB, GPIO12 | GPIO13 | GPIO14| GPIO15);
	/* Set GPIO12 (in GPIO port B) to 'output push-pull'. */
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO0 | GPIO1 |GPIO11 | GPIO12 | GPIO13 | GPIO14| GPIO15);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO8 | GPIO1 | GPIO2 | GPIO3);
}

int main(void)
{
	volatile int i;
	int j = 0;
	
	clock_setup();
	gpio_setup();
	
	
	gpio_clear(GPIOA, GPIO8);


	/* Blink the LED (PB12) on the board. */
	while (1) {
		gpio_toggle(GPIOB, GPIO11);	/* LED on/off */
		gpio_set(GPIOB, GPIO0 | GPIO1 | GPIO12 | GPIO13 | GPIO14| GPIO15);
		if(j < 4)
			gpio_clear(GPIOB, 1 << (12 + j));
		if(j < 6 && j >= 4)
			gpio_clear(GPIOB, 1 << (j - 4));
		j++;
		if(j > 7) j = 0;
		for (i = 0; i < 400000; i++)	/* Wait a bit. */
			__asm__("nop");
	}

	return 0;
}
