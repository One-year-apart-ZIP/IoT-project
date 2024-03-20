/*
 * usart.h
 *
 *  Created on: Mar 14, 2024
 *      Author: hojoon
 */

#ifndef INC_COMMON_USART_H_
#define INC_COMMON_USART_H_

#include "common/helper.h"

#define AIR_CONDITIONAL_ON	'0'
#define AIR_CONDITIONAL_OFF	'1'
#define BOILER_ON		'2'
#define BOILER_OFF		'3'
#define HUMIDIFIER_ON			'4'
#define HUMIDIFIER_OFF			'5'

extern volatile unsigned char command;
extern volatile unsigned char rx_flag;

void USART6_IRQHandler(void);
void USART6_init(void);
void USART6_char(unsigned char data);
void USART6_string(char* string, unsigned int size);

#endif /* INC_COMMON_USART_H_ */
