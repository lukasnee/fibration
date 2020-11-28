#include "system.hpp"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

#include "FreeRTOS.h"
#include "task.h"

#include <cstdio>

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

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

static void vStdioTask(void * pvParams)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	/* DMA interrupt init */
	/* DMA1_Channel4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	/* DMA1_Channel5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}

	/* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
	//setvbuf(stdout, NULL, _IONBF, 0);
	const size_t stdioBuffSize = 0x400; 
	void * stdioBuff = pvPortMalloc(stdioBuffSize);
	if(stdioBuff)
	{
		setvbuf(stdout, (char *)stdioBuff, _IOLBF, stdioBuffSize);
}
	for(;;)
	{
		vTaskDelay(20);
	}
	vPortFree(stdioBuff);	
}

void vMainTask(void *); // main application task

struct TaskCreateParams {
	TaskFunction_t pxTaskCode; 
	const char *const pcName;
	const uint16_t usStackDepth; 
	void *const pvParameters; 
	UBaseType_t uxPriority; 
	TaskHandle_t *const pxCreatedTask;
};

void System::createSystemTasks()
{
	TaskCreateParams arSystemTasks[] = 
	{
		{vStdioTask, "stdio", 0x200, NULL, 16, NULL},
		{vMainTask, "main", 0x200, NULL, 1, NULL},
	};

	for(TaskCreateParams &tc : arSystemTasks)
	{
		BaseType_t xReturned = xTaskCreate(
			tc.pxTaskCode, 
			tc.pcName, 
			tc.usStackDepth, 
			tc.pvParameters, 
			tc.uxPriority, 
			tc.pxCreatedTask);		
	
		if(xReturned == pdFAIL) 
		{
			Error_Handler();
			break;
		}
	}
}

void System::start()
{
	HAL_Init();
	SystemClock_Config();

 	System::createSystemTasks();

	vTaskStartScheduler();

	while(true); // should never reach
}

#include <cerrno>

#define STDIN_FILENO 	0
#define STDOUT_FILENO 	1
#define STDERR_FILENO 	2

extern "C" int _write(int fd, char* ptr, int len) {
  HAL_StatusTypeDef hstatus;

  if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
    hstatus = HAL_UART_Transmit(&huart1, (uint8_t *) ptr, len, HAL_MAX_DELAY);
    if (hstatus == HAL_OK)
      return len;
    else
      return EIO;
  }
  errno = EBADF;
  return -1;
}

extern "C" int _read(int fd, char* ptr, int len) {
  HAL_StatusTypeDef hstatus;

  if (fd == STDIN_FILENO) {
    hstatus = HAL_UART_Receive(&huart1, (uint8_t *) ptr, 1, HAL_MAX_DELAY);
    if (hstatus == HAL_OK)
      return 1;
    else
      return EIO;
  }
  errno = EBADF;
  return -1;
}

uint32_t System::getTick()
{
	return HAL_GetTick();
}

int main()
{  
	System::start(); // never returns
}