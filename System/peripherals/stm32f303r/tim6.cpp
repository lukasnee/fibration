#include "tim6.hpp"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_it.h"

#include <limits>

TIM_HandleTypeDef htim6;

static void Error_Handler()
{
    HardFault_Handler();
}

extern "C" void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 6, 0);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
}

static void sTim6Init(std::uint16_t prescaler, std::uint16_t period)
{
    TIM_MasterConfigTypeDef sMasterConfig = {};
    htim6.Instance = TIM6;
    htim6.Init.Prescaler = prescaler;
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.Period = period;
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

extern "C" void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
    }
}

void sTim6Deinit()
{
    HAL_TIM_Base_DeInit(&htim6);
}

Tim6::Tim6(std::uint16_t prescaler, std::uint16_t period) : counterValue(0)
{
    sTim6Init(prescaler, period);
}

void Tim6::start()
{
    HAL_TIM_Base_Start_IT(&htim6);
}

void Tim6::overflowCallback()
{
    this->counterValue += std::numeric_limits<std::uint16_t>::max();
}

Tim6::CounterType Tim6::getCounterValue()
{
    return this->counterValue + htim6.Instance->CNT;
}

void Tim6::stop()
{
    HAL_TIM_Base_Stop_IT(&htim6);
}

Tim6::~Tim6()
{
    sTim6Deinit();
}