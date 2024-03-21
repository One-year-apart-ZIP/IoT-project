/*
 * led.h
 *
 *  Created on: Mar 19, 2024
 *      Author: hojoon
 */

#ifndef INC_MODULES_LED_H_
#define INC_MODULES_LED_H_

#define AIR_CONDITIONAL	0
#define HUMIDIFIER		2
#define BOILER			4

#define OFF	0
#define ON	1

extern volatile unsigned char air_conditional_state;
extern volatile unsigned char humidifier_state;
extern volatile unsigned char boiler_state;

extern volatile unsigned char ac_remote_mode;
extern volatile unsigned char hum_remote_mode;
extern volatile unsigned char boiler_remote_mode;

void LED_init(void);
void LED_on(unsigned char cmd);
void LED_off(unsigned char cmd);

#endif /* INC_MODULES_LED_H_ */
