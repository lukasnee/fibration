#include "uart1.hpp"
#include "system.hpp"

#include "stm32f3xx_hal.h"

DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;
UART_HandleTypeDef huart1;

static void sInitUartDma1(std::uint32_t baudrate)
{
	__HAL_RCC_DMA1_CLK_ENABLE();
	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

	huart1.Instance = USART1;
	huart1.Init.BaudRate = baudrate;
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
		FibSys::error();
	}
}

Uart1::Uart1(uint32_t baudrate) :
	DmaQueue(), baudrate(baudrate)
{
	sInitUartDma1(this->baudrate);
}

bool Uart1::transmitDma(std::uint8_t *pData, std::uint16_t size, DmaCallbacks &dmaCallbacks)
{
	bool retval = false;
	
	this->dmaCallbacks = &dmaCallbacks; // todo maybe check if cbs not null.

	HAL_StatusTypeDef status; 
	status = HAL_UART_Transmit_DMA(&huart1, pData, size);
	if(status == HAL_OK) 
	{
		retval = true;
	}
	
	return retval;
}

bool Uart1::receiveDma(std::uint8_t *pData, std::uint16_t size, DmaCallbacks &dmaCallbacks)
{
	bool retval = false;

	this->dmaCallbacks = &dmaCallbacks;

	HAL_StatusTypeDef status;
	status = HAL_UART_Receive_DMA(&huart1, pData, size);
	if(status == HAL_OK) 
	{
		retval = true;
	}

	return retval;
}

static void sDeinitUartDma1()
{
	HAL_UART_DeInit(&huart1);
	HAL_NVIC_DisableIRQ(DMA1_Channel4_IRQn);
	HAL_NVIC_DisableIRQ(DMA1_Channel5_IRQn);
}

Uart1::~Uart1()
{
	sDeinitUartDma1();
}
