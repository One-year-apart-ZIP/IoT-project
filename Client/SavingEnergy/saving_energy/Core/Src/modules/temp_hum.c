/*
 * temp_hum.c
 *
 *  Created on: Mar 10, 2024
 *      Author: hojoon
 */
#include "stm32f767xx.h"
#include "modules/tft_lcd.h"
#include "common/helper.h"

unsigned short adc1_channel10_result = 0;
unsigned short adc1_channel12_result = 0;

void TempHum_Init(void)
{
	TFT_LCD_Pin_Init();
	TFT_LCD_Init();

	RCC->APB2ENR |= 0x00004000;
	SYSCFG->CMPCR = 0x00000001;

	GPIOC->MODER |= 0x00000033;	// PC0, PC2 Analog mode
	RCC->APB2ENR |= 0x00000100;	// ADC1 Enable

	ADC->CCR = 0x00010000;

	ADC1->SMPR1 = 0x00000183;	// CHANNEL 10, 12 = 15CYCLE
	ADC1->SMPR2 = 0x00000000;

	ADC1->CR1 = 0x00000900;		// 12-BIT RESOLUTION, SET SCAN
	ADC1->CR2 = 0x00000401;		// SET EOCS, CONT, ADON

	ADC1->SQR1 = 0x00100000;	// REGULAR CHANNEL SEQUENCE LENGTH = 2
	ADC1->SQR2 = 0x00000000;
	ADC1->SQR3 = 0x0000018A;	// CH 10 -> 12

	TFT_LCD_Color_screen(0, 0, 320, 240, WHITE);
}

void TempHum_Start(void)
{
	TFT_LCD_Color_screen(6, 4, 100, 100, WHITE);

	ADC1->CR2 |= 0x40000000;

	while(!(ADC1->SR & 0x00000002));
	adc1_channel10_result = ADC1->DR;

	float temperature = ((float)adc1_channel10_result / 4095.) * 218.75 - 66.875;
	TFT_LCD_String(6, 4, BLACK, WHITE, "Temperature");
	TFT_LCD_xy(7, 8);
	TFT_LCD_Color(BLACK, WHITE);
	TFT_LCD_Signed_Float(temperature, 3, 2);

	ADC1->CR2 |= 0x40000000;

	while(!(ADC1->SR & 0x00000002));
	adc1_channel12_result = ADC1->DR;
	float humidity = ((float)adc1_channel12_result / 4095.) * 125. - 12.5;
	TFT_LCD_String(25, 4, BLACK, WHITE, "Humidity");
	TFT_LCD_xy(25, 8);
	TFT_LCD_Color(BLACK, WHITE);
	TFT_LCD_Signed_Float(humidity, 2, 2);

	Delay_ms(2000);
}
