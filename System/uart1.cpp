#include "uart1.hpp"
#include "system.hpp"

extern "C"
{
#include "stm32f3xx_hal.h"
}

DMA_HandleTypeDef hdma_usart1_tx;
//DMA_HandleTypeDef hdma_usart1_rx;
UART_HandleTypeDef huart1;

extern "C" void DMA1_Channel4_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart1_tx);
}

// extern "C" void DMA1_Channel5_IRQHandler(void)
// {
//     HAL_DMA_IRQHandler(&hdma_usart1_rx);
// }

extern "C" void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}

Uart1::Uart1(std::uint32_t baudrate) : baudrate(baudrate)
{
    this->init();
}

Uart1::~Uart1()
{
    this->deinit();
}

bool Uart1::init()
{
    // setup UART IRQ
    __HAL_RCC_USART1_CLK_ENABLE();
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    // setup according DMA channels IRQ
    __HAL_RCC_DMA1_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    // HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 5, 0);
    // HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

    // setup gpios for alternative function (uart tx/rx)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // setup DMA channel for uart tx
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
        FibSys::panic();
    }
    else
    {
        __HAL_LINKDMA(&huart1, hdmatx, hdma_usart1_tx);
    }

    // // setup DMA channel for uart rx
    // hdma_usart1_rx.Instance = DMA1_Channel5;
    // hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    // hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    // hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    // hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    // hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    // hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    // hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    // if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    // {
    //     FibSys::panic();
    // }
    // else
    // {
    //     __HAL_LINKDMA(&huart1, hdmarx, hdma_usart1_rx);
    // }

    // setup uart itself
    huart1.Instance = USART1;
    huart1.Init.BaudRate = this->baudrate;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    return (HAL_UART_Init(&huart1) == HAL_OK);
}

bool Uart1::deinit()
{
    if (HAL_UART_DeInit(&huart1) != HAL_OK)
    {
        return false;
    }

    __HAL_RCC_USART1_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

    if (HAL_DMA_DeInit(huart1.hdmatx) != HAL_OK)
    {
        return false;
    }

    // if (HAL_DMA_DeInit(huart1.hdmarx) != HAL_OK)
    // {
    //     return false;
    // }

    HAL_NVIC_DisableIRQ(DMA1_Channel4_IRQn);
    // HAL_NVIC_DisableIRQ(DMA1_Channel5_IRQn);

    HAL_NVIC_DisableIRQ(USART1_IRQn);

    return true;
}

bool Uart1::txUnsafe(const std::uint8_t *pData, std::uint16_t size)
{
    bool retval = false;

    if (HAL_UART_GetState(&huart1) == HAL_UART_STATE_READY)
    {
        if (HAL_UART_Transmit_DMA(&huart1, const_cast<std::uint8_t *>(pData), size) == HAL_OK)
        {
            retval = true;
        }
    }

    return retval;
}

bool Uart1::rxUnsafe(std::uint8_t *pData, std::uint16_t size)
{
    bool retval = false;

    if (HAL_UART_GetState(&huart1) == HAL_UART_STATE_READY)
    {
        if (HAL_UART_Receive_DMA(&huart1, pData, size) == HAL_OK)
        {
            retval = true;
        }
    }

    return retval;
}