// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "ln/drivers/EventDrivenStream.hpp"

#include "stm32f3xx_hal.h"

#include <cstdint>
#include <limits>

class UartCommon : public ln::drivers::EventDriven::Stream<
                       ln::drivers::EventDriven::StreamType::rw> {
public:
    struct Default {
        static constexpr std::uint32_t baudrate = 115200;
    };

    bool ll_write_async(const std::uint8_t *data, std::size_t size) final {
        if (!data) {
            return false;
        }
        if (size > std::numeric_limits<std::uint16_t>::max()) {
            return false;
        }
        if (HAL_OK != HAL_UART_Transmit_DMA(&this->handle,
                                            const_cast<std::uint8_t *>(data),
                                            static_cast<std::uint16_t>(size))) {
            return false;
        }
        return true;
    }

    bool ll_read_async(std::uint8_t *pData, std::size_t size) final {
        if (!pData) {
            return false;
        }
        if (size > std::numeric_limits<std::uint16_t>::max()) {
            return false;
        }
        if (HAL_OK != HAL_UART_Receive_DMA(&this->handle, pData,
                                           static_cast<std::uint16_t>(size))) {
            return false;
        }
        return true;
    }

protected:
    UartCommon(UART_HandleTypeDef &handle,
               std::uint32_t baudrate = Default::baudrate)
        : baudrate(baudrate), handle(handle) {}
    virtual ~UartCommon() = default;

    std::uint32_t baudrate = Default::baudrate;
    UART_HandleTypeDef &handle;

private:
    void operator=(UartCommon const &) = delete;
    UartCommon(UartCommon const &) = delete;
};
