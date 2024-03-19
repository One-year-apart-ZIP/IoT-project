/*
 * timer.h
 *
 *  Created on: Mar 16, 2024
 *      Author: hojoon
 */

#ifndef INC_COMMON_TIMER_H_
#define INC_COMMON_TIMER_H_

extern volatile unsigned char interrupt_flag;
extern volatile unsigned char led_flag;

void TIM1_UP_TIM10_IRQHandler(void);
void TIM1_init(void);

#endif /* INC_COMMON_TIMER_H_ */
