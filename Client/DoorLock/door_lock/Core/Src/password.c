/*
 * password.c
 *
 *  Created on: Mar 2, 2024
 *      Author: Administrator
 */

#include "password.h"

// 인터럽트 flag 선언 및 초기화
int flag_start = 0;		// 시작(터치) 인터럽트 flag
int flag_touch1 = 0;	// 1(터치) 인터럽트 flag
int flag_touch2 = 0;	// 2(터치) 인터럽트 flag
int flag_touch3 = 0;	// 3(터치) 인터럽트 flag
int flag_touch4 = 0;	// 4(터치) 인터럽트 flag
int flag_check = 0;		// 비밀번호 입력 완료(터치) 인터럽트 flag

// 비밀번호
int input_password[4] = {0,0,0,0};	// 입력 받을 비밀번호 배열 초기화
int password[4] = {2, 2, 2, 2};			// 비밀번호 설정
int cnt_input = 0;						// 비밀번호 입력 자릿수
int cnt_wrong = 0;						// 비밀번호 틀린 횟수
int pwMatch = 1;						// 비밀번호 일치 여부 확인용 변수(일치:1, 불일치:0)


// password
void Password(void){
	inputPassword();		// 비밀번호 입력
	confirmInputPassword();	// 비밀번호 일치 여부 확인
}


// input password
void inputPassword(void){
	if(flag_start == 1){// 비밀번호 입력 시작 터치

		if (flag_touch1 == 1){			// 1번(터치)
				flag_touch1 = 0; 				// 인터럽트 플래그 초기화
				input_password[cnt_input] = 1;	// 배열에 비밀번호 입력
				cnt_input++;					// 비밀번호 입력 자릿수 증가
		}
		else if (flag_touch2 == 1){		// 2번(터치)
				flag_touch2 = 0;
				input_password[cnt_input] = 2;
				cnt_input++;
		}
		else if (flag_touch3 == 1){ 	// 3번(터치)
			 	flag_touch3 = 0;
			  	input_password[cnt_input] = 3;
			  	cnt_input++;
		}
		else if (flag_touch4 == 1){		// 4번(터치)
			  	flag_touch4 = 0;
			  	input_password[cnt_input] = 4;
			  	cnt_input++;
		}
	}
}


// 입력된 비밀번호 확인
void confirmInputPassword(void){
	if (flag_check == 1){//비밀번호 입력 완료 터치
		if (cnt_input == 4){// 4자리 입력한 경우
			comparePassword();		// compare password. password 틀리면 flag=0.
			if (pwMatch == 1) {//(전체)비밀번호 일치
				rightPassword();	// O부저 울림, O모터 회전
			}
			flag_check = 0;			// 비밀번호 입력 완료 터치 flag 초기화
		}
		else{//4자리 입력이 아닌 경우
			cnt_wrong++;				// 비밀번호 틀린 횟수 증가
			buzzer_X();					// X부저 울림(1.5초)
		}
		cnt_input=0;		// 비밀번호 입력 자릿수 초기화
		if(cnt_wrong == 5){// 비밀번호 5회 틀림
				cnt_wrong=0;		// 비밀번호 틀린 횟수 초기화
				buzzer_X();			// X부저 울림(1.5초)
				motor_X();			// motor -90도 회전
				/*주인 얼굴 없으면
				 log & camera 영상 저장
				*/
			}
	}

	flag_check = 0;	// 비밀번호 입력 완료 터치 flag 초기화
}


// compare password
void comparePassword(void){
	for (int i=0;i<4;i++){// 비밀번호 4자리 각각 일치하는지 확인
		if (password[i] == input_password[i]) {// 해당 배열 비밀번호 일치
			pwMatch = 1;	// 비밀번호 결과 일치 표시
			//continue;
		}
		else{// 해당 배열 비밀번호 불일치 -> X부저 울림
			wrongPassword();
			break;
		}
	}
}

// right password
void rightPassword(void){
	cnt_wrong = 0;		// 비밀번호 틀린 횟수 초기화
	buzzer_O();			// O부저 울림(0.5초)
	motor_O();			// O모터 회전(+90도->0도)
}

// wrong password
void wrongPassword(void){
	pwMatch = 0;			// 비밀번호 결과 불일치 표시
	cnt_wrong++;			// 비밀번호 틀린 횟수 증가
	buzzer_X();				// X부저 울림(1.5초)
}


/* external interrupt callback function
 *
 * GPIO(General Purpose Input/Output) 인터럽트의 콜백 함수.
 * 이 함수는 GPIO 핀의 외부 인터럽트(EXTI)가 발생할 때 호출됨.
 * 	GPIO_PIN은 인터럽트가 발생한 GPIO 핀을 식별하는 데 사용됨.
 * 	일반적으로 이 함수는 인터럽트 핸들러로 사용(GPIO 핀의 상태 변화를 감지, 필요한 작업을 수행).
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){ // interrupt flag
	if (GPIO_PIN == GPIO_start_pin) flag_start = 1;		// start touch
	if (GPIO_PIN == GPIO_touch_1_pin) flag_touch1 = 1;	// 1 touch
	if (GPIO_PIN == GPIO_touch_2_pin) flag_touch2 = 1;	// 2 touch
	if (GPIO_PIN == GPIO_touch_3_pin) flag_touch3 = 1;	// 3 touch
	if (GPIO_PIN == GPIO_touch_4_pin) flag_touch4 = 1;	// 4 touch
	if (GPIO_PIN == GPIO_check_pin)	flag_check = 1;		// pw input finish touch
}


// Delay function
void HAL_Delay(uint32_t Delay){
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;
  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY){
    wait += (uint32_t)(uwTickFreq);
  }
  while ((HAL_GetTick() - tickstart) < wait){
  }
}
