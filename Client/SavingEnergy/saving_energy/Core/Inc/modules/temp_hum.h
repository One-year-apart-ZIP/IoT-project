/*
 * temp_hum.h
 *
 *  Created on: Mar 10, 2024
 *      Author: hojoon
 */

#ifndef INC_MODULES_TEMP_HUM_H_
#define INC_MODULES_TEMP_HUM_H_

#define JSON_FORMAT "{\"Temp\":%d,\"Hum\":%d,\"Gas\":%d,\"ac\":%d,\"bo\":%d,\"hu\":%d}\r\n"


void TempHum_Init(void);
void TempHum_Start(void);

#endif /* INC_MODULES_TEMP_HUM_H_ */
