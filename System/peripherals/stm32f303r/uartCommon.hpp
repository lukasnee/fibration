/*
 * STM32 UART common driver
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#pragma once

#include "ioDataIF.hpp"

extern "C"
{
#include "stm32f3xx_hal.h"
}

#include <cstdint>
#include <limits>

class UartCommon : public IODataIF
{
public:
    struct Default
    {
        static constexpr std::uint32_t baudrate = 115200;
    };

    virtual bool initUnsafe() = 0;
    virtual bool deinitUnsafe() = 0;

    virtual bool txUnsafe(const std::uint8_t *pData, std::size_t size, std::size_t timeout) override
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

    virtual bool txDmaUnsafe(const std::uint8_t *pData, std::size_t size) override
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

    virtual bool rxUnsafe(std::uint8_t *pData, std::size_t size, std::size_t timeout) override
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

    virtual bool rxDmaUnsafe(std::uint8_t *pData, std::size_t size) override
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
    UartCommon(UART_HandleTypeDef &handle, std::uint32_t baudrate = Default::baudrate) : baudrate(baudrate), handle(handle) {}
    virtual ~UartCommon() = default;

    std::uint32_t baudrate = Default::baudrate;
    UART_HandleTypeDef &handle;
private:
    void operator=(UartCommon const &) = delete;
    UartCommon(UartCommon const &) = delete;
};
