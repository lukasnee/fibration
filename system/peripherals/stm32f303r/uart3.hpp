// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

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
