#include "uart2.hpp"
#include "system.hpp"

extern "C"
{
#include "stm32f3xx_hal.h"
}

DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart2_rx;
UART_HandleTypeDef huart2;

extern "C" void DMA1_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart2_tx);
}

extern "C" void DMA1_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart2_rx);
}

extern "C" void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2);
}

Uart2::Uart2(std::uint32_t baudrate) : baudrate(baudrate) {} // TODO: better API for baudrate parametrization

Uart2::~Uart2()
{
}

bool Uart2::initUnsafe()
{
    // setup UART IRQ
    __HAL_RCC_USART2_CLK_ENABLE();
    HAL_NVIC_SetPriority(USART2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    // setup according DMA channels IRQ
    __HAL_RCC_DMA1_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

    // setup gpios for alternative function (uart tx/rx)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // setup DMA channel for uart tx
    hdma_usart2_tx.Instance = DMA1_Channel7;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
        FibSys::panic();
    }
    else
    {
        __HAL_LINKDMA(&huart2, hdmatx, hdma_usart2_tx);
    }

    // setup DMA channel for uart rx
    hdma_usart2_rx.Instance = DMA1_Channel6;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
        FibSys::panic();
    }
    else
    {
        __HAL_LINKDMA(&huart2, hdmarx, hdma_usart2_rx);
    }

    // setup uart itself
    huart2.Instance = USART2;
    huart2.Init.BaudRate = this->baudrate;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    return (HAL_UART_Init(&huart2) == HAL_OK);
}

bool Uart2::deinitUnsafe()
{

    if (HAL_UART_DeInit(&huart2) == HAL_OK)
    {
        return false;
    }

    __HAL_RCC_USART2_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);

    if (HAL_DMA_DeInit(huart2.hdmarx) != HAL_OK)
    {
        return false;
    }

    if (HAL_DMA_DeInit(huart2.hdmatx) != HAL_OK)
    {
        return false;
    }

    HAL_NVIC_DisableIRQ(DMA1_Channel6_IRQn);
    HAL_NVIC_DisableIRQ(DMA1_Channel7_IRQn);

    HAL_NVIC_DisableIRQ(USART2_IRQn);

    return true;
}

bool Uart2::txUnsafe(const std::uint8_t *pData, std::size_t size)
{
    bool retval = false;

    if (size <= static_cast<std::uint16_t>(~0))
    {
        HAL_UART_StateTypeDef status = HAL_UART_GetState(&huart2);
        if ((status == (HAL_UART_STATE_READY | HAL_UART_STATE_BUSY_RX)) || status == HAL_UART_STATE_READY)
        {
            if (HAL_UART_Transmit_DMA(&huart2, const_cast<std::uint8_t *>(pData), static_cast<std::uint16_t>(size)) == HAL_OK)
            {
                retval = true;
            }
        }
    }
    return retval;
}

bool Uart2::rxUnsafe(std::uint8_t *pData, std::size_t size)
{
    bool retval = false;
    if (size <= static_cast<std::uint16_t>(~0))
    {
        HAL_UART_StateTypeDef status = HAL_UART_GetState(&huart2);
        if ((status == (HAL_UART_STATE_READY | HAL_UART_STATE_BUSY_TX)) || status == HAL_UART_STATE_READY)
        {
            if (HAL_UART_Receive_DMA(&huart2, pData, static_cast<std::uint16_t>(size)) == HAL_OK)
            {
                retval = true;
            }
        }
    }

    return retval;
}