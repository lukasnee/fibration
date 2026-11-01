// Copyright (c) 2026 Lukas Neverauskis <lukas.neverauskis@gmail.com>
// SPDX-License-Identifier: GPL-2.0-only

#pragma once

#include "uartCommon.hpp"

class Uart3 : public UartCommon {
public:
    static Uart3 &get_instance();

protected:
    virtual bool ll_init() override;
    virtual bool ll_deinit() override;

private:
    Uart3();
    ~Uart3() = default;
    Uart3(Uart3 const &) = delete;
    Uart3 &operator=(Uart3 const &) = delete;
};
