/*
 * password.h
 *
 *  Created on: Mar 2, 2024
 *      Author: Administrator
 */

#ifndef INC_PASSWORD_H_
#define INC_PASSWORD_H_


#include "stm32f7xx_hal.h"			// STM32F7 시리즈의 HAL 드라이버들을 사용하기 위한 헤더 파일
#include "buzzer.h"
#include "motor.h"
#include "USART.h"

// touchpad(start, check) port			  pin 위치
#define GPIO_check       GPIOA
#define GPIO_start_pin   GPIO_PIN_3 	// 2-1-1
#define GPIO_check_pin   GPIO_PIN_4 	// 3-1-(-2)

// touchpad(1,2,3,4) port				  pin 위치
#define GPIO_touch       GPIOB
#define GPIO_touch_1_pin GPIO_PIN_5 	// 3-1-(-4)
#define GPIO_touch_2_pin GPIO_PIN_6 	// 4-1-7
#define GPIO_touch_3_pin GPIO_PIN_8 	// 3-2-1
#define GPIO_touch_4_pin GPIO_PIN_9 	// 3-2-2

void Password(void);				// password
void inputPassword(void);			// input password
void confirmInputPassword(void);	// 입력된 비밀번호 확인
void comparePassword(void);			// compare password
void rightPassword(void);			// right password
void wrongPassword(void);			// wrong password
void HAL_Delay(uint32_t Delay);		// delay function

#endif /* INC_PASSWORD_H_ */
