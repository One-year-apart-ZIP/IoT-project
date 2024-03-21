/*
 * temp_hum.c
 *
 *  Created on: Mar 10, 2024
 *      Author: hojoon
 */
#include "stm32f767xx.h"
#include <stdio.h>

#include "modules/temp_hum.h"
#include "modules/tft_lcd.h"
#include "modules/buzzer.h"
#include "modules/led.h"

#include "common/usart.h"

volatile unsigned short adc1_channel10_result = 0;
volatile unsigned short adc1_channel12_result = 0;
volatile unsigned short adc1_channel13_result = 0;

volatile unsigned char ac_remote_mode = 0;
volatile unsigned char hum_remote_mode = 0;
volatile unsigned char boiler_remote_mode = 0;

void TempHum_Init(void)
{
	TFT_LCD_Init();

	// PC0 - ADC1_IN10
	// PC2 - ADC1_IN12
	// PC3 - ADC1_IN13

	RCC->APB2ENR |= 0x00004000;
	SYSCFG->CMPCR = 0x00000001;

	GPIOC->MODER |= 0x000000F3;	// PC0, PC2, PC3 Analog mode
	RCC->APB2ENR |= 0x00000100;	// ADC1 Enable

	ADC->CCR = 0x00010000;

	ADC1->SMPR1 = 0x000006C3;	// CHANNEL 10, 12, 13 = 56 CYCLE
	ADC1->SMPR2 = 0x00000000;

	ADC1->CR1 = 0x00000900;		// 12-BIT RESOLUTION, SET SCAN
	ADC1->CR2 = 0x00000401;		// SET EOCS, CONT, ADON

	ADC1->SQR1 = 0x00200000;	// REGULAR CHANNEL SEQUENCE LENGTH = 3
	ADC1->SQR2 = 0x00000000;
	ADC1->SQR3 = 0x0000358A;	// CH 10 -> 12 -> 13

	TFT_LCD_Color_screen(0, 0, 320, 240, WHITE);
}

void TempHum_Start(void)
{
	char buffer[70];

	TFT_LCD_Color_screen(6, 4, 100, 100, WHITE);

	// TEMPERATURE
	ADC1->CR2 |= 0x40000000;

	while(!(ADC1->SR & 0x00000002));
	adc1_channel10_result = ADC1->DR;

	float temperature = ((float)adc1_channel10_result * 217.75 / 4095.) - 66.875;
	TFT_LCD_String(4, 6, BLACK, WHITE, "Temperature");
	TFT_LCD_xy(17, 6);
	if(temperature > 30.)
		TFT_LCD_Color(RED, WHITE);
	else
		TFT_LCD_Color(BLACK, WHITE);

	TFT_LCD_Signed_Decimal((int)temperature, 1, 2);
	TFT_LCD_String(20, 6, BLACK, WHITE, "`C");

	// HUMIDITY
	ADC1->CR2 |= 0x40000000;

	while(!(ADC1->SR & 0x00000002));
	adc1_channel12_result = ADC1->DR;
	float humidity = ((float)adc1_channel12_result *100. / 4095.);
	TFT_LCD_String(4, 20, BLACK, WHITE, "Humidity");
	TFT_LCD_xy(17, 20);
	if(humidity > 70.)
		TFT_LCD_Color(BLUE, WHITE);
	else
		TFT_LCD_Color(BLACK, WHITE);

	TFT_LCD_Signed_Decimal((int)humidity, 1, 2);
	TFT_LCD_String(20, 20, BLACK, WHITE, "%");

	// GAS SENSOR
	ADC1->CR2 |= 0x40000000;

	while(!(ADC1->SR & 0x00000002));
	adc1_channel13_result = ADC1->DR;

	unsigned int ppm = adc1_channel13_result;
	if(ppm > 1500)
	{
		Buzzer_Init();
		Delay_ms(10);
		Buzzer_Start(500);
		Buzzer_Stop(100);
	}

	if(temperature >= 30. && ac_remote_mode == 0) { LED_on(AIR_CONDITIONAL); }
	else if(temperature <= 26. && ac_remote_mode == 0) { LED_off(AIR_CONDITIONAL); }

	if(humidity >= 80 && hum_remote_mode == 0) { LED_on(HUMIDIFIER); }
	else if(humidity <= 60 && hum_remote_mode == 0) { LED_off(HUMIDIFIER); }

	sprintf(buffer, JSON_FORMAT,
			(int)temperature, (int)humidity, ppm, air_conditional_state, humidifier_state, boiler_state);

	USART6_string(buffer, sizeof(buffer));
}
