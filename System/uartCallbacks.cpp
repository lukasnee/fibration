#include "stm32f3xx_hal.h"

#include "peripherals.hpp"

extern UART_HandleTypeDef huart1;

// callbacks signaling DMA stream completion
// todo test if extern C is really needed
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		Periph::getUart1().dmaCallbacks->txComplete();
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1)
	{
		Periph::getUart1().dmaCallbacks->rxComplete();
	}
}
