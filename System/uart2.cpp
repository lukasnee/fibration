#include "uart2.hpp"
#include "system.hpp"

extern "C" {
#include "stm32f3xx_hal.h"
}

#include <limits>

UART_HandleTypeDef huart2;

static void sInitUart2(std::uint32_t baudrate)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = baudrate;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
		FibSys::panic();
    }
}

static void sDeinitUart2()
{
    HAL_UART_DeInit(&huart2);
}


Uart2::Uart2(std::uint32_t baudrate) : baudrate(baudrate)
{
    sInitUart2(this->baudrate);
}

void Uart2::putc(char c)
{
    HAL_UART_Transmit(&huart2, reinterpret_cast<uint8_t*>(&c), 1, std::numeric_limits<std::uint32_t>::max());
}

char Uart2::getc(void)
{
    char c;
    HAL_UART_Receive(&huart2, reinterpret_cast<uint8_t*>(&c), 1, std::numeric_limits<std::uint32_t>::max());
    return c;
}

Uart2::~Uart2()
{
    sDeinitUart2();
}
