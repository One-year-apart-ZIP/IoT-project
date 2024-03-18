/*
 * motor.h
 *
 *  Created on: Mar 11, 2024
 *      Author: Administrator
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "stm32f7xx.h"			// STM32F7 시리즈 마이크로컨트롤러에 대한 전반적인 설정 및 특정 기능들에 대한 매크로 및 정의들을 포함
#include "stm32f7xx_hal_tim.h"	// STM32F7 시리즈에서 타이머와 관련된 HAL (Hardware Abstraction Layer) 함수들을 정의

extern TIM_HandleTypeDef htim5;	// 모터 타이머 관리하는 핸들러 구조체 선언.
/*
 * TIM_HandleTypeDef은 STM32에서 타이머(TIM)를 관리하기 위한 핸들러 구조체.
 * 이 구조체는 HAL (Hardware Abstraction Layer) 라이브러리에서 사용되며,
 * 타이머를 초기화하고 제어하는 데 필요한 정보와 기능을 제공한다.
 * PWM (펄스 폭 변조) 신호를 생성하고 관리하는 데 중요한 역할.
 * 이 구조체의 주요 멤버들로는:
 * 	Instance: 사용할 타이머 인스턴스를 가리킨다.
 * 	Init: 타이머 초기화 구성 정보를 포함한다.
 * 	Lock: 동시에 여러 함수가 접근하지 못하도록 보호하기 위한 Lock 변수.
 * 	State: 타이머의 현재 상태를 나타낸다.
 */

// servo motor pin: PA0 		pin위치: 4-1-(-3)
void MX_TIM5_Init(void);		// servo motor init timer function
void motor_O(void);				// right password motor(+90 -> +0)
void motor_X(void);				// wrong password motor(-90)

#endif /* INC_MOTOR_H_ */