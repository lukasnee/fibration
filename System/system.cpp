#include "system.hpp"

#include <cstdint>
#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

#include "FreeRTOS.h"
#include "task.h"

static void Error_Handler()
{
	UsageFault_Handler();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM7)
	{
		HAL_IncTick();
	}
}

static void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/* Initializes the RCC Oscillators according to the specified parameters
	in the RCC_OscInitTypeDef structure. */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/* Initializes the CPU, AHB and APB buses clocks */
	RCC_ClkInitStruct.ClockType =
		RCC_CLOCKTYPE_HCLK |
		RCC_CLOCKTYPE_SYSCLK |
		RCC_CLOCKTYPE_PCLK1 |
		RCC_CLOCKTYPE_PCLK2;

	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}
}

TaskHandle_t System::hMainTask = NULL;

void System::hardwareInit()
{
	HAL_Init();
	SystemClock_Config();
}

uint32_t System::getTick()
{
	return HAL_GetTick();
}

void System::createTask(UBaseType_t priority)
{
	BaseType_t xReturned = xTaskCreate(vMainTask, 
		"main", 0x200, NULL, 1, &hMainTask);		

	if(xReturned == pdFAIL) 
	{
		Error_Handler();
	}
}

void System::run()
{
	const UBaseType_t cSysPriority = configMAX_PRIORITIES - 1;
	System::hardwareInit();
 	System::createTask(cSysPriority);
	
	vTaskStartScheduler();
	while(true); // should never be reached
}

void System::stop()
{
	vTaskDelete(hMainTask);
}

void System::error()
{
	HardFault_Handler(); // todo something better...
}