/*
 * usart.c
 *
 *  Created on: Mar 14, 2024
 *      Author: hojoon
 */

#include "stm32f767xx.h"
#include "common/helper.h"
#include "common/usart.h"

volatile unsigned short RXD;

void USART6_init(void)
{
	// USART6
	// PG14 -USART6 TX
	// PG9 - USART6 RX

	RCC->AHB1ENR |= 0x00000040;	//	GPIOG CLK ENABLE
	GPIOG->MODER &= 0xCFF3FFFF;
	GPIOG->MODER |= 0x20080000;	// PG14, PG9 - ALTERNATE MODE

	GPIOG->AFR[1] = 0x08000080;	// AF8
	GPIOG->AFR[0] = 0x00000000;

	RCC->APB2ENR |= 0x00000020;	// USART6 CLK ENABLE

	USART6->CR1 = 0x0000002D;	// SET TE, RE, UE, RXENIE
	USART6->CR2 = 0x00000000;
	USART6->CR2 = 0x00000000;
	USART6->BRR = 10000;

	Delay_ms(1);

	RXD = USART6->RDR;

	// NVIC->
}

unsigned char USART6_receive_character(void)
{
	// Check RXNE
	if(USART6->ISR & 0x00000020)
		return USART6->RDR;
	else
		return 0x00;
}

void USART6_char(unsigned char data)
{
	// Check TXE
	while(!(USART6->ISR & 0x00000080));
	USART6->TDR = data;
}

void USART6_string(unsigned char* string, unsigned int size)
{
	unsigned int i = 0;
	for(i = 0; i < size; ++i)
	{
		USART6_char(string[i]);
	}
}
