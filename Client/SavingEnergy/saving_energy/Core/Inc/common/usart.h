/*
 * usart.h
 *
 *  Created on: Mar 14, 2024
 *      Author: hojoon
 */

#ifndef INC_COMMON_USART_H_
#define INC_COMMON_USART_H_

void USART6_init(void);
unsigned char USART6_receive_character(void);
void USART6_char(unsigned char data);
void USART6_string(unsigned char* string, unsigned int size);

#endif /* INC_COMMON_USART_H_ */
