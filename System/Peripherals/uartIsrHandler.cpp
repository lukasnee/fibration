extern "C"
{
#include "stm32f3xx_hal.h"
}
#include "peripherals.hpp"

extern UART_HandleTypeDef huart2;

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2)
    {
        Periph::getUart2().txCpltIsrCalback();
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart1) { Periph::getUart1().rxCpltIsrCalback(); }
	if(huart == &huart2) { Periph::getUart2().rxCpltIsrCalback(); }
}


// TODO: if needed
// void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart1) { Periph::getUart1().txHalfCpltIsrCalback(); }
// 	if(huart == &huart2) { Periph::getUart2().txHalfCpltIsrCalback(); }
// }

// void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart1) { Periph::getUart1().rxHalfCpltIsrCalback(); }
// 	if(huart == &huart2) { Periph::getUart2().rxHalfCpltIsrCalback(); }
// }

// void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart1) { Periph::getUart1().errorIsrCalback(); }
// 	if(huart == &huart2) { Periph::getUart2().errorIsrCalback(); }
// }

// void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart1) { Periph::getUart1().abortCpltIsrCalback(); }
// 	if(huart == &huart2) { Periph::getUart2().abortCpltIsrCalback(); }
// }

// void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart1) { Periph::getUart1().abortTransmitCpltIsrCalback(); }
// 	if(huart == &huart2) { Periph::getUart2().abortTransmitCpltIsrCalback(); }
// }

// void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart1) { Periph::getUart1().abortReceiveCpltIsrCalback(); }
// 	if(huart == &huart2) { Periph::getUart2().abortReceiveCpltIsrCalback(); }
// }
