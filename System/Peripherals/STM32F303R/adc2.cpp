#include "adc2.hpp"
#include "system.hpp"
extern "C"
{
#include "stm32f3xx_hal.h"
}

struct Impl
{
    struct Channel
    {
        GPIO_TypeDef *port;
        uint32_t pin;
        uint32_t channel;
    };

    static constexpr std::uint32_t channelsTotal = 10;
    static constexpr std::uint32_t frameBitWidth = 16;
    static constexpr std::uint32_t bitDepth = 12;
    static constexpr uint32_t singleDiff = ADC_SINGLE_ENDED;

    // NOTE: listed GPIO and ADC channel pairs are hardware defined - it is what issss
    static constexpr std::array<Channel, channelsTotal> channels =
        {{{GPIOA, GPIO_PIN_6, ADC_CHANNEL_3},
          {GPIOA, GPIO_PIN_7, ADC_CHANNEL_4},
          {GPIOC, GPIO_PIN_4, ADC_CHANNEL_5},
          {GPIOC, GPIO_PIN_0, ADC_CHANNEL_6},
          {GPIOC, GPIO_PIN_1, ADC_CHANNEL_7},
          {GPIOC, GPIO_PIN_2, ADC_CHANNEL_8},
          {GPIOC, GPIO_PIN_3, ADC_CHANNEL_9},
          {GPIOC, GPIO_PIN_5, ADC_CHANNEL_11},
          {GPIOB, GPIO_PIN_2, ADC_CHANNEL_12},
          {GPIOB, GPIO_PIN_11, ADC_CHANNEL_14}}};

    static bool init()
    {
        bool result = true;

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
        if (HAL_ADC_Init(&hadc2) != HAL_OK)
        {
            result = false;
        }

        return result;
    }

    static bool deinit()
    {
        return (HAL_ADC_DeInit(&Impl::hadc2) == HAL_OK);
    }

    static bool configChannels()
    {
        bool result = true;

        ADC_ChannelConfTypeDef sConfig = {0};

        sConfig.SingleDiff = singleDiff;
        sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        for (std::size_t i = 0; i < channels.size(); i++)
        {
            sConfig.Rank = ADC_REGULAR_RANK_1 + i;
            sConfig.Channel = channels[i].channel;
            if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
            {
                FibSys::panic();
                result = false;
                break;
            }
        }

        return result;
    }

    static void initGpios()
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_InitTypeDef GPIO_InitStruct = {0};

        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        for (const auto &channel : channels)
        {
            GPIO_InitStruct.Pin = channel.pin;
            HAL_GPIO_Init(channel.port, &GPIO_InitStruct);
        }
    }

    static void denitGpios()
    {
        for (const auto &channel : channels)
        {
            HAL_GPIO_DeInit(channel.port, channel.pin);
        }
    }

    static bool initDma(ADC_HandleTypeDef *hadc)
    {
        bool result = false;

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
            FibSys::panic();
        }
        else
        {
            __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc2);

            HAL_NVIC_SetPriority(ADC1_2_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
            result = true;
        }

        return result;
    }

    static bool deinitDma(ADC_HandleTypeDef *hadc)
    {
        return (HAL_DMA_DeInit(hadc->DMA_Handle) == HAL_OK);
    }

    static bool start()
    {
        bool result = false;

        if (HAL_ADC_Start_DMA(&hadc2, sampleBuffer.data(), sampleBuffer.size()) == HAL_OK)
        {
            isRunning = true;
            result = true;
        }

        return result;
    }

    static bool stop()
    {
        bool result = false;

        if (HAL_ADC_Stop_DMA(&hadc2) == HAL_OK)
        {
            isRunning = false;
            result = true;
        }

        return result;
    }

    static bool autoCalibrate()
    {
        bool result = false;

        const bool restartRequired = isRunning;

        if (restartRequired && false == stop())
        {
            FibSys::panic();
        }
        else
        {
            /* NOTE: must be called before HAL_ADC_Start() or after HAL_ADC_Stop() */
            if (HAL_ADCEx_Calibration_Start(&hadc2, singleDiff) != HAL_OK)
            {
                FibSys::panic();
            }
            else
            {
                result = true;
                if (restartRequired && false == start())
                {
                    FibSys::panic();
                }
            }
        }

        return result;
    }

    static bool isRunning;
    static std::array<std::uint32_t, channelsTotal> sampleBuffer;
    static ADC_HandleTypeDef hadc2;
    static DMA_HandleTypeDef hdma_adc2;
};

bool Impl::isRunning = false;
std::array<std::uint32_t, Impl::channelsTotal> Impl::sampleBuffer = {};
ADC_HandleTypeDef Impl::hadc2 = {};
DMA_HandleTypeDef Impl::hdma_adc2 = {};

extern "C" void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC2)
    {
        __HAL_RCC_ADC12_CLK_ENABLE();
        Impl::initGpios();
        Impl::initDma(hadc);
    }
}

extern "C" void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC2)
    {
        Impl::deinitDma(hadc);
        Impl::denitGpios();
        __HAL_RCC_ADC12_CLK_DISABLE();
        HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
    }
}

// TODO: maybe implement flip-flop pattern for reading ADC value securely ???. Not sure if it's an actual problem tho.
extern "C" void DMA2_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&Impl::hdma_adc2);
}

extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
}

extern "C" void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
}

Adc2 &Adc2::getInstance()
{
    static Adc2 instance;
    return instance;
}

// TODO: maybe init and start in ctor is a bad idea... MAYBE!
Adc2::Adc2()
{
    if (false == this->init() or false == this->start())
    {
        FibSys::panic();
    }
}

Adc2::~Adc2()
{
    if (false == this->stop() or false == this->deinit())
    {
        FibSys::panic();
    }
}

std::uint32_t Adc2::getChannelsTotal() { return Impl::channelsTotal; }

std::uint32_t Adc2::getBitDepth() { return Impl::bitDepth; }

std::uint32_t Adc2::getFrameBitWidth() { return Impl::frameBitWidth; }

bool Adc2::init()
{
    bool result = true;

    if (false == Impl::init() || false == Impl::configChannels() || false == Impl::autoCalibrate())
    {
        FibSys::panic();
        result = false;
    }

    return result;
}

bool Adc2::deinit()
{
    return Impl::deinit();
}

bool Adc2::start()
{
    return Impl::start();
}

bool Adc2::stop()
{
    return Impl::stop();
}

// TODO: maybe return as int or special CMSIS fractional int
bool Adc2::getValueUnsafe(std::size_t channelNo, float &value)
{
    bool result = false;

    if (channelNo < this->getChannelsTotal())
    {
        std::uint32_t valueD = Impl::sampleBuffer[channelNo];
        value = static_cast<float>(valueD) / static_cast<float>((1 << this->getBitDepth()) - 1);
        result = true;
    }

    return result;
}
