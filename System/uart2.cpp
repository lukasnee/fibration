#include "uart2.hpp"
#include "system.hpp"

extern "C"
{
#include "stm32f3xx_hal.h"
}

#include <limits>

UART_HandleTypeDef huart2;

Uart2::Uart2(std::uint32_t baudrate) : baudrate(baudrate)
{
    this->init();
}

Uart2::~Uart2()
{
    this->deinit();
}

bool Uart2::init()
{
    bool retval = false;

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

    retval = (HAL_UART_Init(&huart2) == HAL_OK);

    return retval;
}

bool Uart2::deinit()
{
    bool retval = false;

    retval = (HAL_UART_DeInit(&huart2) == HAL_OK);

    return retval;
}

bool Uart2::txUnsafe(const std::uint8_t *pData, std::uint16_t size)
{
    bool retval = false;

    if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_READY)
    {
        retval = (HAL_UART_Transmit_IT(&huart2, pData, size) == HAL_OK);
            retval = true;
        }
    }

    return retval;
}

bool Uart2::rxUnsafe(std::uint8_t *pData, std::uint16_t size)
{
    bool retval = false;

    if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_READY)
    {
        retval = (HAL_UART_Receive_IT(&huart2, pData, size) == HAL_OK);
            retval = true;
        }
    }

    return retval;
}