extern "C"
{
#include "stm32f3xx_hal.h"
}
#include "resources.hpp"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2)
    {
        Periph::getUart2().txDmaCpltIsrCallback();
    }
    else if (huart == &huart3)
    {
        Periph::getUart3().txDmaCpltIsrCallback();
    }
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2)
    {
        Periph::getUart2().rxDmaCpltIsrCallback();
    }
    else if (huart == &huart3)
    {
        Periph::getUart3().rxDmaCpltIsrCallback();
    }
}

// TODO: if ever needed
// void HAL_UART_TxHalfCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart2) { Periph::getUart2().txHalfCpltIsrCalback(); }
// 	if(huart == &huart3) { Periph::getUart3().txHalfCpltIsrCalback(); }
// }

// void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart2) { Periph::getUart2().rxHalfCpltIsrCalback(); }
// 	if(huart == &huart3) { Periph::getUart3().rxHalfCpltIsrCalback(); }
// }

// void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart2) { Periph::getUart2().errorIsrCalback(); }
// 	if(huart == &huart3) { Periph::getUart3().errorIsrCalback(); }
// }

// void HAL_UART_AbortCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart2) { Periph::getUart2().abortCpltIsrCalback(); }
// 	if(huart == &huart3) { Periph::getUart3().abortCpltIsrCalback(); }
// }

// void HAL_UART_AbortTransmitCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart2) { Periph::getUart2().abortTransmitCpltIsrCalback(); }
// 	if(huart == &huart3) { Periph::getUart3().abortTransmitCpltIsrCalback(); }
// }

// void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart)
// {
// 	if(huart == &huart2) { Periph::getUart2().abortReceiveCpltIsrCalback(); }
// 	if(huart == &huart3) { Periph::getUart3().abortReceiveCpltIsrCalback(); }
// }
