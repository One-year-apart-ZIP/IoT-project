/*
 * password.c
 *
 *  Created on: Mar 2, 2024
 *      Author: Administrator
 */

#include "password.h"

// 인터럽트 flag 선언 및 초기화
unsigned char touch_value = 0;
unsigned char doorlock_status = IDLE;

// 비밀번호
unsigned char input_password[16] = {0,};	// 입력 받을 비밀번호 배열 초기화	5개 이상 받으면 에러나는거 아닌가?
const unsigned char password[4] = {1,1,2,3};		// 비밀번호 설정
unsigned char cnt_input = 0;// 비밀번호 입력 자릿수
unsigned char cnt_wrong = 0;// 비밀번호 틀린 횟수

void password_handle(void)
{
	switch(doorlock_status)
	{
	case IDLE:
		break;

	case PREPARE_INPUT:
		doorlock_status = INPUT_PASSWORD;
		cnt_input = 0;
		touch_value = 0;
		clear_password();
		break;

	case INPUT_PASSWORD:
		if (touch_value > 0)
		{
			input_password[cnt_input] = touch_value;
			touch_value = 0;
			cnt_input++;
		}
		break;

	case CHECKING_PASSWORD:
		unsigned char result = checking_password();
		if(result == PASSWORD_INCORRECT)
		{
			wrongPassword();
			if (cnt_wrong == 5)
			{
				cnt_wrong = 0;
				capture();
			}
		}
		else if(result == PASSWORD_CORRECT)
		{
			rightPassword();
		}
		clear_password();
		doorlock_status = IDLE;
		break;

	default:
		break;
	}
}

void capture(void)
{
	buzzer_X();			// X부저 울림(1.5초)
	motor_X();			// motor -90도 회전

	// 비밀번호 5회 틀렸으니 영상 캡쳐하라고 라즈베리파이(카메라연결)에 데이터 전송
	USART_Transmit(&huart2, (uint8_t *)"Capture video!", 14);
	HAL_Delay(1000);	// 안정적인 USART 데이터 전송을 위한 딜레이
}

unsigned char checking_password(void)
{
	unsigned char i = 0;

	if(cnt_input != 4)
		return PASSWORD_INCORRECT;

	for(i = 0; i < 4; ++i)
	{
		if(input_password[i] != password[i])
			return PASSWORD_INCORRECT;
	}

	return PASSWORD_CORRECT;
}

// right password
void rightPassword(void){
	cnt_wrong = 0;		// 비밀번호 틀린 횟수 초기화
	buzzer_O();			// O부저 울림(0.5초)
	motor_O();			// O모터 회전(+90도->0도)
}

// wrong password
void wrongPassword(void){
	cnt_wrong++;			// 비밀번호 틀린 횟수 증가
	buzzer_X();				// X부저 울림(1.5초)
}

void clear_password(void)
{
	for(int i=0; i<16; i++)
	{
		input_password[i] = 0;
	}
	cnt_input = 0;
}

/* external interrupt callback function
 *
 * GPIO(General Purpose Input/Output) 인터럽트의 콜백 함수.
 * 이 함수는 GPIO 핀의 외부 인터럽트(EXTI)가 발생할 때 호출됨.
 * 	GPIO_PIN은 인터럽트가 발생한 GPIO 핀을 식별하는 데 사용됨.
 * 	일반적으로 이 함수는 인터럽트 핸들러로 사용(GPIO 핀의 상태 변화를 감지, 필요한 작업을 수행).
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){ // interrupt flag
	if (GPIO_PIN == GPIO_start_pin) 	doorlock_status = PREPARE_INPUT;		// start touch
	if (GPIO_PIN == GPIO_touch_1_pin)
	{
		if (doorlock_status != IDLE && doorlock_status == INPUT_PASSWORD)
			touch_value = TOUCH_1;
	}
	if (GPIO_PIN == GPIO_touch_2_pin)
	{
		if (doorlock_status != IDLE && doorlock_status == INPUT_PASSWORD)
			touch_value = TOUCH_2;
	}
	if (GPIO_PIN == GPIO_touch_3_pin)
	{
		if (doorlock_status != IDLE && doorlock_status == INPUT_PASSWORD)
			touch_value = TOUCH_3;
	}
	if (GPIO_PIN == GPIO_touch_4_pin)
	{
		if (doorlock_status != IDLE && doorlock_status == INPUT_PASSWORD)
			touch_value = TOUCH_4;
	}
	if (GPIO_PIN == GPIO_check_pin)		doorlock_status = CHECKING_PASSWORD;		// pw input finish touch
}
