#include <cstdint>

#include "FreeRTOS.h"
#include "task.h"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

#include "gpio.hpp"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

static void Error_Handler()
{
}

static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
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
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

void vBlinkyTask(void * pvParams)
{
	
	Hardware::GPIO led(Hardware::PIN_LED,
					   false,
					   Hardware::PIN_MODE_OUTPUT,
					   Hardware::PIN_PULL_NONE);

	while(true)
	{
		vTaskDelay(100);
		led.toggle();
		vTaskDelay(1000-100);
	}
}

int main()
{  
	HAL_Init();
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	SystemClock_Config();

	Hardware::GPIO led(Hardware::PIN_LED,
					   false,
					   Hardware::PIN_MODE_OUTPUT,
					   Hardware::PIN_PULL_NONE);

	led.digitalWrite(false);
	HAL_Delay(100);
	led.digitalWrite(true);
	HAL_Delay(900);
	led.digitalWrite(false);
	HAL_Delay(100);
	led.digitalWrite(true);
	HAL_Delay(900);
		
	BaseType_t xReturned;
	TaskHandle_t xHandle = NULL;
	xReturned = xTaskCreate(vBlinkyTask,
							"blinky",
							0x200,
							NULL,
							tskIDLE_PRIORITY + 1,
							&xHandle);
	if(xReturned == pdPASS)
    { 
		vTaskStartScheduler();
	}
	else
	{
		vTaskDelete(xHandle);
	}
	while(true); // should never reach
}
