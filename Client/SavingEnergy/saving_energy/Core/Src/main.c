#include "main.h"

#include "common/helper.h"
#include "common/timer.h"
#include "common/usart.h"

#include "modules/led.h"
#include "modules/temp_hum.h"

void MCU_init(void);
void SystemClock_Config(void);

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MCU_init();
	LED_init();
	TIM1_init();
	TempHum_Init();
	USART6_init();

	while (1)
	{
		if(adc_flag == 1)
		{
			adc_flag = 0;
			TempHum_Start();
		}

		if(rx_flag == 1)
		{
			switch(command)
			{
			case AIR_CONDITIONAL_ON:
				LED_on(AIR_CONDITIONAL);
				break;
			case AIR_CONDITIONAL_OFF:
				LED_off(AIR_CONDITIONAL);
				break;
			case HUMIDIFIER_ON:
				LED_on(HUMIDIFIER);
				break;
			case HUMIDIFIER_OFF:
				LED_off(HUMIDIFIER);
				break;
			case BOILER_ON:
				LED_on(BOILER);
				break;
			case BOILER_OFF:
				LED_off(BOILER);
				break;
			default:
				break;
			}
		}
	}
}

void MCU_init(void)
{
	SCB_EnableICache();				// enable L1 instruction cache
	SCB_EnableDCache();				// enable L1 data cache

	FLASH->ACR = 0x00000307;

	RCC->CR |= 0x00010001;
	while((RCC->CR & 0x00000002) == 0);
	RCC->CFGR = 0x00000000;
	while((RCC->CFGR & 0x0000000C) != 0);

	RCC->CR = 0x00010001;
	RCC->PLLCFGR = 0x09403608;
	RCC->CR = 0x01010001;
	while((RCC->CR & 0x02000000) == 0);

	RCC->APB1ENR |= 0x10000000;
	PWR->CR1 |= 0x00010000;
	while((PWR->CSR1 & 0x00010000) == 0);
	PWR->CR1 |= 0x00020000;
	while((PWR->CSR1 & 0x00020000) == 0);

	RCC->CFGR = 0x3040B402;
	while((RCC->CFGR & 0x0000000C) != 0x00000008);
	RCC->CR |= 0x00080000;

	RCC->APB2ENR |= 0x00004000;
	SYSCFG->CMPCR = 0x00000001;

	RCC->AHB1ENR |= 0x0000001F;
	RCC->AHB2ENR |= 0x00000000;

	GPIOC->MODER = 0x00150000;
	GPIOC->ODR = 0x00000000;
	GPIOC->OSPEEDR = 0x00550000;

	GPIOD->MODER  = 0x55005555;
	GPIOD->ODR = 0x00000000;
	GPIOD->OSPEEDR = 0x55005555;
}
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 96;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	RCC_OscInitStruct.PLL.PLLR = 2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	}

	/** Activate the Over-Drive mode
	 */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK)
	{
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
	{
	}
}
