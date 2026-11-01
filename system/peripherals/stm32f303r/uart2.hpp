// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "uartCommon.hpp"

class Uart2 : public UartCommon {
public:
    static Uart2 &get_instance();

protected:
    virtual bool ll_init() override;
    virtual bool ll_deinit() override;

private:
    Uart2();
    ~Uart2() = default;

    Uart2(Uart2 const &) = delete;
    Uart2 &operator=(Uart2 const &) = delete;
};
