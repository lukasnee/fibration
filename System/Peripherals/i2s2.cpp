#include "i2s2.hpp"
#include "system.hpp"

#include "stm32f3xx_hal.h"

DMA_HandleTypeDef hdma_spi2_tx;
I2S_HandleTypeDef hi2s2;

extern "C" void DMA1_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_spi2_tx);
}

extern "C" void SPI2_IRQHandler(void)
{
    HAL_I2S_IRQHandler(&hi2s2);
}

I2s2::I2s2()
{
    this->init();
}

I2s2::~I2s2()
{
    this->deinit();
}

bool I2s2::init()
{
    /* Peripheral clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /*
            I2S2 GPIO Configuration:
            PB12     ------> I2S2_WS
            PB13     ------> I2S2_CK
            PB15     ------> I2S2_SD
    */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2S2 DMA Init */
    /* SPI2_TX Init */
    hdma_spi2_tx.Instance = DMA1_Channel5;
    hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_spi2_tx.Init.Mode = DMA_CIRCULAR;
    hdma_spi2_tx.Init.Priority = DMA_PRIORITY_VERY_HIGH;
    if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
    {
        FibSys::panic();
    }

    __HAL_LINKDMA(&hi2s2, hdmatx, hdma_spi2_tx);

    // setup according DMA channel IRQ
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    /* I2S2 interrupt Init */
    HAL_NVIC_SetPriority(SPI2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);

    hi2s2.Instance = SPI2;
    hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
    hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
    hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B;
    hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_DISABLE;
    hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_44K;
    hi2s2.Init.CPOL = I2S_CPOL_LOW;
    hi2s2.Init.ClockSource = I2S_CLOCK_SYSCLK;
    hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;

    return (HAL_I2S_Init(&hi2s2) == HAL_OK);
}

bool I2s2::deinit()
{
    if (HAL_I2S_DeInit(&hi2s2) != HAL_OK)
    {
        return false;
    }

    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();

    /*
        I2S2 GPIO Configuration:
        PB12     ------> I2S2_WS
        PB13     ------> I2S2_CK
        PB15     ------> I2S2_SD
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15);

    /* I2S2 DMA DeInit */
    if (HAL_DMA_DeInit(hi2s2.hdmatx) != HAL_OK)
    {
        return false;
    }

    /* I2S2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(SPI2_IRQn);

    return true;
}

bool I2s2::txCircularUnsafe(const std::uint16_t *pData16, std::uint16_t size16)
{
    bool retval = false;

    if (HAL_I2S_Transmit_DMA(&hi2s2, const_cast<std::uint16_t *>(pData16), size16) == HAL_OK)
    {
        retval = true;
    }
    
    return retval;
}

bool I2s2::txCircularStopUnsafe()
{
    bool retval = false;

    if (HAL_I2S_DMAStop(&hi2s2) == HAL_OK)
    {
        retval = true;
    }

    return retval;
}
