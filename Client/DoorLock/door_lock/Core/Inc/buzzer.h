/*
 * buzzer.h
 *
 *  Created on: Mar 11, 2024
 *      Author: Administrator
 */

#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

#include "stm32f7xx.h"			// STM32F7 시리즈 마이크로컨트롤러에 대한 전반적인 설정 및 특정 기능들에 대한 매크로 및 정의들을 포함

// buzzer port							  pin 위치
#define GPIO_buzzer      GPIOB
#define GPIO_buzzer_pin  GPIO_PIN_15 	// 3-1-2

void buzzer_O(void); 			// right password buzzer
void buzzer_X(void); 			// wrong password buzzer

#endif /* INC_BUZZER_H_ */