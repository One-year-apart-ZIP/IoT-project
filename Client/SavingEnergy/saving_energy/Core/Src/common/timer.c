/*
 * timer.c
 *
 *  Created on: Mar 16, 2024
 *      Author: hojoon
 */

#include "stm32f767xx.h"
#include "common/timer.h"

volatile unsigned char interrupt_flag = 0;
volatile unsigned int interrupt_count = 0;
volatile unsigned char led_flag = 0;

void TIM1_UP_TIM10_IRQHandler(void)
{
	TIM1->SR = 0x0000;
	interrupt_flag = 1;
	interrupt_count++;

	if ((interrupt_count % 100) == 0)
		led_flag = 1;
}

void TIM1_init(void)
{
	RCC->APB2ENR |= 0x00000001;
	TIM1->PSC = 959;
	TIM1->ARR = 49999;
	TIM1->CNT = 0;
	TIM1->DIER = 0x0001;
	TIM1->CR1 = 0x0005;
	NVIC->ISER[0] |= 0x02000000;
}
