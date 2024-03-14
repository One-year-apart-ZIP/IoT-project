/*
 * buzzer.c
 *
 *  Created on: Mar 14, 2024
 *      Author: hojoon
 */

#include "stm32f767xx.h"

#include "modules/buzzer.h"
#include "common/helper.h"

void Buzzer_Init(void)
{
	// PA3 to buzzer
	RCC->AHB1ENR |= 0x00000001;	// GPIOA CLK ENABLE
	GPIOA->MODER |= 0x00000040;	// PA3 OUTPUT MODE
}

void Buzzer_Start(unsigned int delay)
{
	GPIOA->ODR = 0x00000008;
	Delay_ms(delay);
}

void Buzzer_Stop(unsigned int delay)
{
	GPIOA->ODR = 0x00000000;
	Delay_ms(delay);
}
