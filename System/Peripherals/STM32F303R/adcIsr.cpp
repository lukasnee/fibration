#include "adc2.hpp"
// #include "system.hpp"
extern "C"
{
#include "stm32f3xx_hal.h"
}

extern "C" void DMA2_Channel1_IRQHandler(void)
{
    Adc2::getInstance().dma.handle();
}

extern "C" void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC2)
    {
        Adc2::getInstance().dma.init();
    }
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC2)
    {
        Adc2::getInstance().dma.convCpltCallback();
    }
}

extern "C" void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC2)
    {
        Adc2::getInstance().dma.convHalfCpltCallback();
    }
}

extern "C" void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC2)
    {
        Adc2::getInstance().dma.deinit();
    }
}
