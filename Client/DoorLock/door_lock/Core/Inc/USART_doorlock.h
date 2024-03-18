/*
 * USART_doorlock.h
 *
 *  Created on: Mar 12, 2024
 *      Author: Administrator
 */

#ifndef INC_USART_DOORLOCK_H_
#define INC_USART_DOORLOCK_H_

#include "stm32f7xx.h"				// STM32F7 시리즈 마이크로컨트롤러에 대한 전반적인 설정 및 특정 기능들에 대한 매크로 및 정의들을 포함
#include "stm32f7xx_hal_uart.h"   	// STM32F7 시리즈에서 UART와 관련된 HAL (Hardware Abstraction Layer) 함수들을 정의

//USART2 USART_B_TX:PD5(2-2-3)
//RaspberryPI RXD0:GPIO_15(pin10,2-5), GND:3,7,10

extern UART_HandleTypeDef huart2;

void MX_USART2_UART_Init(void);		// USART_B init
void USART_Transmit(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size); // 데이터 전송(TX)

#endif /* INC_USART_DOORLOCK_H_ */
