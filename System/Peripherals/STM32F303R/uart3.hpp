/*
 * STM32 UART3 instance driver
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#pragma once

#include "uartCommon.hpp"

#include <cstdint>

class Uart3 : public UartCommon
{
public:
    Uart3(std::uint32_t baudrate = UartCommon::Default::baudrate);
    ~Uart3() = default;

protected:
    virtual bool initUnsafe() override;
    virtual bool deinitUnsafe() override;

private:
    void operator=(Uart3 const &) = delete;
    Uart3(Uart3 const &) = delete;
};
