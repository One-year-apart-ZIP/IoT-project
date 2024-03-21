/*
 * led.c
 *
 *  Created on: Mar 19, 2024
 *      Author: hojoon
 */

#include "stm32f767xx.h"
#include "modules/led.h"

volatile unsigned char air_conditional_state = OFF;
volatile unsigned char humidifier_state = OFF;
volatile unsigned char boiler_state = OFF;

void LED_init(void)
{
	// PB0, PB1, PB2
	RCC->AHB1ENR |= 0x00000002;
	GPIOB->MODER &= 0xFFFFFFC0;
	GPIOB->MODER |= 0x00000015;
	GPIOB->ODR = 0x00000000;
}

void LED_on(unsigned char cmd)
{
	switch(cmd)
	{
	case AIR_CONDITIONAL:
		if(air_conditional_state != ON)
		{
			GPIOB->ODR |= 0x00000001;
			air_conditional_state = ON;
		}
		break;
	case HUMIDIFIER:
		if(humidifier_state != ON)
		{
			GPIOB->ODR |= 0x00000002;
			humidifier_state = ON;
		}

		break;
	case BOILER:
		if(boiler_state != ON)
		{
			GPIOB->ODR |= 0x00000004;
			boiler_state = ON;
		}

		break;
	default:
		break;
	}
}

void LED_off(unsigned char cmd)
{
	switch(cmd)
	{
	case AIR_CONDITIONAL:
		if(air_conditional_state != OFF)
		{
			GPIOB->ODR &= 0xFFFFFFFE;
			air_conditional_state = OFF;
		}
		break;
	case HUMIDIFIER:
		if(humidifier_state != OFF)
		{
			GPIOB->ODR &= 0xFFFFFFFD;
			humidifier_state = OFF;
		}
		break;
	case BOILER:
		if(boiler_state != OFF)
		{
			GPIOB->ODR &= 0xFFFFFFFB;
			boiler_state = OFF;
		}
		break;
	default:
		break;
	}
}
