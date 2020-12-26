#include "uart.hpp"
#include "system.hpp"

#include "stm32f3xx_hal.h"

DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;
UART_HandleTypeDef huart1;

static void MX_DMA_Init(uint8_t no)
{
	if(no == 1)
	{
		__HAL_RCC_DMA1_CLK_ENABLE();
		HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
		HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	}
}

static void MX_USART1_UART_Init(uint8_t no, uint32_t baudrate)
{
	if(no == 1)
	{
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
}

Uart::Uart(uint8_t no, uint32_t baudrate) :
	DmaQueue(), no(no), baudrate(baudrate)
{
	MX_DMA_Init(no);
	MX_USART1_UART_Init(no, baudrate);
}

Uart::~Uart()
{
	if(no == 1)
	{
		HAL_UART_DeInit(&huart1);
		HAL_NVIC_DisableIRQ(DMA1_Channel4_IRQn);
		HAL_NVIC_DisableIRQ(DMA1_Channel5_IRQn);
	}
}

bool Uart::transmitDma(DmaCallbacks &dmaCb, uint8_t *pData, uint16_t size)
{
	bool retval = false;
	
	dmaCallbacks = &dmaCb;

	HAL_StatusTypeDef status; 
	status = HAL_UART_Transmit_DMA(&huart1, pData, size);
	if(status == HAL_OK) 
	{
		retval = true;
	}
	else
	{
	}
	
	return retval;
}

bool Uart::receiveDma(DmaCallbacks &dmaCb, uint8_t *pData, uint16_t size)
{
	bool retval = false;

	dmaCallbacks = &dmaCb;

	HAL_StatusTypeDef status;
	status = HAL_UART_Receive_DMA(&huart1, pData, size);
	if(status == HAL_OK) 
	{
		retval = true;
	}
	else
	{
	}

	return retval;
}


// callbacks signaling DMA stream completion
// todo test if extern C is really needed
extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		Periph::getUart(1).dmaCallbacks->txComplete();
	}
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		Periph::getUart(1).dmaCallbacks->rxComplete();
	}
}
