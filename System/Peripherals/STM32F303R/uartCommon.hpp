/*
 * STM32 UART common driver
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#pragma once

extern "C"
{
#include "stm32f3xx_hal.h"
}

#include <cstdint>
#include <limits>

class UartCommon
{
public:
    virtual bool initUnsafe() = 0;
    virtual bool deinitUnsafe() = 0;

    bool txUnsafe(const std::uint8_t *pData, std::size_t size, std::size_t timeout)
    {
        bool retval = false;

        if (pData && size <= std::numeric_limits<std::uint16_t>::max())
        {
            if (HAL_OK == HAL_UART_Transmit(&this->handle, const_cast<std::uint8_t *>(pData), static_cast<std::uint16_t>(size), timeout))
            {
                retval = true;
            }
        }
        return retval;
    }

    bool txDmaUnsafe(const std::uint8_t *pData, std::size_t size)
    {
        bool retval = false;

        if (pData && size <= std::numeric_limits<std::uint16_t>::max())
        {
            if (HAL_OK == HAL_UART_Transmit_DMA(&this->handle, const_cast<std::uint8_t *>(pData), static_cast<std::uint16_t>(size)))
            {
                retval = true;
            }
        }
        return retval;
    }

    bool rxUnsafe(std::uint8_t *pData, std::size_t size, std::size_t timeout)
    {
        bool retval = false;
        if (pData && size <= std::numeric_limits<std::uint16_t>::max())
        {
            if (HAL_OK == HAL_UART_Receive(&this->handle, pData, static_cast<std::uint16_t>(size), timeout))
            {
                retval = true;
            }
        }

        return retval;
    }

    bool rxDmaUnsafe(std::uint8_t *pData, std::size_t size)
    {
        bool retval = false;
        if (pData && size <= std::numeric_limits<std::uint16_t>::max())
        {
            if (HAL_OK == HAL_UART_Receive_DMA(&this->handle, pData, static_cast<std::uint16_t>(size)))
            {
                retval = true;
            }
        }

        return retval;
    }

protected:
    UartCommon(UART_HandleTypeDef &handle) : handle(handle) {}

private:
    virtual ~UartCommon() = default;
    void operator=(UartCommon const &) = delete;
    UartCommon(UartCommon const &) = delete;

    // std::uint32_t baudrate;
    UART_HandleTypeDef &handle;
};
