/*
 * STM32 UART2 instance driver
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#pragma once

#include "uartCommon.hpp"

#include <cstdint>

class Uart2 : public UartCommon
{
public:
    Uart2(std::uint32_t baudrate = UartCommon::Default::baudrate);
    ~Uart2() = default;

protected:
    virtual bool initUnsafe() override;
    virtual bool deinitUnsafe() override;

private:
    void operator=(Uart2 const &) = delete;
    Uart2(Uart2 const &) = delete;
};
