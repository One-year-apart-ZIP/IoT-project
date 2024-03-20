/*
 * tft_lcd.h
 *
 *  Created on: Mar 9, 2024
 *      Author: hojoon
 */

#ifndef INC_MODULES_TFT_LCD_H_
#define INC_MODULES_TFT_LCD_H_

#include "stm32f767xx.h"
//#include "stm32f7xx_hal.h"

#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define WHITE 0xFFFF
#define BLACK 0x0000

void TFT_LCD_Init(void);
void TFT_LCD_Command(unsigned short IR);
void TFT_LCD_Data(unsigned short DR);
void TFT_LCD_Write(unsigned short reg, unsigned short value);
void TFT_LCD_Color_screen(unsigned short start_x, unsigned short start_y, unsigned short width, unsigned short height, unsigned short color);
void TFT_LCD_GRAM_Set(unsigned short x, unsigned short y);

void TFT_LCD_xy(unsigned char x, unsigned char y);
void TFT_LCD_Color(unsigned short foreground, unsigned short background);
void TFT_LCD_Write_Pixel(unsigned short x, unsigned short y, unsigned short color);

void TFT_LCD_Landscape(void);

void TFT_LCD_String(unsigned char x, unsigned char y, unsigned short foreground, unsigned short background, char* str);
void TFT_LCD_English(unsigned char code);

void TFT_LCD_Unsigned_Decimal(unsigned int number, unsigned char zerofill, unsigned char digit);
void TFT_LCD_Signed_Decimal(int number, unsigned char zerofill, unsigned char digit);

#endif /* INC_MODULES_TFT_LCD_H_ */
