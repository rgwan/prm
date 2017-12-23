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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>

/* Set STM32 to 72 MHz. */
static void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	rcc_css_enable();

	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);
	
	rcc_periph_clock_enable(RCC_TIM2);
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
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO8 | GPIO2 | GPIO3);
		
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		GPIO_TIM2_CH2);		 
}

#define TIME_BASE 4000000

uint32_t time_frequency = 0;
uint32_t time;

static void tim_frequency_set(uint16_t frequency)
{
	if(TIME_BASE / frequency < 16) return;
	time_frequency = TIME_BASE / frequency;
	timer_set_period(TIM2, time_frequency);
}


static void tim_duty_set(float duty)
{
	if(duty > 1.0) duty = 0.99;
	if(duty < 0) duty = 0;
	timer_set_oc_value(TIM2, TIM_OC2, duty * time_frequency); // Blue LED
}

#define VVVF_STAGE1  6000
#define VVVF_STAGE2 (2600 + VVVF_STAGE1)
#define VVVF_STAGE3 (3000 + VVVF_STAGE2)
#define VVVF_STAGE4 (4000 + VVVF_STAGE3)
#define VVVF_STAGE5 (8000 + VVVF_STAGE4)
#define VVVF_STAGE6 (3000 + VVVF_STAGE5)

uint16_t note_freq_table[] = {262, 293, 349, 699, 392, 440, 494, 524, 587, 659, 699, 784, 880, 988, 1048};
uint16_t score[] = {
	5+7, 200,
	0, 200,
	5+7, 100,
	0, 100,
	6+7, 100,
	0, 100,
	2+7, 400,
	0, 400,
	
	1+7, 200,
	0, 200,
	1+7, 100,
	0, 100,
	6, 100,
	0, 100,
	2+7, 400,
	0, 400,
	
	5+7, 200,
	0, 200,
	5+7, 200,
	0, 200,
	
	6+7, 100,
	0, 100,		
	1+14, 100,
	0, 100,	
	6+7, 100,
	0, 100,	
	5+7, 100,
	0, 100,	
	
	1+7, 200,
	0, 200,
	1+7, 100,
	0, 100,
	6, 100,
	0, 100,
	2+7, 400,
	0, 400,
	
	5+7, 200,
	0, 200,
	2+7, 200,
	0, 200,
	
	1+7, 200,
	0, 200,

	7, 100,
	0, 100,
	6, 100,
	0, 100,
	
	5, 200,
	0, 200,
	5+7, 200,
	0, 200,
	
	2+7, 200,
	0, 200,
	3+7, 100,
	0, 100,
	2+7, 100,
	0, 100,
	
	1+7 ,200,
	0, 200,
	1+7, 100,
	0, 100,
	6, 100,
	0, 100,
	
	2+7, 100,
	0, 100,
	3+7, 100,
	0, 100,
	2+7, 100,
	0, 100,
	1+7,100,
	0, 100,
	
	2+7,100,
	0,100,
	1+7,100,
	0, 100,
	7, 100,
	0, 100,
	6, 100,
	0, 100,
	5, 400,
	0, 400,
	
	0, -1};

void sys_tick_handler(void)
{
#if 1	
	static float id = 0;
	/* ratio 6s */
	if (time == 0)
	{
		tim_frequency_set(600);
	}
	if (time == VVVF_STAGE1)
	{
		tim_frequency_set(800);
		gpio_toggle(GPIOB, GPIO11);
	}
	if (time == VVVF_STAGE2)
	{
		//tim_frequency_set(800);
		gpio_toggle(GPIOB, GPIO11);
	}
	if (time == VVVF_STAGE3)
	{
		//tim_frequency_set(800);
		gpio_toggle(GPIOB, GPIO11);
	}
	if (time == VVVF_STAGE4)
	{
		//tim_frequency_set(800);
		gpio_toggle(GPIOB, GPIO11);
	}
	if (time == VVVF_STAGE5)
	{
		tim_frequency_set(800);
		gpio_toggle(GPIOB, GPIO11);
	}
	if (time == VVVF_STAGE6)
	{
		tim_frequency_set(600);
		gpio_toggle(GPIOB, GPIO11);
	}
		
	if(time < VVVF_STAGE1)
	{ /* 6s */
		tim_duty_set(id);
		id += 0.5 / 15600;
	}
	if(time < VVVF_STAGE2 && time >= VVVF_STAGE1)
	{/* 2.6s */
		tim_duty_set(id);
		id += 0.5 / 15600;		
	}
	if(time < VVVF_STAGE3 && time >= VVVF_STAGE2)
	{/* 3s */
		tim_frequency_set(time * 4 * 26 / 1000);
		tim_duty_set(id);
		id += 0.5 / 15600;		
	}
	if(time < VVVF_STAGE4 && time >= VVVF_STAGE3)
	{/* 4s */
		tim_frequency_set(time * 4 * 20 / 1000);
		tim_duty_set(id);
		id += 0.5 / 15600;		
	}	
	
	if(time >= VVVF_STAGE5)
	{
		id -= 0.1 / 3000;
		tim_duty_set(id);
	}
#else
	static uint32_t score_p = 0;
	static uint32_t score_tim = 0;
	if(time >= score_tim)
	{
		if(score[score_p * 2] != 0)
		{
			tim_frequency_set(note_freq_table[score[score_p * 2] - 1]);
			tim_duty_set(0.3);
		}
		else
		{
			tim_duty_set(0);
		}
		
		if (score[score_p * 2 + 1] == (uint16_t) -1)
		{
			score_p = 0;
		}
		else if(score[score_p * 2 + 1] != 0)
		{
			score_tim += score[score_p * 2 + 1] * 2;
			score_p ++;
		}
		else
		{
			tim_duty_set(0);
		}
	}
	
#endif
	time++;
	
}

static void tim_setup(void)
{
	// First configure timer 1.
	// Reset the timer configuration and then set it up to use the CPU clock,
	// center-aligned PWM, and an increasing rate.
	timer_reset(TIM2);
	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);
	// Divide counter by 16 to scale it down from 16mhz clock speed to a 1mhz rate.
	timer_set_prescaler(TIM2, 8);
	// Set timer period by solving:
	// PWM frequency = timer clock speed / timer period
	tim_frequency_set(600);
	//timer_enable_break_main_output(TIM2);// Must be called for advanced timers
		// like this one.Unclear what this
		// does or why it's necessary but the
		// libopencm3 timer and STM32 docs
		// mention it.
		// PA8 Blue LED is timer 1, channel 1.
		// Enable the channel PWM output.
	timer_enable_oc_output(TIM2, TIM_OC2);
	timer_enable_counter(TIM2);
	// Set the PWM mode to 2 which means the PWM signal is low (LED turns on) when
	// the timer value is below the threshold and high (LED off) above it.
	timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM2);
	
	timer_set_oc_value(TIM2, TIM_OC2, 0); // Blue LED
}

static void systick_setup(void)
{
	time = 0;
	SCB_VTOR = 0x08000000;
	/* 72MHz / 8 => 9000000 counts per second */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

	/* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
	/* SysTick interrupt every N clock pulses: set reload to N-1 */
	systick_set_reload(8999);

	systick_interrupt_enable();

	/* Start counting. */
	systick_counter_enable();
}
int main(void)
{
	volatile int i;
	
	uint16_t comp = 0;
	uint16_t val = 655;
	
	clock_setup();
	gpio_setup();
	
	systick_setup();
	
	tim_setup();
	
	gpio_clear(GPIOA, GPIO8);
	gpio_clear(GPIOB, GPIO11);

	while (1) {
		
	}

	return 0;
}
