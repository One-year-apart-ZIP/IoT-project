/*
 * buzzer.c
 *
 *  Created on: Mar 11, 2024
 *      Author: Administrator
 */

#include "buzzer.h"


// right password buzzer(0.5sec)
void buzzer_O(void){
	// buzzer on
	HAL_GPIO_WritePin(GPIO_buzzer, GPIO_buzzer_pin, GPIO_PIN_SET);
	HAL_Delay(500);

	// buzzer off
	HAL_GPIO_WritePin(GPIO_buzzer, GPIO_buzzer_pin, GPIO_PIN_RESET);
	HAL_Delay(500);
}


// wrong password buzzer(1.5sec)
void buzzer_X(void){
	// buzzer on
	HAL_GPIO_WritePin(GPIO_buzzer, GPIO_buzzer_pin, GPIO_PIN_SET);
	HAL_Delay(1500);

	// buzzer off
	HAL_GPIO_WritePin(GPIO_buzzer, GPIO_buzzer_pin, GPIO_PIN_RESET);
	HAL_Delay(1500);
}