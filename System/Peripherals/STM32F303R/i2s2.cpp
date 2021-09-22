#include "i2s2.hpp"
#include "system.hpp"

extern "C"
{
#include "stm32f3xx_hal.h"
}

I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_spi2_rx;
DMA_HandleTypeDef hdma_spi2_tx;

I2s2 &I2s2::getInstance()
{
    static I2s2 instance;
    return instance;
}

I2s2::I2s2() {}

I2s2::~I2s2() {}

std::uint32_t I2s2::getSampleRateInHz() const { return 44100; }

std::uint32_t I2s2::getSampleBitDepthInBits() const { return 24; }

std::uint32_t I2s2::getSampleFrameSizeInBytes() const { return sizeof(std::uint32_t); }

bool I2s2::init()
{

    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);

    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

    hi2s2.Instance = SPI2;
    hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s2.Init.DataFormat = I2S_DATAFORMAT_24B;
    hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
    hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_44K;
    hi2s2.Init.CPOL = I2S_CPOL_LOW;
    hi2s2.Init.ClockSource = I2S_CLOCK_SYSCLK;
    hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;

    return (HAL_I2S_Init(&hi2s2) == HAL_OK);
}

bool I2s2::deinit()
{
    return (HAL_I2S_DeInit(&hi2s2) == HAL_OK);
}

bool I2s2::txRxCircularDmaUnsafe(const std::uint16_t *pTxData16, std::uint16_t *pRxData16, std::uint16_t size8)
{
    /* ATTENTION: the following function `Size` parameter stands for amount of samples. 
    When I2S is configured in:
    - 16-bit mode, sample is considered 16-bit wide 
    - 24-bit or 32-bit mode, sample is considered 32-bit wide */
    return (HAL_I2SEx_TransmitReceive_DMA(&hi2s2,
                                          const_cast<std::uint16_t *>(pTxData16),
                                          pRxData16,
                                          (size8 / sizeof(std::uint32_t))) == HAL_OK);
}

bool I2s2::txRxCircularDmaStopUnsafe()
{
    return (HAL_I2S_DMAStop(&hi2s2) == HAL_OK);
}

extern "C" void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_spi2_rx);
}

extern "C" void DMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_spi2_tx);
}

static void Error_Handler()
{
    FIBSYS_PANIC();
}

extern "C" void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hi2s->Instance == SPI2)
    {
        /* Peripheral clock enable */
        __HAL_RCC_SPI2_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        /**I2S2 GPIO Configuration
        PB12     ------> I2S2_WS
        PB13     ------> I2S2_CK
        PB14     ------> I2S2_ext_SD
        PB15     ------> I2S2_SD
        PC6     ------> I2S2_MCK
        */
        GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_14;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF5_I2S2ext;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI2;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* I2S2 DMA Init */
        /* SPI2_RX Init */
        hdma_spi2_rx.Instance = DMA1_Channel4;
        hdma_spi2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_spi2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_spi2_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_spi2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_spi2_rx.Init.Mode = DMA_CIRCULAR;
        hdma_spi2_rx.Init.Priority = DMA_PRIORITY_HIGH;
        if (HAL_DMA_Init(&hdma_spi2_rx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hi2s, hdmarx, hdma_spi2_rx);

        /* SPI2_TX Init */
        hdma_spi2_tx.Instance = DMA1_Channel5;
        hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
        hdma_spi2_tx.Init.Mode = DMA_CIRCULAR;
        hdma_spi2_tx.Init.Priority = DMA_PRIORITY_HIGH;
        if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(hi2s, hdmatx, hdma_spi2_tx);
    }
}

extern "C" void HAL_I2S_MspDeInit(I2S_HandleTypeDef *hi2s)
{
    if (hi2s->Instance == SPI2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_SPI2_CLK_DISABLE();

        /**I2S2 GPIO Configuration
        PB12     ------> I2S2_WS
        PB13     ------> I2S2_CK
        PB14     ------> I2S2_ext_SD
        PB15     ------> I2S2_SD
        PC6     ------> I2S2_MCK
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6);

        /* I2S2 DMA DeInit */
        HAL_DMA_DeInit(hi2s->hdmarx);
        HAL_DMA_DeInit(hi2s->hdmatx);
    }
}
