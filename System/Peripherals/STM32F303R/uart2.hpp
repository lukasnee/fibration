
/*
 * STM32 UART2 instance driver 
 *
 * Copyright (C) 2021 Lukas Neverauskis <lukas.neverauskis@gmail.com>
 *
 */

#pragma once

#include "uartCommon.hpp"
#include "ioDataIF.hpp"

#include <cstdint>

class Uart2 : public UartCommon, public IODataIF
{
public:
    Uart2(std::uint32_t baudrate = 115200);
    ~Uart2();

protected:
    bool initUnsafe() override;
    bool deinitUnsafe() override;

private:
    void operator=(Uart2 const &) = delete;
    Uart2(Uart2 const &) = delete;

    std::uint32_t baudrate;
};
