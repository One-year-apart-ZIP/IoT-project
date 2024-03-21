/*
 * Uart.c
 *
 *  Created on: Mar 12, 2024
 *      Author: hojoon
 */

#include "stm32f767xx.h"

#include "common/usart.h"
#include "common/helper.h"

volatile unsigned char RXD;
volatile unsigned char command;
volatile unsigned char rx_flag;

void USART6_IRQHandler(void)
{
	if (USART6->ISR & USART_ISR_RXNE)
	{
		RXD = USART6->RDR;

		if((RXD >= 0x20) && (RXD <= 0x7F))
		{
			rx_flag = 1;
			command = RXD;
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
	USART6->CR3 = 0x00000000;
	USART6->BRR = 2813;

	Delay_ms(1);

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

void USART6_string(char* string, unsigned int size)
{
	unsigned int i = 0;
	for(i = 0; i < size; ++i)
	{
		if(string[i] == '\0')
			break;
		USART6_char(string[i]);
	}
}
