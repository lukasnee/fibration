#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim7;

#define BLINK_TIME_MS 100
#define OFF_TIME_MS 1000

static void errorLedInit()
{
	static uint8_t errorLedInitialized = 0;
	if(!errorLedInitialized)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
		//__HAL_RCC_GPIOF_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct = { 0 };
		GPIO_InitStruct.Pin = GPIO_PIN_13;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // led off

		errorLedInitialized = 1;
	}
}

static void blinkErrorLed(uint8_t numberOfTimes)
{
	errorLedInit();
	while(numberOfTimes--)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(BLINK_TIME_MS);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(BLINK_TIME_MS);
	}
	HAL_Delay(OFF_TIME_MS);
};

enum TimesToBlinkOnFault
{
	TIMES_TO_BLINK_ON_HARD_FAULT = 1,
	TIMES_TO_BLINK_ON_MEM_FAULT,
	TIMES_TO_BLINK_ON_BUS_FAULT,
	TIMES_TO_BLINK_ON_USAGE_FAULT,
};

// This function handles Non maskable interrupt.
void NMI_Handler(void)
{
}

// This function handles Hard fault interrupt.
void HardFault_Handler(void)
{
	while (1)
	{
		blinkErrorLed(TIMES_TO_BLINK_ON_HARD_FAULT);
	}
}

// This function handles Memory management fault.
void MemManage_Handler(void)
{
	while (1)
	{
		blinkErrorLed(TIMES_TO_BLINK_ON_MEM_FAULT);
	}
}

// This function handles Pre-fetch fault, memory access fault.
void BusFault_Handler(void)
{
	while (1)
	{
		blinkErrorLed(TIMES_TO_BLINK_ON_BUS_FAULT);
	}
}

// This function handles Undefined instruction or illegal state.
void UsageFault_Handler(void)
{
	while (1)
	{
		blinkErrorLed(TIMES_TO_BLINK_ON_USAGE_FAULT);
	}
}

// This function handles Debug monitor.
void DebugMon_Handler(void)
{
}

void DMA1_Channel4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

void DMA1_Channel5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1);
}

void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim7);
}