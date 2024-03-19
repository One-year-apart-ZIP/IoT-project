/*
 * Uart.c
 *
 *  Created on: Mar 12, 2024
 *      Author: hojoon
 */

#include "usart.h"
#include "stm32f767xx.h"
#include "helper.h"

volatile unsigned char RXD;

void USART6_IRQHandler(void)
{
	if (USART6->ISR & USART_ISR_RXNE)
	{
		RXD = USART6->RDR;

		if ((RXD >= 0x20) && (RXD <= 0x7F))
		{
			GPIOB->ODR = 0x00000080;
			Delay_ms(500);
			GPIOB->ODR = 0x00000000;
			Delay_ms(300);
		}
	}
}

void USART6_init(void)
{
	// USART6
	// PG14 -USART6 TX
	// PG9 - USART6 RX
	RCC->AHB1ENR |= 0x00000040;	//	GPIOG CLK ENABLE
	RCC->APB2ENR |= 0x00000020;	// USART6 CLK ENABLE

	GPIOG->MODER &= 0xCFF3FFFF;
	GPIOG->MODER |= 0x20080000;	// PG14, PG9 - ALTERNATE MODE
	GPIOG->AFR[1] = 0x08000080;	// AF8
	GPIOG->AFR[0] = 0x00000000;

	USART6->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART6->CR1 |= USART_CR1_RXNEIE;
	USART6->CR2 = 0x00000000;
	USART6->BRR = 10000;

	RXD = USART6->RDR;

	NVIC->ISER[2] = 0x00000080;	// SET USART6[39]
//	NVIC_EnableIRQ(USART6_IRQn);
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
