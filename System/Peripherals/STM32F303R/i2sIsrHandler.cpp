extern "C" {
#include "stm32f3xx_hal.h"
}

#include "resources.hpp"

extern I2S_HandleTypeDef hi2s2;

extern "C" void HAL_I2SEx_TxRxCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if(hi2s == &hi2s2) { Periph::getI2s2().txRxCpltIsrCalback(); }
}

extern "C" void HAL_I2SEx_TxRxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
    if(hi2s == &hi2s2) { Periph::getI2s2().txRxHalfCpltIsrCalback(); }
}

// TODO: if needed
// void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
// {
// 	if(hi2s == &hi2s2) { Periph::getI2s2().rxCpltIsrCalback(); }
// }

// void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
// {
// 	if(hi2s == &hi2s1) { Periph::getI2s2().rxHalfCpltIsrCalback(); }
// }

// void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s)
// {
// 	if(hi2s == &hi2s1) { Periph::getI2s2().errorIsrCalback(); }
// }

// void HAL_I2S_AbortCpltCallback(I2S_HandleTypeDef *hi2s)
// {
// 	if(hi2s == &hi2s1) { Periph::getI2s2().abortCpltIsrCalback(); }
// }

// void HAL_I2S_AbortTransmitCpltCallback(I2S_HandleTypeDef *hi2s)
// {
// 	if(hi2s == &hi2s1) { Periph::getI2s2().abortTransmitCpltIsrCalback(); }
// }

// void HAL_I2S_AbortReceiveCpltCallback(I2S_HandleTypeDef *hi2s)
// {
// 	if(hi2s == &hi2s1) { Periph::getI2s2().abortReceiveCpltIsrCalback(); }
// }
