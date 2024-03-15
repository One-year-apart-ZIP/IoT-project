/*
 * motor.c
 *
 *  Created on: Mar 11, 2024
 *      Author: Administrator
 */

#include "motor.h"
TIM_HandleTypeDef htim5;


// right password motor(+90 -> +0)
void motor_O(void){
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	htim5.Instance->CCR1 = 75;		// motor +90도 회전
	HAL_Delay(4000);				// 5초 후
	htim5.Instance->CCR1 = 12;		// motor 0도 회전(원위치)
	HAL_Delay(2000);
	HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_1);
}


// wrong password motor(-90)
void motor_X(void){
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	htim5.Instance->CCR1 = 130;		// motor -90도 회전
	HAL_Delay(2000);
	HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_1);
}


void MX_TIM5_Init(void){
  /* USER CODE BEGIN TIM5_Init 0 */
  /* USER CODE END TIM5_Init 0 */
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */
  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 900-1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 1000-1;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */
  /* USER CODE END TIM5_Init 2 */
  HAL_TIM_MspPostInit(&htim5);
}