#include "adc2.hpp"
#include "system.hpp"
extern "C"
{
#include "arm_math.h"
#include "stm32f3xx_hal.h"
}

struct Config
{
    static constexpr std::uint32_t channelsTotal = 10;
    static constexpr std::uint32_t frameBitWidth = 16;
    static constexpr std::uint32_t bitDepth = 12;
    static constexpr std::uint32_t singleDiff = ADC_SINGLE_ENDED;
};

static std::array<std::uint32_t, Config::channelsTotal> sampleBuffer = {};
static ADC_HandleTypeDef hadc2 = {};
static DMA_HandleTypeDef hdma_adc2 = {};
static bool isRunning = false;

struct Channel
{
    GPIO_TypeDef *port;
    std::uint32_t pin;
    std::uint32_t channel;
};
/* NOTE: GPIO and ADC channel pairs are hardware defined */
constexpr std::array<Channel, Config::channelsTotal> channels = {
    {{.port = GPIOA, .pin = GPIO_PIN_6, .channel = ADC_CHANNEL_3},
     {.port = GPIOA, .pin = GPIO_PIN_7, .channel = ADC_CHANNEL_4},
     {.port = GPIOC, .pin = GPIO_PIN_4, .channel = ADC_CHANNEL_5},
     {.port = GPIOC, .pin = GPIO_PIN_0, .channel = ADC_CHANNEL_6},
     {.port = GPIOC, .pin = GPIO_PIN_1, .channel = ADC_CHANNEL_7},
     {.port = GPIOC, .pin = GPIO_PIN_2, .channel = ADC_CHANNEL_8},
     {.port = GPIOC, .pin = GPIO_PIN_3, .channel = ADC_CHANNEL_9},
     {.port = GPIOC, .pin = GPIO_PIN_5, .channel = ADC_CHANNEL_11},
     {.port = GPIOB, .pin = GPIO_PIN_2, .channel = ADC_CHANNEL_12},
     {.port = GPIOB, .pin = GPIO_PIN_11, .channel = ADC_CHANNEL_14}}};

Adc2 &Adc2::getInstance()
{
    static Adc2 instance;
    return instance;
}

std::uint32_t Adc2::getChannelsTotal()
{
    return Config::channelsTotal;
}

std::uint32_t Adc2::getBitDepth()
{
    return Config::bitDepth;
}

std::uint32_t Adc2::getFrameBitWidth()
{
    return Config::frameBitWidth;
}

bool Adc2::init()
{
    bool result = false;

    __HAL_RCC_DMA2_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);

    hadc2.Instance = ADC2;
    hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
    hadc2.Init.Resolution = ADC_RESOLUTION_12B;
    hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
    hadc2.Init.ContinuousConvMode = ENABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion = channels.size();
    hadc2.Init.DMAContinuousRequests = ENABLE;
    hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    hadc2.Init.LowPowerAutoWait = DISABLE;
    hadc2.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    if (HAL_OK != HAL_ADC_Init(&hadc2))
    {
        FIBSYS_PANIC();
    }
    else if (false == this->configChannels())
    {
        FIBSYS_PANIC();
    }
    else if (false == this->autoCalibrate())
    {
        FIBSYS_PANIC();
    }
    else
    {
        result = true;
    }

    return result;
}

bool Adc2::configChannels()
{
    bool result = true;

    ADC_ChannelConfTypeDef sConfig = {};

    sConfig.SingleDiff = Config::singleDiff;
    sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    for (std::size_t i = 0; i < channels.size(); i++)
    {
        sConfig.Rank = ADC_REGULAR_RANK_1 + i;
        sConfig.Channel = channels[i].channel;
        if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
        {
            FIBSYS_PANIC();
            result = false;
            break;
        }
    }

    return result;
}

bool Adc2::autoCalibrate()
{
    bool result = false;

    const bool restartRequired = isRunning;
    if (restartRequired && false == this->stop())
    {
        FIBSYS_PANIC();
    }
    else
    {
        /* NOTE: must be called before HAL_ADC_Start() or after HAL_ADC_Stop() */
        if (HAL_ADCEx_Calibration_Start(&hadc2, Config::singleDiff) != HAL_OK)
        {
            FIBSYS_PANIC();
        }
        else
        {
            result = true;
            if (restartRequired && false == this->start())
            {
                FIBSYS_PANIC();
            }
        }
    }

    return result;
}

bool Adc2::start()
{
    bool result = false;

    if (HAL_OK == HAL_ADC_Start_DMA(&hadc2, sampleBuffer.data(), sampleBuffer.size()))
    {
        isRunning = true;
        result = true;
    }

    return result;
}

bool Adc2::stop()
{
    bool result = false;

    if (HAL_OK == HAL_ADC_Stop_DMA(&hadc2))
    {
        sampleBuffer.fill(0);
        isRunning = false;
        result = true;
    }

    return result;
}

bool Adc2::deinit()
{
    return (HAL_OK == HAL_ADC_DeInit(&hadc2));
}

bool Adc2::getValueUnsafe(std::size_t channelNo, std::uint32_t &valueOut)
{
    bool result = false;
    if (channelNo < this->getChannelsTotal())
    {
        valueOut = sampleBuffer[channelNo];
        result = true;
    }

    return result;
}

bool Adc2::Internal::init()
{
    bool result = false;

    __HAL_RCC_ADC12_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {};

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    for (const auto &channel : channels)
    {
        GPIO_InitStruct.Pin = channel.pin;
        HAL_GPIO_Init(channel.port, &GPIO_InitStruct);
    }

    hdma_adc2.Instance = DMA2_Channel1;
    hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc2.Init.Mode = DMA_CIRCULAR;
    hdma_adc2.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_adc2) != HAL_OK)
    {
        FIBSYS_PANIC();
    }
    else
    {
        __HAL_LINKDMA(&hadc2, DMA_Handle, hdma_adc2);

        HAL_NVIC_SetPriority(ADC1_2_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
        result = true;
    }

    return result;
}

bool Adc2::Internal::deinit()
{
    bool result = false;

    if (HAL_OK != HAL_DMA_DeInit(hadc2.DMA_Handle))
    {
        FIBSYS_PANIC();
    }
    else
    {
        for (const auto &channel : channels)
        {
            HAL_GPIO_DeInit(channel.port, channel.pin);
        }
        __HAL_RCC_ADC12_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(ADC1_2_IRQn);

        result = true;
    }

    return result;
}

// TODO: maybe implement flip-flop pattern for reading ADC value securely ???. Not sure if it's an actual problem tho.
void Adc2::Internal::Irq::handle()
{
    HAL_DMA_IRQHandler(&hdma_adc2);
}

void Adc2::Internal::Irq::convCpltCallback()
{
}

void Adc2::Internal::Irq::convHalfCpltCallback()
{
}

// TODO: maybe implement flip-flow read/write sequence
// template <typename SampleFrame, std::size_t samplesTotal>
// struct CircularDmaRxStream
// {
//     using HalfBuffer = std::array<SampleFrame, samplesTotal>;
//     using Buffer = std::pair<HalfBuffer, HalfBuffer>;

//     Buffer buffer;

//     enum State
//     {
//         standby = 0,

//         // starting states
//         firstDmaBeginSecondStandby,
//         firstDmaActiveSecondStandby,
//         firstDmaEndSecondStandby,

//         // running states - ping
//         firstReadyToUseSecondDmaBegin,
//         firstReadyToUseSecondDmaActive,
//         firstReadyToUseSecondDmaDone,

//         // running states - pong
//         firstDmaBeginSecondReadyToUse,
//         firstDmaActiveSecondReadyToUse,
//         firstDmaDoneSecondReadyToUse,

//     } state = State::standby;
// };

// CircularDmaRxStream<std::uint32_t, Config::channelsTotal> circularDmaRxStream;