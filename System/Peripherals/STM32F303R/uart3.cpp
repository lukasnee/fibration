/*
 * STM32 UART3 instance driver
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 */

// TODO: better API for baudrate parametrization
// TODO: make DMA priorities referenced (centralize)

#include "uart3.hpp"
#include "system.hpp"

extern "C"
{
#include "stm32f3xx_hal.h"
}

DMA_HandleTypeDef hdma_usart3_tx;
DMA_HandleTypeDef hdma_usart3_rx;
UART_HandleTypeDef huart3;

extern "C" void DMA1_Channel2_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart3_tx);
}

extern "C" void DMA1_Channel3_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_usart3_rx);
}

extern "C" void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart3);
}

Uart3::Uart3(std::uint32_t baudrate) : UartCommon(huart3, baudrate) {}
Uart3::~Uart3() {}

bool Uart3::initUnsafe()
{
    bool result = false;

    /* setup UART IRQ */
    __HAL_RCC_USART3_CLK_ENABLE();
    HAL_NVIC_SetPriority(USART3_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    /* setup according DMA channels IRQ */
    __HAL_RCC_DMA1_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 6, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

    /* setup GPIOs for alternative function (UART tx/rx) */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* setup DMA channel for UART TX */
    hdma_usart3_tx.Instance = DMA1_Channel2;
    hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_tx.Init.Mode = DMA_NORMAL;
    hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
    {
        FIBSYS_PANIC();
    }
    else
    {
        __HAL_LINKDMA(&huart3, hdmatx, hdma_usart3_tx);
    }

    /* setup DMA channel for UART RX */
    hdma_usart3_rx.Instance = DMA1_Channel3;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
        FIBSYS_PANIC();
    }
    else
    {
        __HAL_LINKDMA(&huart3, hdmarx, hdma_usart3_rx);
    }

    /* setup UART itself */
    huart3.Instance = USART3;
    huart3.Init.BaudRate = this->baudrate;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart3) == HAL_OK)
    {
        result = true;
    }

    return result;
}

bool Uart3::deinitUnsafe()
{
    bool result = false;

    if (HAL_UART_DeInit(&huart3) != HAL_OK)
    {
    }
    else
    {
        __HAL_RCC_USART3_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9 | GPIO_PIN_10);
        if (HAL_DMA_DeInit(huart3.hdmatx) != HAL_OK)
        {
        }
        else if (HAL_DMA_DeInit(huart3.hdmarx) != HAL_OK)
        {
        }
        else
        {
            HAL_NVIC_DisableIRQ(DMA1_Channel2_IRQn);
            HAL_NVIC_DisableIRQ(DMA1_Channel3_IRQn);
            HAL_NVIC_DisableIRQ(USART3_IRQn);
            result = true;
        }
    }

    return result;
}
