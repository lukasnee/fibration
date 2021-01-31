#include "stm32f3xx_hal.h"

#include "peripherals.hpp"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    // TODO: remake uart1 with UartInterface
	if(huart == &huart1) { Periph::getUart1().dmaCallbacks->dmaTxCpltCallback(); }
	if(huart == &huart2) { Periph::getUart2().txCpltCallback(); }
}

void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1) { Periph::getUart1().dmaCallbacks->dmaTxHalfCpltCallback(); }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1) { Periph::getUart1().dmaCallbacks->dmaRxCpltCallback(); }
	if(huart == &huart2) { Periph::getUart2().rxCpltCallback(); }
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1) { Periph::getUart1().dmaCallbacks->dmaRxHalfCpltCallback(); }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1) { Periph::getUart1().dmaCallbacks->dmaErrorCallback(); }
}

void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1) { Periph::getUart1().dmaCallbacks->dmaAbortCpltCallback(); }
}

void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1) { Periph::getUart1().dmaCallbacks->dmaAbortTransmitCpltCallback(); }
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1) { Periph::getUart1().dmaCallbacks->dmaAbortReceiveCpltCallback(); }
}
