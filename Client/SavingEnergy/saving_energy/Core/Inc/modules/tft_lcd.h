/*
 * tft_lcd.h
 *
 *  Created on: Mar 9, 2024
 *      Author: hojoon
 */

#ifndef INC_MODULES_TFT_LCD_H_
#define INC_MODULES_TFT_LCD_H_

#include "stm32f767xx.h"
#include "stm32f7xx_hal.h"

#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define WHITE 0xFFFF
#define BLACK 0x0000

void TFT_LCD_Pin_Init(void);
void TFT_LCD_Init(void);
void TFT_LCD_Command(unsigned short IR);
void TFT_LCD_Data(unsigned short DR);
void TFT_LCD_Write(unsigned short reg, unsigned short value);
void TFT_LCD_Color_screen(unsigned short color);
void TFT_LCD_GRAM_Set(unsigned short x, unsigned short y);

void TFT_LCD_xy(unsigned char x, unsigned char y);
void TFT_LCD_Color(unsigned short foreground, unsigned short background);
void TFT_LCD_Write_Pixel(unsigned short x, unsigned short y, unsigned short color);

void TFT_LCD_Landscape(void);

void TFT_LCD_String(unsigned char x, unsigned char y, unsigned short foreground, unsigned short background, unsigned char* str);
void TFT_LCD_English(unsigned char code);
void TFT_LCD_English_Pixel(unsigned short xPixel, unsigned short yPixel, unsigned char code);

void TFT_LCD_Signed_Decimal(int number, unsigned char zerofill, unsigned char digit);
void TFT_LCD_Signed_Float(float number, unsigned char integral, unsigned char fractional);

#endif /* INC_MODULES_TFT_LCD_H_ */
