#include "adc2.hpp"
#include "system.hpp"
extern "C"
{
#include "stm32f3xx_hal.h"
}
ADC_HandleTypeDef hadc2;

Adc2 &Adc2::getInstance()
{
    static Adc2 instance;
    return instance;
}

Adc2::Adc2()
{
    this->init();
}

Adc2::~Adc2()
{
    this->deinit();
}

bool Adc2::init()
{
    bool result = true;

    __HAL_RCC_GPIOA_CLK_ENABLE();

    hadc2.Instance = ADC2;
    hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc2.Init.Resolution = ADC_RESOLUTION_12B;
    hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc2.Init.ContinuousConvMode = DISABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion = 1;
    hadc2.Init.DMAContinuousRequests = DISABLE;
    hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc2.Init.LowPowerAutoWait = DISABLE;
    hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    if (HAL_ADC_Init(&hadc2) != HAL_OK)
    {
        FibSys::panic();
        result = false;
    }

    return result;
}

bool Adc2::deinit()
{
    bool result = true;

    result &= (HAL_ADC_DeInit(&hadc2) == HAL_OK);

    return result;
}

std::uint32_t Adc2::getBitDepth() { return 12; }

std::uint32_t Adc2::getChannelsTotal() { return 6; }

bool Adc2::getValueUnsafe(std::uint32_t channelNo, float &value)
{
    bool result = false;

    constexpr std::uint32_t channels[] = {
        ADC_CHANNEL_3,
        ADC_CHANNEL_4,
        ADC_CHANNEL_5,
        ADC_CHANNEL_6,
        ADC_CHANNEL_7,
        ADC_CHANNEL_8};

    if (channelNo && channelNo <= this->getChannelsTotal())
    {
        ADC_ChannelConfTypeDef sConfig;
        sConfig.Channel = channels[channelNo - 1];
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) == HAL_OK)
        {
            if (HAL_ADC_Start(&hadc2) == HAL_OK)
            {
                if (HAL_ADC_PollForConversion(&hadc2, 1000) == HAL_OK)
                {
                    std::uint32_t valueD = HAL_ADC_GetValue(&hadc2);
                    value = static_cast<float>(valueD) / static_cast<float>((1 << this->getBitDepth()) - 1);
                    result = true;
                }
            }
            if (HAL_ADC_Stop(&hadc2) == HAL_OK) {
                result = false;
            }
        }
    }

    return result;
}

extern "C" void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hadc->Instance == ADC2)
    {
        /**ADC2 GPIO Configuration
        PC0     ------> ADC2_IN6
        PC1     ------> ADC2_IN7
        PC2     ------> ADC2_IN8
        PA6     ------> ADC2_IN3
        PA7     ------> ADC2_IN4
        PC4     ------> ADC2_IN5
        */

        __HAL_RCC_ADC12_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        __HAL_RCC_GPIOC_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    }
}

extern "C" void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC2)
    {
        /**ADC2 GPIO Configuration
        PC0     ------> ADC2_IN6
        PC1     ------> ADC2_IN7
        PC2     ------> ADC2_IN8
        PA6     ------> ADC2_IN3
        PA7     ------> ADC2_IN4
        PC4     ------> ADC2_IN5
        */
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4);
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6 | GPIO_PIN_7);
    }
}
